/**
 * @addtogroup bsm_shell_if_p1609 BSM Shell P1609 interface
 * @{
 *
 * @file
 *
 * Module code for P1609 interface
 *
 * The usual top-level P1609 interface control function @ref P1609WSMInit(),
 *  @ref P1609WMEInit(), @ref P1609WSMDeinit(), @ref P1609WMEDeinit(),
 *  @ref P1609WSMUpdate() and @ref P1609WMEUpdate() handle initialisation
 *  and updating the store when requested.
 *
 * In addition, the internal functions do the following:
 *  - @ref P1609CtrlWME_Dot3Setup() : Initialise the P1609.3 WME
 *  - @ref P1609CtrlMLME_Dot4Setup() : Initialise the P1609.3 MLME
 *  - @ref P1609CtrlWME_StartServiceReq() : Start a WME service (in our case: User service, unconditional)
 *  - @ref P1609CtrlWME_StopServiceReq() : Stop a registered WME service
 *  - @ref P1609CtrlWSMP_OpenInterface() : Open an WSMP protocol socket
 *  - @ref P1609CtrlWSMP_CloseInterface() : Closes the open WSMP protocol socket
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
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>

#include "dot3-wsmp.h"

#include "debug-levels.h"

#include "if-p1609.h"


//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// Supported channels (CCH, SCH & Safety)
#define P1609_SYSTEM_SUPPORTED_CHANNEL_COUNT  3

/// Max WSM length
#define P1609_SYSTEM_WSM_LENGTH_MAX   2048

/// WLAN device stem name
#define P1609_WLAN_STEM_NAME          "wlanmac"
/// WLAN device 1 choice
#define P1609_WLAN_1_DEV              "1"
/// WLAN device stem name
#define P1609_WLAN_2_DEV              "2"
/// WLAN device stem name
#define P1609_WLAN_MAC_ADDR_FILE      "mac.txt"
/// WLAN MAC address length in uint8_t
#define P1609_WLAN_MAC_ADDR_LEN       6
/// WLAN MAC address query command size
#define P1609_WLAN_MAC_ADDR_QUERY_LEN 100


//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------

/// Get WLAN MAC addresses from fw_printenv
static int GetWLANMAC(
  char *pDev,
  uint8_t MAC[P1609_WLAN_MAC_ADDR_LEN]);

/// Set default config
static int P1609CtrlInfo_DefaultCfgState(
  tConfigIniData *pConfig);

/// Restore Dot4MLMEMIB
static void P1609CtrlMLME_Dot4Restore (
  struct P1609State *pP1609State);

/// Setup the User's IPv6 parameters
void P1609CtrlWME_DoUserIPAddress (struct Dot3WMENotifInd *pInd);


/// Handle a WSA signing request
static int P1609CtrlWME_WMEWSASignReq (struct Dot3WMEData *pReq,
                                       void *pPriv);

/// Handle a WSA verification request
static int P1609CtrlWME_WMEWSAVerifyReq (struct Dot3WMEWSAVerifyReq *pReq,
    void *pPriv);

/// Handle a WME RCPI indication
static int P1609CtrlWME_WMERCPIInd (struct Dot3WMEMeasurementReportSet *pInd,
                                    void *pPriv);

/// Prints the passed WME notification indication packet
void P1609CtrlWME_PrintWMENotifIndicationPacket (struct Dot3WMENotifInd *pInd);


/// Handle a WME service notification
static int P1609CtrlWME_WMENotificationInd (struct Dot3WMENotifInd *pInd,
    void *pPriv);

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// P1609 State store
static tP1609State P1609State;

/// P1609 State access pointer
static tP1609State *pP1609State = &P1609State;

/// Original MIB before any modifications
struct Dot4MLMEMIB OriginalMIB;


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


/**
 * @brief P1609 WME Init
 * @param pConfig pointer to user configuration
 * @return File descriptor to monitor for P1609 WME Rx
 *
 * The P1609 WME initialisation requires configuring the dot3 and dot4
 * interfaces and associated MIB's.
 * Open the lib1609 (control) handle and reset the Dot3 MIB
 * The different channel modes require
 * different WME service requests.
 */
int P1609WMEInit (tConfigIniData *pConfig)
{
  d_fnstart(D_INTERN, NULL, "(pConfig %p)\n", pConfig);

  int Res = P1609CtrlInfo_DefaultCfgState(pConfig);
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "Setting defaults failed: %d\n", Res);
    return Res;
  }

  // configure dot3 socket
  // Setup the Dot3
  Res = P1609CtrlWME_Dot3Setup(pP1609State);
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "WME_Dot3Setup failed (%d)\n", Res);
    return Res;
  }

  // Setup the Dot4
  Res = P1609CtrlMLME_Dot4Setup(pP1609State);
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "MLME_Dot4Setup failed (%d)\n", Res);
    return Res;
  }

  d_printf(D_INFO, NULL, "Interface open, setting mode: %d\n",
           pConfig->ChannelMode);

  // Try to clear existing service requests - this helps in the situation
  // where application dies before deleting an added service request, by
  // preventing multiple duplicate service requests filling the WME
  {
    // silence prints since we don't care or want to know if this fails
    // (let ((D_LEVEL 0)) ...)
    int d_level = D_LEVEL;
    D_LEVEL = 0;
    Res = P1609CtrlWME_StopServiceReq(pP1609State);
    D_LEVEL = d_level;
  }

  // Configure the 1609 stack
  Res = P1609CtrlWME_StartServiceReq(pP1609State);
  if (Res != 0)
  {
    d_printf(D_WARN, NULL, "Start service req reported %d\n", Res);
  }

  // Return file descriptor
  d_fnend(D_INTERN, NULL, "() = %d\n", pP1609State->WME.Fd);
  return pP1609State->WME.Fd;

}

/**
 * @brief P1609 WSM Init
 * @param pConfig pointer to user configuration
 * @return File descriptor to monitor for P1609 Rx
 *
 * The P1609 WSM initialisation initialises channel configuration
 * and opens up WSM Interface
 */
int P1609WSMInit (tConfigIniData *pConfig)
{
  d_fnstart(D_INTERN, NULL, "(pConfig %p)\n", pConfig);


  // open a  WSMP socket
  int Res = P1609CtrlWSMP_OpenInterface(pP1609State);
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "Opening WSMP interface failed: %d\n", Res);
    return Res;
  }

  d_printf(D_INFO, NULL, "Interface open, setting mode: %d\n",
           pConfig->ChannelMode);

  // Return file descriptor
  d_fnend(D_INTERN, NULL, "() = %d\n", pP1609State->WSMP.Fd);
  return pP1609State->WSMP.Fd;
}

/**
 * @brief P1609 WSM Deinit
 *
 * Clean up P1609 WSM interface
 *
 */
void P1609WSMDeinit(
  void)
{

  d_fnstart(D_INTERN, NULL, "()\n");

  // close the opened interface(s)
  P1609CtrlWSMP_CloseInterface(pP1609State);

  d_fnend(D_INTERN, NULL, "()\n");
}

/**
 * @brief P1609 WME Deinit
 *
 * Clean up P1609 WME interface
 *
 */
void P1609WMEDeinit(
  void)
{
  int Res;

  d_fnstart(D_INTERN, NULL, "()\n");

  // Stop the service
  Res = P1609CtrlWME_StopServiceReq(pP1609State);
  if (Res != 0)
  {
    d_error(D_ERR, NULL, "Failed to stop the Service Request!\n");
  }

  // Release the MLME configuration data
  if (pP1609State->pDot4 != NULL)
  {
    P1609CtrlMLME_Dot4Restore(pP1609State);
    Dot4_Close(pP1609State->pDot4);
    pP1609State->pDot4 = NULL;
  }

  // Release the WSM configuration data
  if (pP1609State->pDot3 != NULL)
  {
    Dot3_Close(pP1609State->pDot3);
    pP1609State->pDot3 = NULL;
  }

  d_fnend(D_INTERN, NULL, "()\n");
}


/**
 * @brief Update provided store with P1609 WSM information
 * @param P1609Fd File descriptor to read
 * @param pRxBuf pointer to storage for received WSM
 * @return Status, 0 for succes, otherwise negative
 *
 * Store received P1609 packet
 *
 */
int P1609WSMUpdate(
  int P1609Fd,
  char **pRxBuf)
{
  int RetVal = 0;
  int Flags = 0;
  struct Dot3WSMPSockAddr SockAddr;
  socklen_t AddrLen = sizeof(SockAddr);

  d_fnstart(D_INTERN, NULL, "(P1609Fd %d, pRxBuf %p)\n",
            P1609Fd, pRxBuf);

  // Get space for received block
  int Len = P1609_RX_BUF_SIZE;
  char *pBuf = (char *)calloc(1,Len);

  int Res = recvfrom(P1609Fd, pBuf, Len, Flags,
                     (struct sockaddr *)&SockAddr, &AddrLen);

  if (Res < 0)
  {
    d_error(D_DEBUG, NULL, "recvfrom() failed %d '%s'\n",
            errno, strerror(errno));
    RetVal = -errno;
    free(pBuf);
    goto Error;
  }
  else if (Res == 0)
  {
    d_printf(D_WARN, NULL, "recvfrom() got no bytes %d '%s'\n",
             errno, strerror(errno));
    free(pBuf);
    RetVal = -2;
    goto Error;
  }
  // else received data OK, Res is amount of data
  d_printf(D_DEBUG, NULL, "recvfrom(): %d (%d)\n",
           Res, AddrLen);


  // Update supplied pointer
  *pRxBuf = pBuf;

Error:
  d_fnend(D_DEBUG, NULL, "() = %d\n", RetVal);

  return RetVal;
}


//------------------------------------------------------------------------------
// P1609_CTRL WSMP control interface
//

/**
 * @brief Open WSMP socket interfaces
 * @param pP1609State Pointer to the top-level control structure
 * @return zero for success, otherwise a negative errno
 *
 * - Open the WSMP (data) socket
 * - Bind the WSMP socket to the requested PSID
 */

int P1609CtrlWSMP_OpenInterface(
  tP1609State *pP1609State)
{
  int Res = -ENOSYS;
  int Fd;
  struct Dot3WSMPSockAddr SockAddr;
  int protocol;

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  protocol = PROTO_IEEE1609DOT3_WSMP;

  // Open the WSMP socket

  Fd = socket(AF_IEEE1609_WSMP, SOCK_DGRAM, protocol);
  if (Fd < 0)
  {
    Res = -errno;
    d_error(D_ERR, NULL, "socket() failed %d '%s'\n", errno, strerror(errno));
    goto Error;
  }

  pP1609State->WSMP.Fd = Fd;

  // Bind the WSMP socket to a particular PSID
  SockAddr.Family = AF_IEEE1609_WSMP;
  memset(SockAddr.Hdr.Tx.DA, 0, sizeof(SockAddr.Hdr.Tx.DA));
  SockAddr.Hdr.Version       = DOT3_WSMP_VERSION_3;
  SockAddr.Hdr.Tx.Priority   = pP1609State->Cfg.Priority;
  SockAddr.Hdr.Tx.ExpiryTime = 0;
  SockAddr.Hdr.ChannelNumber = pP1609State->Cfg.ChanNbr;
  SockAddr.Hdr.DataRate      = pP1609State->Cfg.DataRate;
  SockAddr.Hdr.TxPower       = pP1609State->Cfg.TxPwr;
  SockAddr.Hdr.PSID          = htonl(DOT3_WSMP_PSID_ALL); // Promiscuous receive
  SockAddr.Hdr.HdrExtFlags   = 0x07; // Channel | DataRate | TxPwr

  Res = bind(Fd, (struct sockaddr *)&SockAddr, sizeof(SockAddr));
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "bind() failed %d '%s'\n", errno, strerror(errno));
    Res = -errno;
    goto Error;
  }

  if (Res < 0)
    goto Error;

  goto Success;

Error:
  // Close the WSMP socket
  close(Fd);
Success:
  d_fnend(D_INTERN, NULL, "(pP1609State %p) Fd %d\n", pP1609State, Fd);

  return Res;
} // P1609CtrlWSMP_OpenInterface


/**
 * @brief Close all the application's interfaces & WSMP socket interfaces
 * @param pP1609State Pointer to the top-level control structure
 *
 * - Close the lib1609 (control) handle
 * - Close the WSMP (data) socket
 */

void P1609CtrlWSMP_CloseInterface(
  tP1609State *pP1609State)
{

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  d_printf(D_DEBUG, NULL, "Closing the WSMP socket\n");

  if (pP1609State->WSMP.Fd >= 0)
    close(pP1609State->WSMP.Fd);

  pP1609State->WSMP.Fd = -1;

  d_fnend(D_INTERN, NULL, "(pP1609State %p) = void\n", pP1609State);

} // P1609CtrlWSMP_CloseInterface


//------------------------------------------------------------------------------
// P1609_CTRL WME control interface
//

/**
 * @brief Setup the WME configuration data
 * @param pP1609State Pointer to the top-level control structure
 * @return zero for success, otherwise a negative errno
 *
 */
int P1609CtrlWME_Dot3Setup (
  tP1609State *pP1609State)
{
  int Res = -ENOSYS;
  struct Dot3WMEMIB *pMIB = &(pP1609State->WME.WMEMIB);
  uint8_t MACAddr[P1609_WLAN_MAC_ADDR_LEN];

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  pP1609State->pDot3 = (struct Dot3 *) Dot3_Open("dot3");
  if (pP1609State->pDot3 == NULL)
  {
    Res = -ENONET;
    goto Error;
  }

  // Get the MIB and apply the changes
  Res = Dot3_WMEGet(pP1609State->pDot3, pMIB);
  if (Res != 0)
    goto Error;

  // Initialize the System MIB
  pMIB->SystemMIB.LocalInfo.NumberOfChannelsSupported =
    P1609_SYSTEM_SUPPORTED_CHANNEL_COUNT;
  pMIB->SystemMIB.LocalInfo.WSMMaxLength              =
    P1609_SYSTEM_WSM_LENGTH_MAX;


  // Initialize the WME MIB
  pMIB->ApplicationMIB.ChannelInfo[0].CitIndex         = 0;
  pMIB->ApplicationMIB.ChannelInfo[0].CitChannelNumber =
    pP1609State->Cfg.ChanCCHA;
  pMIB->ApplicationMIB.ChannelInfo[0].CitAdaptable     = NOT_ADAPTABLE;
  pMIB->ApplicationMIB.ChannelInfo[0].CitDataRate      =
    pP1609State->Cfg.DataRate;
  pMIB->ApplicationMIB.ChannelInfo[0].CitTxPower       =
    pP1609State->Cfg.TxPwr;
  pMIB->ApplicationMIB.ChannelInfo[0].CitChannelAccess = DOT3_WME_ALTERNATING_SLOT_1_ONLY;

  pMIB->ApplicationMIB.ChannelInfo[1].CitIndex         = 1;
  pMIB->ApplicationMIB.ChannelInfo[1].CitChannelNumber =
    pP1609State->Cfg.ChanSCHA;
  pMIB->ApplicationMIB.ChannelInfo[1].CitAdaptable     = NOT_ADAPTABLE;
  pMIB->ApplicationMIB.ChannelInfo[1].CitDataRate      =
    pP1609State->Cfg.DataRate;
  pMIB->ApplicationMIB.ChannelInfo[1].CitTxPower       =
    pP1609State->Cfg.TxPwr;
  pMIB->ApplicationMIB.ChannelInfo[1].CitChannelAccess = DOT3_WME_ALTERNATING_SLOT_1_ONLY;

  pMIB->ApplicationMIB.ChannelInfo[2].CitIndex         = 2;
  pMIB->ApplicationMIB.ChannelInfo[2].CitChannelNumber =
    pP1609State->Cfg.ChanSCHB;
  pMIB->ApplicationMIB.ChannelInfo[2].CitAdaptable     = NOT_ADAPTABLE;
  pMIB->ApplicationMIB.ChannelInfo[2].CitDataRate      =
    pP1609State->Cfg.DataRate;
  pMIB->ApplicationMIB.ChannelInfo[2].CitTxPower       =
    pP1609State->Cfg.TxPwr;
  pMIB->ApplicationMIB.ChannelInfo[2].CitChannelAccess = DOT3_WME_CONTINUOUS_SLOT_0_AND_1;

  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_FORCED:
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      break;

    case BSM_OPTS_CHANNEL_MODE_DUAL:
      pMIB->ApplicationMIB.ChannelInfo[0].CitChannelAccess = DOT3_WME_CONTINUOUS_SLOT_0_AND_1;
      pMIB->ApplicationMIB.ChannelInfo[1].CitChannelNumber = 0; // SCHA off
      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    default:
      d_printf(D_INFO, NULL, "No SCH!\n");
      pMIB->ApplicationMIB.ChannelInfo[0].CitChannelAccess = DOT3_WME_CONTINUOUS_SLOT_0_AND_1;
      pMIB->ApplicationMIB.ChannelInfo[1].CitChannelNumber = 0; // SCHA off
      pMIB->ApplicationMIB.ChannelInfo[2].CitChannelNumber = 0; // SCHB off
      break;
  }

  d_printf(D_INFO, NULL, "WME MIB [%d, %d, %d]\n",
           pMIB->ApplicationMIB.ChannelInfo[0].CitChannelNumber,
           pMIB->ApplicationMIB.ChannelInfo[1].CitChannelNumber,
           pMIB->ApplicationMIB.ChannelInfo[2].CitChannelNumber);

  // Set both MAC addresses
  pMIB->SystemMIB.AddressInfo[0].AiIndex = 0;
  pMIB->SystemMIB.AddressInfo[1].AiIndex = 1;

  // Get WLAN MAC addresses from 'fw_printenv'
  if (GetWLANMAC(P1609_WLAN_1_DEV, MACAddr) == 0)
    memcpy(pMIB->SystemMIB.AddressInfo[0].MACAddress,
           MACAddr,
           P1609_WLAN_MAC_ADDR_LEN);
  else
    d_error(D_WARN, NULL, "Unable to get wlanmac%s MAC address\n",
            P1609_WLAN_1_DEV);

  if (GetWLANMAC(P1609_WLAN_2_DEV, MACAddr) == 0)
    memcpy(pMIB->SystemMIB.AddressInfo[1].MACAddress,
           MACAddr,
           P1609_WLAN_MAC_ADDR_LEN);
  else
    d_error(D_WARN, NULL, "Unable to get wlanmac%s MAC address\n",
            P1609_WLAN_2_DEV);

  d_printf(D_DEBUG, NULL, "WLAN MAC address 1 is "
           "%02X:%02X:%02X:%02X:%02X:%02X\n",
           pMIB->SystemMIB.AddressInfo[0].MACAddress[0],
           pMIB->SystemMIB.AddressInfo[0].MACAddress[1],
           pMIB->SystemMIB.AddressInfo[0].MACAddress[2],
           pMIB->SystemMIB.AddressInfo[0].MACAddress[3],
           pMIB->SystemMIB.AddressInfo[0].MACAddress[4],
           pMIB->SystemMIB.AddressInfo[0].MACAddress[5]);
  d_printf(D_DEBUG, NULL, "WLAN MAC address 2 is "
           "%02X:%02X:%02X:%02X:%02X:%02X\n",
           pMIB->SystemMIB.AddressInfo[1].MACAddress[0],
           pMIB->SystemMIB.AddressInfo[1].MACAddress[1],
           pMIB->SystemMIB.AddressInfo[1].MACAddress[2],
           pMIB->SystemMIB.AddressInfo[1].MACAddress[3],
           pMIB->SystemMIB.AddressInfo[1].MACAddress[4],
           pMIB->SystemMIB.AddressInfo[1].MACAddress[5]);

  // Setting of the priority/QoS configurations is done in the MLME MIB

  Res = Dot3_WMESet(pP1609State->pDot3, pMIB);
  if (Res != 0)
    goto Error;

  // Get the WME interface file descriptor
  int Fd =  Dot3_MsgFd(pP1609State->pDot3);
  if (Fd < 0)
  {
    Res = Fd;
    d_error(D_ERR, NULL, "Dot3_MsgFd () failed %d '%s'\n", Fd, strerror(Fd));
    goto Error;
  }
  pP1609State->WME.Fd = Fd;

  Res = 0;

Error:
  d_fnend(D_INTERN, NULL, "(Res %d)\n", Res);

  return Res;
} // P1609CtrlWME_Dot3Setup


/**
 * @brief Setup/Connect according to the chosen Service Request Type
 * @param pP1609State Pointer to the top-level control structure
 * @return zero for success, otherwise a negative errno
 *
 * Create any required service requests.
 *
 */
int P1609CtrlWME_StartServiceReq(
  tP1609State *pP1609State)
{
  int Res = -ENOSYS;
  struct Dot3WMEUserServiceReq UsrSvcReq = { 0, };
  struct Dot3WMEUserServiceReq *pReq = &(UsrSvcReq);
  struct Dot3WMEMIB *pMIB = &(pP1609State->WME.WMEMIB);

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  if (pP1609State->pDot3 == NULL)
  {
    Res = -ENONET;
    goto Error;
  }

  // Get the MIB (for later reference)
  Res = Dot3_WMEGet(pP1609State->pDot3, pMIB);
  if (Res != 0)
    goto Error;

  // Just in case: Delete the PSID
  {
    pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_DEL;
    pReq->ProviderServiceIdentifier = pP1609State->Cfg.WsmPsid;
    pReq->Valid = DOT3_WME_USRCON_PSID;
    {
      // silence prints since we don't care or want to know if this fails
      // (let ((D_LEVEL 0)) ...)
      int d_level = D_LEVEL;
      D_LEVEL = 0;
      Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      D_LEVEL = d_level;
    }
  }


  // Radio B
  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    default:
      d_printf(D_INFO, NULL, "SCHB off\n");
      break;

    case BSM_OPTS_CHANNEL_MODE_DUAL:
    case BSM_OPTS_CHANNEL_MODE_FORCED:
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      d_printf(D_INFO, NULL, "SCHB on\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_ADD;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_DEDICATED;
      pReq->LclServiceIndex = DOT3_UST_ENTRIES - 1;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WsmPsid;
      pReq->ServicePriority = 31;
      pReq->SchNumber = pP1609State->Cfg.ChanSCHB;
      memcpy(pReq->SourceMACAddress,
             pMIB->SystemMIB.AddressInfo[1].MACAddress,
             sizeof(pReq->SourceMACAddress));
      pReq->Valid = DOT3_WME_USRCON_LSI  |
                    DOT3_WME_USRCON_PSID |
                    DOT3_WME_USRCON_SPRI |
                    DOT3_WME_USRCON_SCHN |
                    DOT3_WME_USRCON_SMAC;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;
  }

  // Radio A
  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_FORCED:
      d_printf(D_INFO, NULL, "SCHA forced\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_ADD;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_UNCOND; // forced == unconditional
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      pReq->ServicePriority = 31;
      pReq->SchNumber = pP1609State->Cfg.ChanSCHA;
      memcpy(pReq->SourceMACAddress,
             pMIB->SystemMIB.AddressInfo[1].MACAddress,
             sizeof(pReq->SourceMACAddress));
      pReq->Valid = DOT3_WME_USRCON_PSID |
                    DOT3_WME_USRCON_SPRI |
                    DOT3_WME_USRCON_SCHN |
                    DOT3_WME_USRCON_SMAC;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;

    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      d_printf(D_INFO, NULL, "SCHA conditional\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_ADD;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_SVCMATCH; // conditional == match
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      pReq->ServicePriority = 31;
      pReq->SchNumber = pP1609State->Cfg.ChanSCHA;
      memcpy(pReq->SourceMACAddress,
             pMIB->SystemMIB.AddressInfo[1].MACAddress,
             sizeof(pReq->SourceMACAddress));
      pReq->Valid = DOT3_WME_USRCON_PSID |
                    DOT3_WME_USRCON_SPRI |
                    DOT3_WME_USRCON_SCHN |
                    DOT3_WME_USRCON_SMAC;
      if (pReq->ProviderServiceIdentifier != BSM_OPTS_WBSS_PSID_OFF)
      {
        //Start WBSS user service
        Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
        if (Res != 0)
        {
          Dot3_Close(pP1609State->pDot3);
          goto Error;
        }
      }

      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    case BSM_OPTS_CHANNEL_MODE_DUAL:
    default:
      d_printf(D_INFO, NULL, "SCHA off\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_ADD;
      pReq->UsrReqType = DOT3_WME_SCHREQ_NO_ACCESS;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      memcpy(pReq->SourceMACAddress,
             pMIB->SystemMIB.AddressInfo[1].MACAddress,
             sizeof(pReq->SourceMACAddress));
      pReq->Valid = DOT3_WME_USRCON_PSID |
                    DOT3_WME_USRCON_SMAC;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      if (Res != 0)
      {
        d_printf(D_DEBUG, NULL, "Dot3_WMEUserServiceReq() = %d '%s'\n",
                 errno, strerror(errno));
      }
      Res = 0;
      break;
  }

  //Set Dot3 Call backs
  {
    Dot3_SetCallbackWMEWSASignReq(pP1609State->pDot3,
                                  P1609CtrlWME_WMEWSASignReq, pP1609State);
    Dot3_SetCallbackWMEWSAVerifyReq(pP1609State->pDot3,
                                    P1609CtrlWME_WMEWSAVerifyReq, pP1609State);
    Dot3_SetCallbackWMENotificationInd(pP1609State->pDot3, P1609CtrlWME_WMENotificationInd, pP1609State);

    Dot3_SetCallbackWMERCPIInd(pP1609State->pDot3, P1609CtrlWME_WMERCPIInd, pP1609State);
  }


  if (Res < 0)
    goto Error;

Error:
  d_fnend(D_INTERN, NULL, "(pP1609State %p) = void\n", pP1609State);
  return Res;
} // P1609CtrlWME_StartServiceReq


/**
 * @brief Obtain P1609 WME information
 * @return Status 0 for succes, otherwise negative
 */
int P1609WMEUpdate (void)
{

  int RetVal = 0;

  d_printf(D_TST, NULL, "Wait for Dot3MsgRecv\n");
  int Res = Dot3_MsgRecv(pP1609State->pDot3);

  if (Res < 0)
  {
    d_error(D_DEBUG, NULL, "Dot3MsgRecv() failed %d '%s'\n",
            errno, strerror(errno));
    RetVal = -errno;
    goto Error;
  }

  // else received data OK
  d_printf(D_TST, NULL, "Dot3_MsgRecv returned Res = %d\n", Res);

Error:
  d_fnend(D_DEBUG, NULL, "() = %d\n", RetVal);

  return RetVal;
} // P1609WMEUpdate


/**
 * @brief Disconnect according to the chosen Service Request Type
 * @param pP1609State Pointer to the top-level control structure
 * @return zero for success, otherwise a negative errno
 *
 * Clear any configured WME service requests
 *
 */
int P1609CtrlWME_StopServiceReq(
  tP1609State *pP1609State)
{
  int Res = -ENOSYS;
  struct Dot3WMEUserServiceReq UsrSvcReq;
  struct Dot3WMEUserServiceReq *pReq = &(UsrSvcReq);

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  // Radio B
  memset(pReq, 0, sizeof(*pReq));
  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_SINGLE:
      Res = 0;
      break;

    case BSM_OPTS_CHANNEL_MODE_DUAL:
    case BSM_OPTS_CHANNEL_MODE_FORCED:
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      d_printf(D_INFO, NULL, "SCHB\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_DEL;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_DEDICATED;
      pReq->LclServiceIndex = DOT3_UST_ENTRIES - 1;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WsmPsid;
      pReq->Valid = DOT3_WME_USRCON_LSI | DOT3_WME_USRCON_PSID;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;

    default:
      Res = -EINVAL;
      break;
  }
  if (Res != 0)
  {
    d_printf(D_WARN, NULL, "SCHB stop failed %d\n", Res);
  }

  // Radio A
  memset(pReq, 0, sizeof(*pReq));
  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_FORCED:
      d_printf(D_INFO, NULL, "SCHA forced\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_DEL;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_UNCOND;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      pReq->Valid = DOT3_WME_USRCON_PSID;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;

    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      d_printf(D_INFO, NULL, "SCHA conditional\n");
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_DEL;
      pReq->UsrReqType = DOT3_WME_SCHREQ_AUTO_ACCESS_SVCMATCH;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      pReq->Valid = DOT3_WME_USRCON_PSID;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    case BSM_OPTS_CHANNEL_MODE_DUAL:
      pReq->UsrSvcAction = DOT3_WME_ACTION_SVCREQ_DEL;
      pReq->UsrReqType = DOT3_WME_SCHREQ_NO_ACCESS;
      pReq->ProviderServiceIdentifier = pP1609State->Cfg.WbssPsid;
      pReq->Valid = DOT3_WME_USRCON_PSID;
      Res = Dot3_WMEUserServiceReq(pP1609State->pDot3, pReq);
      break;

    default:
      Res = -EINVAL;
      break;
  }
  if (Res != 0)
  {
    d_printf(D_INFO, NULL, "SCHA stop failed %d\n", Res);
  }

  d_fnend(D_INTERN, NULL, "(pP1609State %p) = void\n", pP1609State);
  return Res;
} // P1609CtrlWME_StopServiceReq


//------------------------------------------------------------------------------
// P1609_CTRL MLME control interface (MAC Media Management Entity)
//

/**
 * @brief Setup the MLME configuration data
 * @param pP1609State Pointer to the top-level control structure
 * @return zero for success, otherwise a negative errno
 *
 */
int P1609CtrlMLME_Dot4Setup(
  tP1609State *pP1609State)
{
  int Res = -ENOSYS;
  struct Dot4MLMEMIB *pMIB = &(pP1609State->WME.MLMEMIB);

  d_fnstart(D_INTERN, NULL, "(pP1609State %p)\n", pP1609State);

  pP1609State->pDot4 = (struct Dot4 *) Dot4_Open("dot4");
  if (pP1609State->pDot4 == NULL)
  {
    Res = -ENONET;
    goto Error;
  }

  // Get the MLME MIB and apply the changes
  Res = Dot4_MLMEGet(pP1609State->pDot4, pMIB);
  if (Res != 0)
    goto Error;

  memcpy(&OriginalMIB, pMIB, sizeof(OriginalMIB));
  // Initialize the MLME MIB
  // Radio A CCH: 0 = best effort, 1 = background, 2 = video, 3 = voice
  pMIB->SystemMIB.EDCACchTable[0].EDCATableIndex = 0;
  pMIB->SystemMIB.EDCACchTable[0].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCACchTable[0].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCACchTable[0].EDCATableAIFSN = 6;
  pMIB->SystemMIB.EDCACchTable[1].EDCATableIndex = 1;
  pMIB->SystemMIB.EDCACchTable[1].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCACchTable[1].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCACchTable[1].EDCATableAIFSN = 9;
  pMIB->SystemMIB.EDCACchTable[2].EDCATableIndex = 2;
  pMIB->SystemMIB.EDCACchTable[2].EDCATableCWmin = (1 << 3) - 1;
  pMIB->SystemMIB.EDCACchTable[2].EDCATableCWmax = (1 << 4) - 1;
  pMIB->SystemMIB.EDCACchTable[2].EDCATableAIFSN = 3;
  pMIB->SystemMIB.EDCACchTable[3].EDCATableIndex = 3;
  pMIB->SystemMIB.EDCACchTable[3].EDCATableCWmin = (1 << 2) - 1;
  pMIB->SystemMIB.EDCACchTable[3].EDCATableCWmax = (1 << 3) - 1;
  pMIB->SystemMIB.EDCACchTable[3].EDCATableAIFSN = 2;
  // Radio A SCH: 0 = best effort, 1 = background, 2 = video, 3 = voice
  pMIB->SystemMIB.EDCASchTable[0].EDCATableIndex = 0;
  pMIB->SystemMIB.EDCASchTable[0].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCASchTable[0].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCASchTable[0].EDCATableAIFSN = 6;
  pMIB->SystemMIB.EDCASchTable[1].EDCATableIndex = 1;
  pMIB->SystemMIB.EDCASchTable[1].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCASchTable[1].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCASchTable[1].EDCATableAIFSN = 9;
  pMIB->SystemMIB.EDCASchTable[2].EDCATableIndex = 2;
  pMIB->SystemMIB.EDCASchTable[2].EDCATableCWmin = (1 << 3) - 1;
  pMIB->SystemMIB.EDCASchTable[2].EDCATableCWmax = (1 << 4) - 1;
  pMIB->SystemMIB.EDCASchTable[2].EDCATableAIFSN = 3;
  pMIB->SystemMIB.EDCASchTable[3].EDCATableIndex = 3;
  pMIB->SystemMIB.EDCASchTable[3].EDCATableCWmin = (1 << 2) - 1;
  pMIB->SystemMIB.EDCASchTable[3].EDCATableCWmax = (1 << 3) - 1;
  pMIB->SystemMIB.EDCASchTable[3].EDCATableAIFSN = 2;
  // Radio B Safety: 0 = best effort, 1 = background, 2 = video, 3 = voice
  pMIB->SystemMIB.EDCAXchTable[0].EDCATableIndex = 0;
  pMIB->SystemMIB.EDCAXchTable[0].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCAXchTable[0].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCAXchTable[0].EDCATableAIFSN = 6;
  pMIB->SystemMIB.EDCAXchTable[1].EDCATableIndex = 1;
  pMIB->SystemMIB.EDCAXchTable[1].EDCATableCWmin = (1 << 4) - 1;
  pMIB->SystemMIB.EDCAXchTable[1].EDCATableCWmax = (1 << 10) - 1;
  pMIB->SystemMIB.EDCAXchTable[1].EDCATableAIFSN = 9;
  pMIB->SystemMIB.EDCAXchTable[2].EDCATableIndex = 2;
  pMIB->SystemMIB.EDCAXchTable[2].EDCATableCWmin = (1 << 3) - 1;
  pMIB->SystemMIB.EDCAXchTable[2].EDCATableCWmax = (1 << 4) - 1;
  pMIB->SystemMIB.EDCAXchTable[2].EDCATableAIFSN = 3;
  pMIB->SystemMIB.EDCAXchTable[3].EDCATableIndex = 3;
  pMIB->SystemMIB.EDCAXchTable[3].EDCATableCWmin = (1 << 2) - 1;
  pMIB->SystemMIB.EDCAXchTable[3].EDCATableCWmax = (1 << 3) - 1;
  pMIB->SystemMIB.EDCAXchTable[3].EDCATableAIFSN = 2;

  pMIB->SystemMIB.ChannelList.ChannelSet[0] = 0; // Radio A CCH is off
  pMIB->SystemMIB.ChannelList.ChannelSet[1] = 0; // Radio A SCH is off
  pMIB->SystemMIB.ChannelList.ChannelSet[2] = 0; // Radio B SCH is off

  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_SINGLE:
      pMIB->SystemMIB.RadioMode[0] = SCHD_MODE_CCH_ONLY;
      pMIB->SystemMIB.RadioMode[1] = SCHD_MODE_NONE;
      break;

    case BSM_OPTS_CHANNEL_MODE_DUAL:
    default:
      pMIB->SystemMIB.RadioMode[0] = SCHD_MODE_CCH_ONLY;
      pMIB->SystemMIB.RadioMode[1] = SCHD_MODE_SCH_ONLY;
      break;

    case BSM_OPTS_CHANNEL_MODE_FORCED:
      pMIB->SystemMIB.RadioMode[0] = SCHD_MODE_CCH_SCH;
      pMIB->SystemMIB.RadioMode[1] = SCHD_MODE_SCH_ONLY;
      break;

    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      pMIB->SystemMIB.RadioMode[0] = SCHD_MODE_CCH_ONLY;
      pMIB->SystemMIB.RadioMode[1] = SCHD_MODE_SCH_ONLY;
      break;
  }

  pMIB->SystemMIB.FiltTable[0] = htons(0x88dc); // IEEE1609.3 WSMP
  pMIB->SystemMIB.FiltTable[1] = htons(0x86dd); // IPv6
  pMIB->SystemMIB.FiltTable[2] = 0;
  if (pP1609State->Cfg.IPv4)
  {
    pMIB->SystemMIB.FiltTable[2] = htons(0x0800); // IPv4
    pMIB->SystemMIB.FiltTable[3] = htons(0x0806); // ARP
    pMIB->SystemMIB.FiltTable[4] = 0;
  }

  Res = Dot4_MLMESet(pP1609State->pDot4, pMIB);
  if (Res != 0)
    goto Error;

  Res = 0;

Error:
  d_fnend(D_INTERN, NULL, "(Res %d)\n", Res);
  return Res;
} // P1609CtrlMLME_Dot4Setup

static void P1609CtrlMLME_Dot4Restore(tP1609State *pP1609State)
{
  (void)Dot4_MLMESet(pP1609State->pDot4, &OriginalMIB);
}

/**
 * @brief Get MAC address for given WLAN name
 * @param pDev WLAN device instance name
 * @param MAC buffer array for MAC address
 * @return zero for success, otherwise a negative errno
 *
 * Use a system command to get MAC address from 'fw_printenv'
 * Save output in file and read
 *
 */
static int GetWLANMAC(
  char *pDev,
  uint8_t MAC[P1609_WLAN_MAC_ADDR_LEN])
{
  int Ret = 0;
  int Stat;
  // System command to get WLAN MAC address in a file the app can read
  char Cmd[P1609_WLAN_MAC_ADDR_QUERY_LEN];

  // Clear MAC address
  memset(MAC, 0, P1609_WLAN_MAC_ADDR_LEN);

  // Contruct the system command
  memset(Cmd, 0, P1609_WLAN_MAC_ADDR_QUERY_LEN);
  strcat(Cmd, "fw_printenv ");
  strcat(Cmd, P1609_WLAN_STEM_NAME);
  strcat(Cmd, pDev);
  strcat(Cmd, " | cut -d= -f2 > ");
  strcat(Cmd, P1609_WLAN_MAC_ADDR_FILE);

  // Run system command, then read file to get info
  Stat = system(Cmd);
  if (Stat == 0)
  {
    FILE *Fd;
    // MAC address in mac.txt
    Fd = fopen(P1609_WLAN_MAC_ADDR_FILE, "r");
    if (Fd != NULL)
    {
      // scan for MAC address in the file
      Stat = fscanf(Fd, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
                    &MAC[0], &MAC[1], &MAC[2], &MAC[3], &MAC[4], &MAC[5]);
      if (Stat != P1609_WLAN_MAC_ADDR_LEN)
        d_error(D_WARN, NULL, "MAC address scanning incorrect (%d)\n", Stat);
      fclose(Fd);
      remove(P1609_WLAN_MAC_ADDR_FILE);
    }
    else
    {
      d_error(D_WARN, NULL, "Failed to open %s\n",
              P1609_WLAN_MAC_ADDR_FILE);
      Ret = -1;
    }
  }
  else
  {
    d_error(D_WARN, NULL, "Failed to run system command (%s, %d)\n",
            Cmd, Stat);
    Ret = -1;
  }

  return Ret;
}


/**
 * @brief Set the Application configuration to default.
 * @param pConfig Pointer to user configuration
 * @return Result code (0 = no failure)
 *
 * Set the P1609 configuration values based on the supplied user
 * configuration options. Invalid values may be overridden
 * and a default is applied.
 *
 */
static int P1609CtrlInfo_DefaultCfgState(
  tConfigIniData *pConfig)
{
  int Res = 0;

  // Limit check channel numbers (use default if necessary)

  pP1609State->Cfg.ChanCCHA = pConfig->ChanCchA;
  if ((pConfig->ChanCchA < P1609_CONFIG_MIN_CHANNEL) ||
      (pConfig->ChanCchA > P1609_CONFIG_MAX_CHANNEL))
  {
    pP1609State->Cfg.ChanCCHA = P1609_CONFIG_DEF_CHANNEL;
    d_error(D_ERR, NULL, "ChanCCHA out of range (%d), using default %d\n",
            pConfig->ChanCchA, pP1609State->Cfg.ChanCCHA);
  }

  pP1609State->Cfg.ChanSCHA = pConfig->ChanSchA;
  if ((pConfig->ChanSchA < P1609_CONFIG_MIN_CHANNEL) ||
      (pConfig->ChanSchA > P1609_CONFIG_MAX_CHANNEL))
  {
    pP1609State->Cfg.ChanSCHA = P1609_CONFIG_DEF_CHANNEL;
    d_error(D_ERR, NULL, "ChanSCHA out of range (%d), using default %d\n",
            pConfig->ChanSchA, pP1609State->Cfg.ChanSCHA);
  }

  pP1609State->Cfg.ChanSCHB = pConfig->ChanSchB;
  if ((pConfig->ChanSchB < P1609_CONFIG_MIN_CHANNEL) ||
      (pConfig->ChanSchB > P1609_CONFIG_MAX_CHANNEL))
  {
    pP1609State->Cfg.ChanSCHB = P1609_CONFIG_DEF_CHANNEL;
    d_error(D_ERR, NULL, "ChanSCHB out of range (%d), using default %d\n",
            pConfig->ChanSchB, pP1609State->Cfg.ChanSCHB);
  }

  d_printf(D_INFO, NULL, "ChannelMode %d\n", pConfig->ChannelMode);
  pP1609State->Cfg.ChannelMode = pConfig->ChannelMode;

  pP1609State->Cfg.ChanNbr = pConfig->ChanNbr;

  // TxDataRate values need to be checked
  switch (pConfig->TxDataRate)
  {
    case P1609_CONFIG_TXDATARATE_6:
    case P1609_CONFIG_TXDATARATE_9:
    case P1609_CONFIG_TXDATARATE_12:
    case P1609_CONFIG_TXDATARATE_18:
    case P1609_CONFIG_TXDATARATE_24:
    case P1609_CONFIG_TXDATARATE_36:
    case P1609_CONFIG_TXDATARATE_48:
    case P1609_CONFIG_TXDATARATE_54:
      if ((pP1609State->Cfg.ChanCCHA & 0x1) == 0x1)
      {
        // Odd channel number => 20MHz bandwidth
        pP1609State->Cfg.DataRate =
          pConfig->TxDataRate * P1609_CONFIG_TXDATASCALING;
      }
      else
      {
        // Even channel number => 10MHz bandwidth
        pP1609State->Cfg.DataRate =
          pConfig->TxDataRate;
      }
      break;
    default:
      d_error(D_ERR, NULL, "TxDataRate out of range (%d)\n",
              pConfig->TxDataRate);
      Res = -ENOSYS;
      goto Error;
      break;
  }

  // TxPwrLevel_dBm is 0->33, TxPwr is -64 -> +64,
  //  0= 0dB => TxPwr= 0
  //  1= 1dB => TxPwr= 2
  // 32=32dB => TxPwr=64
  // 33=33dB => TxPwr=64 (limited)
  // So limit TxPwrLevel_dBm to MKx maximum
  if (pConfig->TxPwrLevel_dBm > P1609_CONFIG_TXPOWERMAX)
  {
    // Limit TX Power to maximum supported
    pP1609State->Cfg.TxPwr =
      P1609_CONFIG_TXPOWERMAX * P1609_CONFIG_TXPOWERSCALING;
  }
  else
  {
    // Scale Tx Power
    pP1609State->Cfg.TxPwr =
      pConfig->TxPwrLevel_dBm * P1609_CONFIG_TXPOWERSCALING;
  }

  // User defined PSID for WSM
  pP1609State->Cfg.WsmPsid = pConfig->WsmPsid;
  // User defined WBSS PSID for WME
  pP1609State->Cfg.WbssPsid = pConfig->WbssPsid;
  // IPv4 support on wave-data
  pP1609State->Cfg.IPv4 = pConfig->IPv4;

  // Defaults for other non-modifiable values
  pP1609State->Cfg.WAVEElemID = DOT3_WSMP_ELMID_WSM;
  pP1609State->Cfg.Priority   = P1609_CONFIG_DEF_PRIORITY;

Error:
  return Res;
}

/**
 * @brief Get the Application configuration.
 * @return the Config structure.
 *
 * This provides access to the P1609 configuration, which can be
 * used e.g. when constructing the Wave Short Message header.
 *
 */
tWAVEConfig P1609CtrlInfo_GetCfgState(void)
{
  d_printf(D_DEBUG, NULL, "(pP1609State %p "
           "ChanCCHA %d ChanSCHA %d ChanSCHB %d ChanNbr %d "
           "Priority %d DataRate %d TxPwr %d WsmPsid %x  WbssPsid %x)\n",
           pP1609State,
           pP1609State->Cfg.ChanCCHA,
           pP1609State->Cfg.ChanSCHA,
           pP1609State->Cfg.ChanSCHB,
           pP1609State->Cfg.ChanNbr,
           pP1609State->Cfg.Priority,
           pP1609State->Cfg.DataRate,
           pP1609State->Cfg.TxPwr,
           pP1609State->Cfg.WsmPsid,
           pP1609State->Cfg.WbssPsid);

  return pP1609State->Cfg;
}


/**
 * @brief Get the stored WSMP socket file descriptor.
 * @return     The socket file descriptor.
 *
 * This provides access to the P1609 WSMP file descriptor which
 * is used e.g. when sending a Wave Short Message.
 *
 */
int P1609CtrlInfo_GetWSMPSocketFD(void)
{
  d_printf(D_DEBUG, NULL, "(pP1609State %p Fd %d)\n",
           pP1609State,
           pP1609State->WSMP.Fd);

  return pP1609State->WSMP.Fd;

}

/**
 * @brief Setup the User's IPv6 parameters
 */
void P1609CtrlWME_DoUserIPAddress (struct Dot3WMENotifInd *pInd)
{
  int Res;
  struct sockaddr_in6 SockAddr;
  char StrAddr[INET6_ADDRSTRLEN];
  uint8_t *pMAC;
  char Cmd[256];
  int Sock;
  struct ifreq IfreqBuf;

  // Bounce the 'wave-data' interface
  snprintf(Cmd, 256, "ip link set wave-data down");
  Res = system(Cmd);
  if (Res != 0)
    d_printf(D_DEBUG, NULL, "system(%s) = %d '%s'\n",
             Cmd, errno, strerror(errno));
  snprintf(Cmd, 256, "ip link set wave-data up");
  Res = system(Cmd);
  if (Res != 0)
    d_printf(D_DEBUG, NULL, "system(%s) = %d '%s'\n",
             Cmd, errno, strerror(errno));

  if (d_test(D_TST))
  {
    inet_ntop(AF_INET6, pInd->IPv6Address, StrAddr, INET6_ADDRSTRLEN);
    d_printf(D_TST, NULL, "pInd->IPv6Address: %s\n", StrAddr);
    printf("pInd->IPv6Address: %s\n", StrAddr);
  }

  // Get the IP prefix & local mac address to set our global address
  // get the MAC addess
  Sock = socket(AF_INET6, SOCK_DGRAM, 0);
  memset(&IfreqBuf, 0x00, sizeof(IfreqBuf));
  strcpy(IfreqBuf.ifr_name, "wave-data");
  ioctl(Sock, SIOCGIFHWADDR, &IfreqBuf);
  pMAC = (uint8_t *) (IfreqBuf.ifr_hwaddr.sa_data);

  // Get the Prefix from pInd
  memcpy(&(SockAddr.sin6_addr), pInd->IPv6Address, sizeof(SockAddr.sin6_addr));
  SockAddr.sin6_addr.s6_addr[8] = pMAC[0] ^ 0x02;
  SockAddr.sin6_addr.s6_addr[9] = pMAC[1];
  SockAddr.sin6_addr.s6_addr[10] = pMAC[2];
  SockAddr.sin6_addr.s6_addr[11] = 0xFF;
  SockAddr.sin6_addr.s6_addr[12] = 0xFE;
  SockAddr.sin6_addr.s6_addr[13] = pMAC[3];
  SockAddr.sin6_addr.s6_addr[14] = pMAC[4];
  SockAddr.sin6_addr.s6_addr[15] = pMAC[5];
  // Now get the global IP address string
  inet_ntop(AF_INET6, &(SockAddr.sin6_addr), StrAddr, INET6_ADDRSTRLEN);
  // Format the command to set the address & execute
  snprintf(Cmd, 256, "ip -6 addr add %s/64 dev wave-data", StrAddr);
  d_printf(D_TST, NULL, "Cmd: %s\n", Cmd);
  Res = system(Cmd);
  if (Res != 0)
    d_printf(D_DEBUG, NULL, "system(%s) = %d '%s'\n",
             Cmd, errno, strerror(errno));

  // Get the neighbour (provider) address from pInd
  memcpy(&(SockAddr.sin6_addr), pInd->DefaultGateway,
         sizeof(SockAddr.sin6_addr));
  d_printf(D_TST, NULL,
           "pInd->DefaultGateway: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
           pInd->DefaultGateway[0], pInd->DefaultGateway[1],
           pInd->DefaultGateway[2], pInd->DefaultGateway[3],
           pInd->DefaultGateway[4], pInd->DefaultGateway[5],
           pInd->DefaultGateway[6], pInd->DefaultGateway[7],
           pInd->DefaultGateway[8], pInd->DefaultGateway[9],
           pInd->DefaultGateway[10], pInd->DefaultGateway[11],
           pInd->DefaultGateway[12], pInd->DefaultGateway[13],
           pInd->DefaultGateway[14], pInd->DefaultGateway[15]);
  // Now get the IP address string
  inet_ntop(AF_INET6, &(SockAddr.sin6_addr), StrAddr, INET6_ADDRSTRLEN);
  d_printf(D_TST, NULL, "StrAddr: %s\n", StrAddr);

  // Format the command to set the neighbour & execute
  pMAC = pInd->GatewayMACAddress;
  snprintf(Cmd, 256, "ip -6 neigh add %s nud permanent "
           "lladdr %02x:%02x:%02x:%02x:%02x:%02x dev wave-data",
           StrAddr,
           pMAC[0], pMAC[1], pMAC[2], pMAC[3], pMAC[4], pMAC[5]);
  d_printf(D_TST, NULL, "Cmd: %s\n", Cmd);
  Res = system(Cmd);
  if (Res != 0)
    d_printf(D_DEBUG, NULL, "system(%s) = %d '%s'\n",
             Cmd, errno, strerror(errno));

  // Format the command to set the route & execute
  snprintf(Cmd, 256, "ip -6 route add default via %s dev wave-data", StrAddr);
  d_printf(D_TST, NULL, "Cmd: %s\n", Cmd);
  Res = system(Cmd);
  if (Res != 0)
    d_printf(D_DEBUG, NULL, "system(%s) = %d '%s'\n",
             Cmd, errno, strerror(errno));

}


/**
 * @brief Handle a WSA signing request
 * @return Zero for success or negative errno
 */
static int P1609CtrlWME_WMEWSASignReq (struct Dot3WMEData *pReq,
                                       void *pPriv)
{
  int Res = -ENOSYS;

  d_printf(D_TST, NULL, "WME-WSASign.request Length %d\n", pReq->Length);
  d_dump(D_TST, NULL, pReq->Data, pReq->Length);

  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      // Just return the same (unsigned) data
      Res = Dot3_WMEWSASignCnf(pP1609State->pDot3, pReq);
      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    case BSM_OPTS_CHANNEL_MODE_DUAL:
    case BSM_OPTS_CHANNEL_MODE_FORCED:
      Res = 0;
    default:
      break;
  }
  return Res;
}

/**
 * @brief Handle a WSA verification request
 * @return Zero for success or negative errno
 */
static int P1609CtrlWME_WMEWSAVerifyReq (struct Dot3WMEWSAVerifyReq *pReq,
                                         void *pPriv)
{
  int Res = -ENOSYS;

  d_printf(D_TST, NULL, "WME-WSAVerify.request Length %d\n", pReq->Data.Length);
  d_dump(D_TST, NULL, pReq->Data.Data, pReq->Data.Length);

  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      // Join anyway
      Res = Dot3_WMEWSAVerifyCnf(pP1609State->pDot3,
                                 (struct Dot3WMEWSAVerifyCnf *)pReq);
      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    case BSM_OPTS_CHANNEL_MODE_DUAL:
    case BSM_OPTS_CHANNEL_MODE_FORCED:
      Res = 0;
    default:
      break;
  }

  return Res;
}

/**
 * @brief Handle a WME RCPI indication
 * @return Zero for success or negative errno
 */
static int P1609CtrlWME_WMERCPIInd (struct Dot3WMEMeasurementReportSet *pInd,
                                    void *pPriv)
{
  int Res = -ENOSYS;

  d_printf(D_TST, NULL, "WME-RCPI.indication\n");
  d_dump(D_TST, NULL, pInd, sizeof(struct Dot3WMEMeasurementReportSet));

  return Res;
}

/**
 * @brief Handle a WME service notification
 * @return Zero for success or negative errno
 */
static int P1609CtrlWME_WMENotificationInd (struct Dot3WMENotifInd *pInd,
                                            void *pPriv)
{
  int Res = 0;

  switch (pP1609State->Cfg.ChannelMode)
  {
    case BSM_OPTS_CHANNEL_MODE_CONDITIONAL:
      if (pInd == NULL)
      {
        Res = -EINVAL;
        goto Error;
      }
      {
        // User
        static bool ActiveWBSS = false;
        P1609CtrlWME_PrintWMENotifIndicationPacket(pInd);
        switch (pInd->Event)
        {
          case DOT3_WME_WBSS_ACTIVE:
          {
            P1609CtrlWME_DoUserIPAddress(pInd);
            ActiveWBSS = true;
            break;
          }
          case DOT3_WME_WBSS_TERMINATED:
          {
            d_printf(D_TST, NULL, "DOT3_WME_WBSS_TERMINATED\n");
            if (ActiveWBSS == true)
            {
              ; // TODO: clear the IPv6 address config
            }
            ActiveWBSS = false;
            break;
          }
          case DOT3_WME_CHANNEL_ASSIGNED:
          {
            d_printf(D_TST, NULL, "DOT3_WME_CHANNEL_ASSIGNED\n");
            break;
          }
          case DOT3_WME_CHANNEL_NOT_ASSIGNED:
          {
            d_printf(D_TST, NULL, "DOT3_WME_CHANNEL_NOT_ASSIGNED\n");
            break;
          }
          case DOT3_WME_REQUEST_MATCHED:
          {
            d_printf(D_TST, NULL, "DOT3_WME_WBSS_MATCHED\n");
            break;
          }
          default:
          {
            d_printf(D_WARN, NULL, "Unknown WME.Notification.Ind %d\n", pInd->Event);
            break;
          }
        }
      }
      break;

    case BSM_OPTS_CHANNEL_MODE_SINGLE:
    case BSM_OPTS_CHANNEL_MODE_DUAL:
    case BSM_OPTS_CHANNEL_MODE_FORCED:
    default:
      ; // Nothing to do...
      break;
  }

Error:
  return Res;
}


/**
 * @brief Prints the passed WME notification indication packet
 * @param pInd Pointer to the notification indication
 * @return None
 */
void P1609CtrlWME_PrintWMENotifIndicationPacket (struct Dot3WMENotifInd *pInd)
{
  int Counter = 0;

  if (NULL == pInd)
  {
    fprintf(stdout, "Error: Notification indication pointer = %p", pInd);
    return;
  }

  fprintf(stdout, "------------Notification Indication------------\n");
  fprintf(stdout, "Event = %d\n", pInd->Event);
  fprintf(stdout, "Reason = %d\n", pInd->Reason);
  fprintf(stdout, "Valid = %d\n", pInd->Valid);


  if (pInd->Valid & DOT3_WME_VALID_SERVICE_PSID)
  {
    fprintf(stdout, "\nPSID = 0x%08x (%d)\n",
            pInd->ProviderServiceIdentifier, pInd->ProviderServiceIdentifier);
  }
  if (pInd->Valid & DOT3_WME_VALID_SERVICE_PSC)
  {
    fprintf(stdout, "\nProviderServiceContext.Length = %d\n",
            pInd->ProviderServiceContext.Length);
    fprintf(stdout, "ProviderServiceContext.psc\n");
    for (Counter = 0; Counter < pInd->ProviderServiceContext.Length; Counter++)
    {
      fprintf(stdout, "%02x\t",
              pInd->ProviderServiceContext.PSCContents[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_SERVICE_ADDR)
  {
    fprintf(stdout, "\nIPv6Address:\n");
    for (Counter = 0; Counter < sizeof(pInd->IPv6Address); Counter++)
    {
      fprintf(stdout, "%02x\t", pInd->IPv6Address[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_SERVICE_PORT)
  {
    fprintf(stdout, "\nServicePort = %d\n", pInd->ServicePort);
  }
  if (pInd->Valid & DOT3_WME_VALID_IPV6_PREFIX)
  {
    fprintf(stdout, "\nIPv6 Prefix (/%d):\n", pInd->IPv6Prefix.Length);
    for (Counter = 0; Counter < sizeof(pInd->IPv6Prefix.Address); Counter++)
    {
      fprintf(stdout, "%02x\t", pInd->IPv6Prefix.Address[Counter]);
    }
  }

  if (pInd->Valid & DOT3_WME_VALID_DEFAULT_GATEWAY)
  {
    fprintf(stdout, "DefaultGateway:\n");
    for (Counter = 0; Counter < 16; Counter++)
    {
      fprintf(stdout, "%02x\t", pInd->DefaultGateway[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_GATEWAY_MAC_ADDR)
  {
    fprintf(stdout, "\nGatewayMACAddress:\n");
    for (Counter = 0; Counter < 6; Counter++)
    {
      fprintf(stdout, "%02x\t", pInd->GatewayMACAddress[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_CHANNEL_INFO)
  {
    fprintf(stdout, "\nChannelInfo.count = %d\n", pInd->ChannelInfo.Count);
    fprintf(stdout, "ChannelInfo list\n");
    for (Counter = 0; Counter < pInd->ChannelInfo.Count; Counter++)
    {
      fprintf(stdout, "ChannelInfo[%d].Adaptable: %d\n", Counter,
              pInd->ChannelInfo.List[Counter].Adaptable);
      fprintf(stdout, "ChannelInfo[%d].ChannelNumber: %d\n", Counter,
              pInd->ChannelInfo.List[Counter].ChannelNumber);
      fprintf(stdout, "ChannelInfo[%d].DataRate: %d\n", Counter,
              pInd->ChannelInfo.List[Counter].DataRate);
      fprintf(stdout, "ChannelInfo[%d].TxPwrLevel: %d\n", Counter,
              pInd->ChannelInfo.List[Counter].TxPwrLevel);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_BSSID)
  {
    fprintf(stdout, "\nBSSID:\n");
    for (Counter = 0; Counter < sizeof(pInd->BSSID); Counter++)
    {
      fprintf(stdout, "%02x\t", pInd->BSSID[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_SSID)
  {
    fprintf(stdout, "\nSSID:\n");
    for (Counter = 0; Counter < sizeof(pInd->SSID); Counter++)
    {
      fprintf(stdout, "%d\t", pInd->SSID[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_TIME_STAMP)
  {
    fprintf(stdout, "\nTimestamp = 0x%08x%08x\n", (uint32_t)(pInd->Timestamp
            >> 32), (uint32_t) pInd->Timestamp);
  }
  if (pInd->Valid & DOT3_WME_VALID_LOCAL_TIME)
  {
    fprintf(stdout, "\nLocalTime = 0x%08x%08x\n", (uint32_t)(pInd->LocalTime
            >> 32), (uint32_t) pInd->LocalTime);
  }
  if (pInd->Valid & DOT3_WME_VALID_BSS_BASIC_RATE_SET)
  {
    fprintf(stdout, "\nBSSBasicRateSet:\n");
    for (Counter = 0; Counter < sizeof(pInd->BSSBasicRateSet); Counter++)
    {
      fprintf(stdout, "%d\t", pInd->BSSBasicRateSet[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_OPERATIONAL_RATE_SET)
  {
    fprintf(stdout, "\nOperationalRateSet:\n");
    for (Counter = 0; Counter < sizeof(pInd->OperationalRateSet); Counter++)
    {
      fprintf(stdout, "%d\t", pInd->OperationalRateSet[Counter]);
    }
  }
  if (pInd->Valid & DOT3_WME_VALID_EDCA_PARA_SET)
  {
    fprintf(stdout, "\nEDCA Parameters:\n");
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BE_ECWmin);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BE_ECWmax);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BE_AIFSN);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BK_ECWmin);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BK_ECWmax);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.BK_AIFSN);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VI_ECWmin);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VI_ECWmax);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VI_AIFSN);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VO_ECWmin);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VO_ECWmax);
    fprintf(stdout, "%d\t", pInd->EDCAParameterSet.VO_AIFSN);
  }
  if (pInd->Valid & DOT3_WME_VALID_RCPI)
  {
    fprintf(stdout, "\nRCPI = %d\n", pInd->RCPI);
  }
  fprintf(stdout, "\n------------------------------------------\n\n");
}

// Close the doxygen group
/**
 * @}
 */


