/**
 * @addtogroup bsm_shell_if_p1609 BSM Shell P1609 interface
 * @{
 *
 * @section bsm_shell_if_p1609_doc MKx P1609 stack interface
 * Applications control the P1609.3 WME and the P1609.4 MLME via the lib1609 APIs
 *
 * The IEEE1609_WSMP protocol socket provides a WSMP/WSMP-S interface for Tx and Rx
 *
 * @subsection ieee1609_channel_modes IEEE 1609 stack channel modes
 * The 1609 stack supports the following modes of operation:
@verbatim

    -------------------------------time--------------------------------------->

 --ChannelMode=0 (Single)
           +-------------------+-------------------+-------------------+------
   Radio A | --ChanCCHA (Continious)                                      ...
           +-------------------+-------------------+-------------------+------
           +-------------------+-------------------+-------------------+------
   Radio B | (Disabled)                                                   ...
           +-------------------+-------------------+-------------------+------

 --ChannelMode=1 (Dual)
           +-------------------+-------------------+-------------------+------
   Radio A | --ChanCCHA (Continious)                                      ...
           +-------------------+-------------------+-------------------+------
           +-------------------+-------------------+-------------------+------
   Radio B | --ChanSCHB (Continious)                                      ...
           +-------------------+-------------------+-------------------+------

 --ChannelMode=2 (Forced)
           +-------------------+-------------------+-------------------+------
   Radio A | --ChanCCHA (50ms) | --ChanSCHA (50ms) | --ChanCCHA (50ms) | ...
           +-------------------+-------------------+-------------------+------
           +-------------------+-------------------+-------------------+------
   Radio B | --ChanSCHB (Continious)                                      ...
           +-------------------+-------------------+-------------------+------

 --ChannelMode=3 (Conditional)
                  +-------------------+-------------------+------
   Radio A        | --ChanCCHA (Continious)                 ...
(no WSA received) +-------------------+-------------------+------
                  +-------------------+-------------------+------
                  | --ChanCCHA (50ms) | --ChanSCHA (50ms) | ...
(WSA received)    +-------------------+-------------------+------
           +-------------------+-------------------+-------------------+------
   Radio B | --ChanSCHB (Continious)                                      ...
           +-------------------+-------------------+-------------------+------
@endverbatim
 *
 *
 *
 * @subsection ieee1609dot3_arch IEEE 1609.3 architecture
 *
@verbatim
                         +---------------------+
                         |  WAVE applications  |
                         +---------------------+
                             |            |
                +---------------------+   |
                |     lib1609dot3     |   |
                +---------------------+   |
                            |             |
                            |        AF_IEEE1609 socket
Userspace                   |             |
---------------------------------------------------------------
Kernel                      |             |
                   netlink socket   +------------+
                            |       | dot3: WSMP |
                            |       +------------+
                      +-----------+       |
                      | dot3: WME |       |
                      +-----------+       |
                         |                |
         +-----------------+      +----------------+
         | dot4: WAVE-MGMT |      | dot4: WAVE-RAW |
         +-----------------+      +----------------+

@endverbatim
 *
 * @subsection ieee1609dot4_arch IEEE 1609.4 architecture
 *
@verbatim
                         +---------------------+
                         |  WAVE applications  |
                         +---------------------+
           MKx remote API |   | dot4 API   |
                +---------------------+    |
                |     lib1609dot4     |    |
                +---------------------+    |
                         |       |         |
                         |       |   Raw Linux socket (AF_LINUX, SOCK_RAW)
                         |       |         |
                       ioctl     |         |
                 (low level)     |         |
                         |    netlink      |
                         |    (high level) |
Userspace                |       |         |
---------------------------------------------------------------
Kernel                   |       |         |
                         |       |         |   +------------+
                         |       |         +---| dot4: MGMT |
                         |       |         |   +------------+
                         |       |         |    +------------+
                         |       |         +----| dot4: SYNC |
                         |       |         |    +------------+
                         |       |         |     +------------+
                         |       |         +-----| dot4: RAW  |
                         |       |         |     +------------+
                         |       |         |      +------------+
                         |       |         +------| dot4: DATA |
                         |       |                +------------+
                       +------------+
                       | dot4: MLME |
                       +------------+
                                +-------------+
                                |  dot4: WMAC |
                                +-------------+

@endverbatim

 * @file
 *
 * P1609 Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_P1609_H_
#define __IF_P1609_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>

#include "lib1609.h"
#include "bsm-opts.h"


//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Bit for P1609_CTRL open
#define P1609_CTRL_OPEN  0x01
/// Bit for P1609_CTRL start
#define P1609_CTRL_START 0x02

/// Max config value for Tx Power
#define P1609_CONFIG_TXPOWERMAX     32

/// Scaling value for Tx Power units (1 step = 1 dB)
#define P1609_CONFIG_TXPOWERSCALING  1

/// Valid Tx Data Rate values (6)
#define P1609_CONFIG_TXDATARATE_6    6
/// Valid Tx Data Rate values (9)
#define P1609_CONFIG_TXDATARATE_9    9
/// Valid Tx Data Rate values (12)
#define P1609_CONFIG_TXDATARATE_12  12
/// Valid Tx Data Rate values (18)
#define P1609_CONFIG_TXDATARATE_18  18
/// Valid Tx Data Rate values (24)
#define P1609_CONFIG_TXDATARATE_24  24
/// Valid Tx Data Rate values (36)
#define P1609_CONFIG_TXDATARATE_36  36
/// Valid Tx Data Rate values (48)
#define P1609_CONFIG_TXDATARATE_48  48
/// Valid Tx Data Rate values (54)
#define P1609_CONFIG_TXDATARATE_54  54

/// Scaling value for Tx Data Rate (2 steps = 1Mbps)
#define P1609_CONFIG_TXDATASCALING   2

/// Min channel number
#define P1609_CONFIG_MIN_CHANNEL   168
/// Max channel number
#define P1609_CONFIG_MAX_CHANNEL   184
/// Default channel number
#define P1609_CONFIG_DEF_CHANNEL   178

/// Default PRIORITY
#define P1609_CONFIG_DEF_PRIORITY  2

/// P1609 receiver buffer size - at least MTU size
#define P1609_RX_BUF_SIZE 2048


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// Dot3 configuration
typedef struct P1609_CTRL_WME
{
  /// WME MIB
  struct Dot3WMEMIB  WMEMIB;
  /// MLME MIB
  struct Dot4MLMEMIB MLMEMIB;
  /// Socket fd
  int Fd;
} tP1609_CTRL_WME;


/// WSMP socket store
typedef struct P1609_CTRL_WSMP
{
  /// Socket fd
  int Fd;

} tP1609_CTRL_WSMP;

/// WAVE configuration parameters
typedef struct WAVEConfig
{
  /// CCH channel number (on radio A)
  uint8_t ChanCCHA;
  /// SCH channel number (on radio A)
  uint8_t ChanSCHA;
  /// SCH channel number (on radio B)
  uint8_t ChanSCHB;
  /// Channel mode
  uint8_t ChannelMode;
  /// Priority
  uint8_t Priority;
  /// Channel number (WSM header)
  uint8_t ChanNbr;
  /// Data rate (WSM header)
  uint8_t DataRate;
  /// Tx power (WSM header)
  uint8_t TxPwr;
  /// WSM PSID (WSM header)
  uint32_t WsmPsid;
  /// WBSS PSID (WME header)
  uint32_t WbssPsid;
  /// WAVEElemID
  uint8_t WAVEElemID;
  /// SafetySupp
  uint32_t SafetySupp;
  /// IPv4
  bool IPv4;
} tWAVEConfig;

/// P1609 state
typedef struct P1609State
{
  /// Handle to the lib1609.3 interface
  struct Dot3 *pDot3;
  /// Handle to the lib1609.4 interface
  struct Dot4 *pDot4;
  /// WME MIBs
  tP1609_CTRL_WME WME;
  /// WSMP Socket
  tP1609_CTRL_WSMP WSMP;
  /// Configuration
  tWAVEConfig Cfg;
  /// Service Request Type
  uint8_t ServiceReqType;
} tP1609State;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

int P1609WSMInit(tConfigIniData *pConfig);
int P1609WMEInit (tConfigIniData *pConfig);
void P1609WSMDeinit(void);
void P1609WMEDeinit(void);
int P1609WSMUpdate(int P1609Fd,char **pRxBuf);
int P1609WMEUpdate(void);

int P1609CtrlWSMP_OpenInterface(tP1609State *pP1609State);
void P1609CtrlWSMP_CloseInterface(tP1609State *pP1609State);

int P1609CtrlWME_Dot3Setup(tP1609State *pP1609State);
int P1609CtrlWME_StartServiceReq(tP1609State *pP1609State);
int P1609CtrlWME_StopServiceReq(tP1609State *pP1609State);

int P1609CtrlMLME_Dot4Setup(tP1609State *pP1609State);

tWAVEConfig P1609CtrlInfo_GetCfgState(void);

int P1609CtrlInfo_GetWSMPSocketFD(void);

#endif /* __IF_P1609_H_ */

// Close the doxygen group
/**
 * @}
 */

