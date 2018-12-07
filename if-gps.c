/**
 * @addtogroup bsm_shell_if_gps BSM Shell GPS interface
 * @{
 *
 * @file
 *
 * Module code for GPS interface
 *
 * @ref GPSInit() : Initialise GPS, using libgps to access gpsd and set the GPS
 * processing callback function. The initialisation provides a file descriptor
 * for the main management routine to monitor in a select() call.
 *
 * @ref GPSUpdate() : When data becomes available on the file descriptor, the
 * 'update' routine is called to fetch data from the interface.
 * If all required GPS data are cached, then the system store is updated.
 *
 * @ref GPSDeinit() : A de-initialisation routine is provided for the management
 * routine to call at application exit.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <poll.h>


#include "gps.h"
#include "if-gps.h"

#include "debug-levels.h"

#ifndef  __USE_ISOC99
/// make sure we get NAN
#define __USE_ISOC99 1
#endif
#include <math.h>

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// What data needed before committing GPS data
#define GPS_REQUIRED_SET_BITS (TIME_SET|LATLON_SET|ALTITUDE_SET|\
                               SPEED_SET|TRACK_SET|MODE_SET)

//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// GPS IP
static const char *pHost = GPS_DEFAULT_HOSTIP;

/// GPS port
static const char *pPort = GPS_DEFAULT_PORT;

/// GPS data store
static struct gps_data_t GpsData;

/// GPS data store access
static struct gps_data_t *pGpsData = &GpsData;

/// Local GPS cumulative store
static tGPSInfo GpsCache = {NAN,{NAN,0,0,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},0,-1,0};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief Initialise the GPS interface
 * @return File descriptor to monitor for GPS input
 *
 * Open GPS interface and set up the GPS handler. This function provides
 * a file handle for the management function to monitor to see when data
 * becomes available for reading.
 *
 */
int GPSInit(void)
{
  int Res;

  d_fnstart(D_INTERN, NULL, "()\n");

  // attempt to attach to the gpsd and create pGpsData object
  Res = gps_open(pHost, pPort, pGpsData);

  d_printf(D_DEBUG, NULL, "gps_open(%s,%s, %p) = %d\n",
           pHost, pPort, pGpsData, Res);

  if (Res != 0)
  {
    char *pErrStr;
    switch (errno)
    {
      case NL_NOSERVICE:
        pErrStr = "can't get service entry";
        break;
      case NL_NOHOST:
        pErrStr = "can't get host entry";
        break;
      case NL_NOPROTO:
        pErrStr = "can't get protocol entry";
        break;
      case NL_NOSOCK:
        pErrStr = "can't create socket";
        break;
      case NL_NOSOCKOPT:
        pErrStr = "error SETSOCKOPT SO_REUSEADDR";
        break;
      case NL_NOCONNECT:
        pErrStr = "can't connect to host";
        break;
      default:
        pErrStr = "Unknown";
        break;
    }
    d_error(D_WARN, NULL, "No gpsd running or network error: %d, %s\n",
            errno, pErrStr);
    exit(1);
  }

  // setup the select call
  (void)gps_stream(pGpsData, WATCH_ENABLE, NULL);

  d_fnend(D_INTERN, NULL, "() = %d\n", pGpsData->gps_fd);

  return pGpsData->gps_fd;
}


/**
 * @brief GPS interface de-initialisation
 *
 * Clean up GPS interface
 *
 */
void GPSDeinit(void)
{
  d_fnstart(D_INTERN, NULL, "()\n");

  // tidy up interfaces
  (void)gps_stream(pGpsData, WATCH_DISABLE, NULL);
  (void)gps_close(pGpsData);

  d_fnend(D_INTERN, NULL, "()\n");
}

/**
 * @brief Update provided store with GPS information
 * @param pGpsStore Data structure for updating
 * @return GPS_UPDATE_POSITION_AND_STORE for updated store (caller can use),
 *         GPS_UPDATE_POSITION for successful cache update (store not updated),
 *         or a negative errno
 *
 * Read GPSD and update appropriate fields.  Since update comes in two
 * sentences (GPGGA and GPRMC) the information is cached locally to determine
 * when all required fields are available. Once available the system
 * store is updated and the cache marked as dirty.
 *
 */
int GPSUpdate(
  tGPSInfo *pGpsStore)
{
  int RetVal = -ENOSYS;
  d_fnstart(D_INTERN, NULL, "(pGpsStore %p)\n",
            pGpsStore);

  // Read data from gpsd
  int Res = gps_read(pGpsData);

  if (Res < 0)
  {
    d_printf(D_DEBUG, NULL, "gps_read failed (%d)\n", Res);
    RetVal = -ENOMSG;
    goto Exit;
  }

  const struct gps_fix_t *Fix = &pGpsData->fix;
  gps_mask_t LocalSet = pGpsData->set;

  d_printf(D_DEBUG, NULL, "gps: mode %d, time %f, lat %f, lon %f, alt %f, head %f, speed %f\n",
           Fix->mode,
           Fix->time,
           Fix->latitude,
           Fix->longitude,
           Fix->altitude,
           Fix->track,
           Fix->speed);

  // Set a mimnimum level for fix quality
  if ((Fix->mode        <= MODE_NO_FIX) ||
      (pGpsData->status == STATUS_NO_FIX))
  {
    d_printf(D_DEBUG, NULL, "No GPS fix (%d,%d)\n",
             Fix->mode, pGpsData->status);
    // No fix at all, ensure all data updated with 'no position'
    LocalSet = GPS_REQUIRED_SET_BITS;
  }

  // Store which fields are available
  GpsCache.GpsMask |= LocalSet;

  // Update cache with available fields

  if (LocalSet & ONLINE_SET)
    GpsCache.Online        = pGpsData->online;
  if (LocalSet & TIME_SET)
  {
    GpsCache.Fix.time      = Fix->time;
    GpsCache.Fix.ept       = Fix->ept;
  }
  if (LocalSet & LATLON_SET)
  {
    GpsCache.Fix.latitude  = Fix->latitude;
    GpsCache.Fix.longitude = Fix->longitude;
    GpsCache.Fix.epx       = Fix->epx;
    GpsCache.Fix.epy       = Fix->epy;
  }
  if (LocalSet & ALTITUDE_SET)
  {
    GpsCache.Fix.altitude  = Fix->altitude;
    GpsCache.Fix.epv       = Fix->epv;
  }
  // speed is sometimes rubbish unless TRACK is set, should use
  // if (pGpsInfo->set & SPEED_SET), but check TRACK instead
  if (LocalSet & TRACK_SET)
  {
    GpsCache.Fix.speed     = Fix->speed;
    GpsCache.Fix.eps       = Fix->eps;
  }
  if (LocalSet & TRACK_SET)
  {
    GpsCache.Fix.track     = Fix->track;
    GpsCache.Fix.epd       = Fix->epd;
  }
  if (LocalSet & CLIMB_SET)
  {
    GpsCache.Fix.climb     = Fix->climb;
    GpsCache.Fix.epc       = Fix->epc;
  }
  if (LocalSet & MODE_SET)
    GpsCache.Fix.mode      = Fix->mode;
  if (LocalSet & SATELLITE_SET)
    GpsCache.SatellitesUsed = pGpsData->satellites_used;

  // Got here, so no error
  RetVal = GPS_UPDATE_POSITION;

  // When all required information available, update store
  if ((GpsCache.GpsMask & GPS_REQUIRED_SET_BITS) == GPS_REQUIRED_SET_BITS)
  {
    memcpy(pGpsStore, &GpsCache, sizeof(tGPSInfo));
    d_printf(D_DEBUG, NULL, "all GPS fields acquired (0x%04x)\n",
             (uint32_t)GpsCache.GpsMask);

    // Clear mask
    GpsCache.GpsMask = 0;
    // Indicate Store updated
    RetVal = GPS_UPDATE_POSITION_AND_STORE;
  }

Exit:
  d_fnend(D_INTERN, NULL, "() = %d\n", RetVal);

  return RetVal;
}


// Close the doxygen group
/**
 * @}
 */


