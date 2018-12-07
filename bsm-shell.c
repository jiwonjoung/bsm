/**
 * @addtogroup bsm_shell_main BSM Shell Main Application
 * @{
 *
 *
 * @file
 *
 * BSM-Shell: application using MKx interfaces to construct and send
 * regular Basic Safety Messages
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "debug-levels.h"

#include "dot3-wsmp.h"

#include "if-gps.h"
#include "if-can.h"
#include "if-p1609.h"
#include "if-p1609sync.h"
#include "if-udp.h"
#include "if-vic.h"

#include "bsm-msg.h"
#include "bsm-sec.h"
#include "bsm-shell.h"
#include "bsm-opts.h"

#include "util-log.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// Handle poll input types
#define POLL_INPUT (POLLIN  | POLLPRI)

/// Handle poll error types
#define POLL_ERROR (POLLERR | POLLHUP | POLLNVAL)

///Num of RX BSM Blobs
#define NUM_RX_BSM_BLOBS  4

//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------

/// Identify each physical interface
typedef enum FHandleVal
{
  INTERFACE_GPS = 0,
  INTERFACE_CAN,
  INTERFACE_P1609_SYNC,
  INTERFACE_P1609_WSM,
  INTERFACE_P1609_WME,
  INTERFACE_UDP,

  INTERFACE_MAX_VAL
}
eFHandleVal;

//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------

/// Main signal handler
static void ShellSignal(
  int SigNum);

/// Update action per BSM
static void UpdateBsmInfo(
  tBlob1 *pBlob1);

/// Sync event actions
static void ShellSyncAction(
  tStateStore *pStore);

/// Rx WSM event actions
static void ShellWSMAction(
  tStateStore *pStore);

/// Rx WME event actions
static void ShellWMEAction(
  tStateStore *pStore);

/// Rx CAN message event actions
static void ShellCANAction(
  tStateStore *pStore);

/// Rx UDP stream event actions
static void ShellUDPAction(
  tStateStore *pStore);

/// VIC event actions
static void ShellVICAction(
  tStateStore *pStore);

/// Rx GPS update event actions
static void ShellGPSAction(
  tStateStore *pStore);

/// Check for Rx safety Events
static void ShellCheckRxEvents(uint16_t RxBsmCacheIndex);

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// Debug control
D_LEVEL_DECLARE();

/// File descriptors to monitor
static struct pollfd FDesc[INTERFACE_MAX_VAL];

/// Exit condition
static bool MainExit = false;

/// BSM counter
static uint32_t BSMTxCounter = 0;

/// Array of structures to store Blob1 & event details from received BSM
static tRxBsmCache RxBsmCache[NUM_RX_BSM_BLOBS];

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------

/**
 * @brief Application entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Zero if successful, otherwise a negative errno
 *
 * Main application
 * - Initialise data variables
 * - Parse user option command line variables
 * - Initialise interfaces
 * - Use poll() to wait for some input from interfaces
 * - Perform actions on each input action
 *   (@ref ShellSyncAction(), @ref ShellWSMAction(), @ref ShellWMEAction(),
 *    @ref ShellCANAction(), @ref ShellUDPAction(),
 *    @ref ShellGPSAction(), @ref ShellVICAction())
 * - De-initialise interfaces when Exit condition met
 *
 */
int main(int argc, char **argv)
{
  tStateStore Store;
  struct drand48_data rand_buffer;
  int RetVal = -ENOSYS;

  // Set initial debug level
  D_LEVEL_INIT();

  // Init data
  memset(&Store, 0, sizeof(Store));

  //Init RxBsmCache array of structures
  {
    int i;
    for (i = 0; i< NUM_RX_BSM_BLOBS; i++)
    {
      memset(&RxBsmCache[i].Blob1, 0, sizeof(tBlob1));
      RxBsmCache[i].EventFlag = 0;
      RxBsmCache[i].HardBrakingEventRx = false;
      RxBsmCache[i].ExtraInfo = NULL;
    }
  }


  d_printf(D_DEBUG, NULL, "Signal handlers\n");

  // setup the signal handlers to exit gracefully
  signal(SIGINT,  ShellSignal);
  signal(SIGTERM, ShellSignal);
  signal(SIGQUIT, ShellSignal);
  signal(SIGHUP,  ShellSignal);
  signal(SIGPIPE, ShellSignal);

  // Parse arguments
  int Res = BSMOpts_Parse(argc, argv, &Store.Config);

  if (Res < 0)
  {
    // Argument parsing error
    BSMOpts_Usage(argv[0]);
    return -EINVAL;
  }

  // Show active options
  BSMOpts_Print(&Store.Config);

  (void)srand48_r((rand() << 7) ^ rand() ^ (rand() % 37), &rand_buffer);

  // Set some Id
  (void)lrand48_r(&rand_buffer, (long int *)&Store.Blob1.Id);
  // Set initial Blob1 MsgCnt value
  (void)lrand48_r(&rand_buffer, (long int *)&Store.Blob1.MsgCnt);
  Store.Blob1.MsgCnt %= 128;


  d_printf(D_DEBUG, NULL, "Init interfaces\n");

  LOGInit(
    (Store.Config.DumpToStdout   ? LOG_DEST_STDOUT : 0) |
    LOG_DEST_FILE        |
    (Store.Config.LogUdp ?         LOG_DEST_UDP    : 0),
    Store.Config.LogFileName);

  // Init interfaces
  FDesc[INTERFACE_GPS].fd = GPSInit();
  if (FDesc[INTERFACE_GPS].fd < 0) goto Exit;

  FDesc[INTERFACE_CAN].fd = CANInit();
  if (FDesc[INTERFACE_CAN].fd < 0) goto Exit;

  FDesc[INTERFACE_UDP].fd = UDPInit(&Store.Config);
  if (FDesc[INTERFACE_UDP].fd < 0) goto Exit;
  Store.UDP.Status = UDP_STATUS_RUNNING;

  FDesc[INTERFACE_P1609_WME].fd = P1609WMEInit(&Store.Config);
  if (FDesc[INTERFACE_P1609_WME].fd < 0) goto Exit;

  FDesc[INTERFACE_P1609_WSM].fd = P1609WSMInit(&Store.Config);
  if (FDesc[INTERFACE_P1609_WSM].fd < 0) goto Exit;

  FDesc[INTERFACE_P1609_SYNC].fd = P1609SyncInit();
  if (FDesc[INTERFACE_P1609_SYNC].fd < 0) goto Exit;

  (void)Dot2_Init(&Store.Config);

  VICInit(&Store.VIC);
  Store.HardBrakingEventVIC = false;

  // Set CAN defaults (in case no real information available on CAN)
  CANDefaults(&Store.CAN);
  ShellCANAction(&Store);

  // setup the poll events
  FDesc[INTERFACE_GPS].events          = POLL_INPUT;
  FDesc[INTERFACE_CAN].events          = POLL_INPUT;
  FDesc[INTERFACE_UDP].events          = POLL_INPUT;
  FDesc[INTERFACE_P1609_WSM].events    = POLL_INPUT;
  FDesc[INTERFACE_P1609_WME].events    = POLL_INPUT;
  FDesc[INTERFACE_P1609_SYNC].events   = POLL_INPUT;

  // Main loop
  while (!MainExit)
  {
    // Wait for an event on the gpsd file descriptor
    // poll() returns >0 if descriptor is readable, 0 if timeout, -1 if error
    int Data = poll(FDesc, INTERFACE_MAX_VAL, 1000);
    d_printf(D_DEBUG, NULL, "Poll data %d\n", Data);

    // poll status check
    if (Data < 0)
    {
      // error
      d_error(D_WARN, NULL, "Poll error %d '%s'\n", errno, strerror(errno));
      RetVal = -errno;
      goto Error;
    }
    else if (Data == 0)
    {
      // timeout
      d_printf(D_DEBUG, NULL, "Poll timeout\n");
      continue;
    }

    // Identify FD unblocking

    // P1609Sync
    if (FDesc[INTERFACE_P1609_SYNC].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on P1609Sync (revents 0x%02x)\n",
              FDesc[INTERFACE_P1609_SYNC].revents);
    }

    if (FDesc[INTERFACE_P1609_SYNC].revents & POLL_INPUT)
    {
      int Res = P1609SyncUpdate(FDesc[INTERFACE_P1609_SYNC].fd, &Store.P1609Sync);

      if (Res >= 0)
      {
        // Channel is in successful return code
        if (Res == MK2CHAN_CCH)
        {
          ShellSyncAction(&Store);
        }
      }
      else
      {
        d_printf(D_DEBUG, NULL, "P1609Sync update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // P1609 WSM Rx
    if (FDesc[INTERFACE_P1609_WSM].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on P1609 (revents 0x%02x)\n",
              FDesc[INTERFACE_P1609_WSM].revents);
    }

    if (FDesc[INTERFACE_P1609_WSM].revents & POLL_INPUT)
    {
      int Res = P1609WSMUpdate(FDesc[INTERFACE_P1609_WSM].fd, &Store.pP1609WSM);

      if (Res >= 0)
      {
        ShellWSMAction(&Store);
      }
      else
      {
        d_printf(D_DEBUG, NULL, "P1609 WSM update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // P1609 WME Rx
    if (FDesc[INTERFACE_P1609_WME].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on P1609 WME(revents 0x%02x)\n",
              FDesc[INTERFACE_P1609_WME].revents);
    }

    if (FDesc[INTERFACE_P1609_WME].revents & POLL_INPUT)
    {
      int Res = P1609WMEUpdate();

      if (Res >= 0)
      {
        ShellWMEAction(&Store);
      }
      else
      {
        d_printf(D_DEBUG, NULL, "P1609 WME update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // CAN
    if (FDesc[INTERFACE_CAN].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on CAN (revents 0x%02x)\n",
              FDesc[INTERFACE_CAN].revents);
    }

    if (FDesc[INTERFACE_CAN].revents & POLL_INPUT)
    {
      int Res = CANUpdate(FDesc[INTERFACE_CAN].fd, &Store.CAN);

      if (Res >= 0)
      {
        ShellCANAction(&Store);
      }
      else
      {
        d_printf(D_DEBUG, NULL, "CAN update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // UDP
    if (FDesc[INTERFACE_UDP].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on UDP (revents 0x%02x)\n",
              FDesc[INTERFACE_UDP].revents);
    }

    if (FDesc[INTERFACE_UDP].revents & POLL_INPUT)
    {
      int Res = UDPUpdate(FDesc[INTERFACE_UDP].fd, &Store.UDP);

      if (Res >= 0)
      {
        ShellUDPAction(&Store);
      }
      else
      {
        d_printf(D_DEBUG, NULL, "UDP update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // GPS
    if (FDesc[INTERFACE_GPS].revents & POLL_ERROR)
    {
      d_error(D_ERR, NULL, "Poll error on GPS (revents 0x%02x)\n",
              FDesc[INTERFACE_GPS].revents);
    }

    if (FDesc[INTERFACE_GPS].revents & POLL_INPUT)
    {
      int Res = GPSUpdate(&Store.GPS);

      if (Res >= 0)
      {
        // Check if store updated, otherwise no need for actions
        if (Res == GPS_UPDATE_POSITION_AND_STORE)
        {
          ShellGPSAction(&Store);
        }
      }
      else
      {
        d_printf(D_DEBUG, NULL, "GPS update failed (%d)\n", Res);
        goto Exit;
      }
    }

    // The VIC is refreshed either on the timeout or whenever another device
    // is ready.
    switch (VICUpdate())
    {
      case -1: // Error
        // The device may come online again; considered non-fatal
        d_error(D_INFO, NULL, "Could not read VIC GPIO\n");
        break;
      case 0: // No state changes
        break;
      default: // 1 or more GPINs have changed
        ShellVICAction(&Store);
        break;
    }

  }

  RetVal = 0;

Error:
Exit:

  // Deinit interfaces
  if (FDesc[INTERFACE_GPS].fd >= 0)
    GPSDeinit();
  if (FDesc[INTERFACE_CAN].fd >= 0)
    CANDeinit(FDesc[INTERFACE_CAN].fd);
  if (FDesc[INTERFACE_UDP].fd >= 0)
    UDPDeinit(FDesc[INTERFACE_UDP].fd);
  if (FDesc[INTERFACE_P1609_WSM].fd >= 0)
    P1609WSMDeinit();
  if (FDesc[INTERFACE_P1609_WME].fd >= 0)
    P1609WMEDeinit();
  if (FDesc[INTERFACE_P1609_SYNC].fd >= 0)
    P1609SyncDeinit(FDesc[INTERFACE_P1609_SYNC].fd);
  VICDeinit();

  Dot2_Deinit();

  LOGDeinit();

  return RetVal;
}


/**
 * @brief Actions for P1609Sync event
 * @param pStore the Shell store data
 *
 * P1609 Sync events:
 *   - Send BSM with current store & events
 *   - Update BSM Blob1 counters
 *   - Count sent BSMs when limiting amount of BSMs to sent
 *
 */
static void ShellSyncAction(
  tStateStore *pStore)
{
  // Check count on 'next' SyncAction
  // to handle events in the already in the system
  if ((pStore->Config.TxCounter) &&
      (BSMTxCounter == pStore->Config.TxCounter))
  {
    /// If the UDP isn't running, then exit the app
    if (pStore->UDP.Status != UDP_STATUS_RUNNING)
    {
      MainExit = true;
    }

    d_printf(D_DEBUG, NULL, "BSM msg count reached (%d), exiting...\n",
             BSMTxCounter);
    goto Exit;
  }

  d_printf(D_DEBUG, NULL, "P1609 Sync Actions: encode BSM\n");

  //Check for Hard braking event
  if (pStore->HardBrakingEventVIC == true)
  {
    pStore->Events =  (pStore->Events | (long) J2735_BSM_EVENT_HardBraking);
  }

  // BSM trigger
  J2735_BSMSend(&pStore->Blob1,
                &pStore->Events);

  UpdateBsmInfo(&pStore->Blob1);

  // May limit BSMs to send
  BSMTxCounter++;

  // BSM Tx limit not checked here as application will exit rather abruptly

Exit:
  return;
}


/**
 * @brief Actions for P1609 WSM Rx event
 * @param pStore the Shell store data
 *
 * P1609 Rx events:
 *   - Check BSM received.
 *   - Decode received DSRC message if possible
 *
 */
static void ShellWSMAction(
  tStateStore *pStore)
{
  d_printf(D_DEBUG, NULL, "P1609 Rx Actions: decode received BSM\n");

  if (pStore->pP1609WSM)
  {
    struct Dot3WSMPHdr *pWSM = (struct Dot3WSMPHdr *)pStore->pP1609WSM;

    // Attempt to decode Rx message
    // If it is a signed Data, then dot2 header is stripped off
    // and offset points to start of BSM header
    int Offset = J2735_BSMReceive(pWSM);

    if ((Offset < 0) &&
        (ntohl(pWSM->PSID) != pStore->Config.WbssPsid))
    {
      d_printf(D_WARN, NULL,"BSMReceive returned with error or -ve offset %d\n", Offset);
    }

    // Forward the received WSM over UDP if:
    //  - configured to not log over UDP
    //    AND
    //  - The UDP peer address is configured
    if ((pStore->Config.LogUdp == false) &&
        (pStore->Config.UdpAddr.s_addr != 0x00000000))
    {
      int Len = ntohs(pWSM->Length);
      (void)UDPSend(Len, (const char *)(pWSM->Data));
    }

    // free buffer
    free(pStore->pP1609WSM);
    // clear once used
    pStore->pP1609WSM = NULL;
  }

  return;
}


/**
 * @brief Actions for P1609 WME Rx event
 * @param pStore the Shell store data
 *
 * Currently does nothing
 *
 */
static void ShellWMEAction(
  tStateStore *pStore)
{
  d_printf(D_DEBUG, NULL, "P1609 WME Action\n");
  ; // None yet
  return;
}



/**
 * @brief Actions for CAN event
 * @param pStore the Shell store data
 *
 * CAN events:
 *   - Extract vehicle data and populate the appropriate BSM Blob1
 *     fields and part II fields.
 *
 */
static void ShellCANAction(
  tStateStore *pStore)
{
  tCANInfo *pCan = &pStore->CAN;

  d_printf(D_DEBUG, NULL, "CAN Actions: update vehicle status\n");

  // Speed - pack Blob1 TransmissionAndSpeed field
  pStore->Blob1.Speed = htons((pCan->Speed & 0x1FFF) |
                              ((pCan->TransmissionState & 0x7) << 13));

  // Angle
  pStore->Blob1.Angle = pCan->SteeringWheelAngle;

  // AccelLong
  pStore->Blob1.AccelLong = htons(pCan->AccelLong);

  // AccelLat
  pStore->Blob1.AccelLatSet = htons(pCan->AccelLat);

  // AccelVert
  pStore->Blob1.AccelVert = (int8_t)pCan->AccelVert;

  // AccelYaw
  pStore->Blob1.AccelYaw = htons(pCan->AccelYaw);

  // Brakes
  pStore->Blob1.Brakes = htons(pCan->SAEBrakeSystemStatus);

  // Size - pack Blob1 size field
  // Width/Length field is 24 bits, 10 bits for width and 14 bits for length
  pStore->Blob1.VehicleWidth_MSB =
    (pCan->VehicleWidth >> 2) & 0xFF;
  pStore->Blob1.VehicleLength_WidthLSB =
    htons(((pCan->VehicleLength) & 0x3FFF) |
          ((pCan->VehicleWidth & 0x03) << 14));

  pStore->Events = pCan->EventFlags;
  return;
}


/**
 * @brief Actions for UDP event
 * @param pStore the Shell store data
 *
 * UDP events:
 *   - Do nothing (just placeholder).
 *
 */
static void ShellUDPAction(
  tStateStore *pStore)
{
  struct Dot3WSMPHdr *pWSM;
  uint16_t Len;

  // Pack the UDP data into a WSM and send it over-the-air

  // pWSMTxBuf is the exclusion value, so WSMTxLen is read last to avoid a race
  // This would be important if this was a multi-threaded app... which it isn't
  pWSM = (struct Dot3WSMPHdr *)(pStore->UDP.pWSMTxBuf);
  if (pWSM == NULL)
    goto Error;
  Len = pStore->UDP.WSMTxLen;

  // Send the WSM, free the buffer if neccessary and clear the pointer
  (void)WSMSend(pWSM, Len);
  pStore->UDP.pWSMTxBuf = NULL;

Error:
  return;
}


/**
 * @brief Actions for VIC event
 * @param pStore the Shell store data
 *
 * This function is invoked when 1 or more GPIN states have altered.
 * Currently it checks only the state of gpin0.
 */
static void ShellVICAction(
  tStateStore *pStore)
{
  uint16_t Brakes_status;

  tVicInfo *pVic = &pStore->VIC;
  d_printf(D_DEBUG, NULL, "VIC Actions: update vehicle status\n");

  Brakes_status = pVic->gpin0State;
  if (Brakes_status == GPIN_HIGH)
  {
    d_printf(D_WARN, NULL,"Brakes Applied\n");
    pStore->HardBrakingEventVIC = true;
  }
  else if (Brakes_status == GPIN_LOW)
  {
    d_printf(D_WARN, NULL,"Brakes Released\n");
    pStore->HardBrakingEventVIC = false;
    pStore->Events =  (pStore->Events & ((long) J2735_BSM_EVENT_HardBraking^0xFFFF));
  }
  else
  {
    //Should never come to this stae
    d_error(D_ERR, NULL,"Unable to read GPIN0 status\n");
  }
}


/**
 * @brief Actions for GPS event
 * @param pStore the Shell store data
 *
 * GPS events:
 *   - Update Blob1 data values from GPS. Perform conversions from GPS
 *     data format to J2735 format (and apply appropriate limits or
 *     unavailable values.) Update BSM Blob1 fields.
 *
 */
static void ShellGPSAction(
  tStateStore *pStore)
{
  struct gps_fix_t *pFix = &pStore->GPS.Fix;

  d_printf(D_DEBUG, NULL, "GPS Actions: update position\n");

  uint16_t TmpSecMark;
  int32_t  TmpLat;
  int32_t  TmpLong;
  int16_t  TmpElev;
  uint16_t TmpHeading;

  if (!isnan(pFix->time))
  {
    // SecMark
    struct timeval Time;
    // Convert GPS time to constituent parts
    Time.tv_sec = pFix->time;
    Time.tv_usec = (1e6*(pFix->time - (uint32_t)pFix->time))+1;
    // Calculate milliseconds since minute start
    TmpSecMark = ((Time.tv_sec % 60) * 1000) + (Time.tv_usec / 1000);
  }
  else
  {
    TmpSecMark = UNAVAIL_DE_TIME;
  }

  // Latitude
  if (!isnan(pFix->latitude))
  {
    if (pFix->latitude >= (double)90.0)
      TmpLat = MAX_DE_LAT;
    else if (pFix->latitude <= (double)-90.0)
      TmpLat = MIN_DE_LAT;
    else
      TmpLat = CONV_DE_CAN_LAT*pFix->latitude;
  }
  else
  {
    TmpLat = UNAVAIL_DE_LAT;
  }

  // Longitude
  if (!isnan(pFix->longitude))
  {
    if (pFix->longitude >= (double)180.0)
      TmpLong = MAX_DE_LON;
    else if (pFix->longitude <= (double)-180.0)
      TmpLong = MIN_DE_LON;
    else
      TmpLong = CONV_DE_CAN_LON*pFix->longitude;
  }
  else
  {
    TmpLong = UNAVAIL_DE_LON;
  }

  // Elevation
  if (!isnan(pFix->altitude))
  {
    if (pFix->altitude > (double)MAX_CAN_ELEV)
      TmpElev = MAX_DE_ELEV;
    else if (pFix->altitude < (double)MIN_CAN_ELEV)
      TmpElev = MIN_DE_ELEV;
    else
      TmpElev = CONV_DE_CAN_ELEV*pFix->altitude;
  }
  else
  {
    TmpElev = UNAVAIL_DE_ELEV;
  }

  // Heading
  if (!isnan(pFix->track))
  {
    if (pFix->track == (double)360.0)
      TmpHeading = UNAVAIL_DE_HEAD;
    else
      TmpHeading = CONV_DE_CAN_HEAD*pFix->track;
  }
  else
  {
    TmpHeading = UNAVAIL_DE_HEAD;
  }

  // Update Blob1 location fields
  pStore->Blob1.SecMark  = htons(TmpSecMark);
  pStore->Blob1.Lat      = htonl(TmpLat);
  pStore->Blob1.Long     = htonl(TmpLong);
  pStore->Blob1.Elev     = htons(TmpElev);
  pStore->Blob1.Accuracy = htonl(UNAVAIL_DE_POSACC);
  pStore->Blob1.Heading  = htons(TmpHeading);

  return;
}


/**
 * @brief Signal Handler
 * @param SigNum the signal caught
 *
 * Set the exit flag so that application can exit following next
 * main loop iteration.
 *
 */
static void ShellSignal(
  int SigNum)
{
  fprintf(stderr, "Signal %d!\n", SigNum);

  MainExit = true;
}


/**
 * @brief Update BSM data from interface stores
 * @param pBlob1 pointer to Blob1 parameters
 *
 * Update per-BSM system information. Increment Message Count field
 * for each sent BSM.
 *
 */
static void UpdateBsmInfo(
  tBlob1 *pBlob1)
{
  // Update MsgCnt
  pBlob1->MsgCnt++;

  if (pBlob1->MsgCnt > 127)
    pBlob1->MsgCnt = 0;
}

/**
 * @brief Checks for events as received from the BSM
 * @param RxBsmCacheIndex Index of Rx BSM cache in which event has occured
 *
 * P1609 Rx events:
 *   - Check Rx events
 *   - Currently for Demo purpose, checks if remote car has applied brakes, brakes are still ON or released
 *
 */
static void ShellCheckRxEvents(uint16_t RxBsmCacheIndex)
{

  if (RxBsmCache[RxBsmCacheIndex].EventFlag & (long) J2735_BSM_EVENT_HardBraking)
  {
    if (RxBsmCache[RxBsmCacheIndex].HardBrakingEventRx == false)
    {
      //Brake has just been applied
      RxBsmCache[RxBsmCacheIndex].HardBrakingEventRx = true;
      d_printf(D_WARN, NULL, "Car #%x Braked\n", RxBsmCache[RxBsmCacheIndex].Blob1.Id);
      int Unused = system("aplay shotgun.wav");
      (void)Unused;
    }
    else
    {
      //Brake continues to be applied
      d_printf(D_WARN, NULL,"Car #%x Brakes ON\n", RxBsmCache[RxBsmCacheIndex].Blob1.Id);
    }
  }
  else
  {
    if (RxBsmCache[RxBsmCacheIndex].HardBrakingEventRx == true)
    {
      //Brakes just released
      RxBsmCache[RxBsmCacheIndex].HardBrakingEventRx = false;
      d_printf(D_WARN, NULL,"Car #%x Released Brakes\n", RxBsmCache[RxBsmCacheIndex].Blob1.Id);
      int Unused = system("aplay laser.wav");
      (void)Unused;
    }
  }
}


/**
 * @brief Get Blob1 &  Event details from received BSM
 * @param None
 *
 * Store the Blob1 & event details in a local structure
 */
void  ShellGetBsmEvents(DSRC_BasicSafetyMessage_t *pBsm)
{
  uint32_t TempID;
  uint8_t MsgCnt;
  uint8_t *pBlob1;
  uint16_t i;

  pBlob1 = pBsm->blob1.buf;
  MsgCnt = pBlob1[0];
  (void)MsgCnt; // MsgCnt is not currently used
  TempID = (pBlob1[4] << 24) | (pBlob1[3] << 16) | (pBlob1[2] << 8) | (pBlob1[1]);

  //Match TempID with those from the array of RxBsmCache structure
  //and store  in the structure where there is a match, or if storing for the first time
  for (i=0; i<NUM_RX_BSM_BLOBS; i++)
  {
    uint32_t StoredID = RxBsmCache[i].Blob1.Id;
    if ((StoredID == TempID) || (StoredID == 0))
    {
      if ((pBsm->safetyExt != NULL) && (pBsm->safetyExt->events != NULL))
      {
        RxBsmCache[i].EventFlag = *(pBsm->safetyExt->events);
        //Check what event occured
        ShellCheckRxEvents(i);
      }
      else
      {
        RxBsmCache[i].EventFlag = 0;
      }
      RxBsmCache[i].MsgID = pBsm->msgID;
      memcpy(&RxBsmCache[i].Blob1, pBlob1, sizeof(tBlob1));
      break;
    }
  }
}

// Close the Doxygen group
/**
 * @}
 */
