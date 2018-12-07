/**
 * @addtogroup bsm_shell_if_gps BSM Shell GPS interface
 * @{
 *
 * @section bsm_shell_if_gps_doc MKx GPS interface
 * The MKx runs a gpsd server http://http://gpsd.berlios.de/ to allow
 * applications to access its on-board GPS receiver.
 *
 * The applications would typically access the GPS position via libgps.
 *
 * It is also possible to access the GPS position from the command line
 * to confirm that the GPS is operating correctly. For the GPS to operate
 * correctly the MKx must have a GPS antenna fitted, and the antenna must
 * have a clear view of the sky.
@verbatim
 cgps
 gpspipe -r
@endverbatim
 *
 * @file
 *
 * GPS Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_GPS_H_
#define __IF_GPS_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "gps.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Default GPSD host
#define GPS_DEFAULT_HOSTIP "127.0.0.1"
/// Default GPSD port
#define GPS_DEFAULT_PORT   "2947"

/// GPS position information cached
#define GPS_UPDATE_POSITION           0
/// GPS position information available
#define GPS_UPDATE_POSITION_AND_STORE 1


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// GPS fix information
typedef struct GPSInfo
{
  /// NZ if GPS is on line, 0 if not
  double Online;
  /// accumulated PVT data
  struct gps_fix_t Fix;
  /// Number of satellites used in solution
  int SatellitesUsed;
  /// Accuracy blob (DF_SAEPositionalAccuracy (Desc Name) Record 44)
  uint32_t SAEPositionalAccuracy;
  /// Mask of GPS updated bits
  gps_mask_t GpsMask;
} tGPSInfo;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Start GPS
int GPSInit(
  void);

// Stop GPS
void GPSDeinit(void);

// Handle GPS
int GPSUpdate(
  tGPSInfo *pGpsStore);


#endif /* __IF_GPS_H_ */

// Close the doxygen group
/**
 * @}
 */

