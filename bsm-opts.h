/**
 * @addtogroup bsm_shell_opt BSM Shell Options handling
 * @{
 *
 * @section bsm_shell_opt_doc_intro BSM shell options processing
 * The options parsing code is responsible for:
 * - Setting all options to default values
 * - Parsing the provided options and populating the @ref tConfigIniData
 *   structure and potentially printing usage information
 * - Printing the full configuration options (for feedback)
 *
 * @section bsm_shell_opt_doc_usage Usage
@verbatim
./bsm-shell <Option>
-? --help           : Print this usage information
              N/A
-f --LogFile        : Log filename
           string
-l --LogUdp         : Send logs to UDP peer

-c --ChanCCHA       : CCH channel Number (on radio A)
          uint8_t
-a --ChanSCHA       : SCHA channel Number (on radio A)
          uint8_t
-b --ChanSCHB       : SCHA channel Number (on radio B)
          uint8_t
-i --TxCounter      : Total number of messages to transmit
           uint32
-n --ChanNbr        : Channel Number
          uint8_t
-m --ChannelMode    : Channel Mode
      0=Single 1=Dual 2=Forced 3=Conditional
-k --DataRate       : Data rate
          uint8_t
-p --TxPwr          : Transmit power
          uint8_t
-x --WsmPsid        : WSM PSID
         uint32_t
-u --UdpAddr        : UDP peer IP (v4) Address
     aaa.bbb.ccc.ddd
-y --UdpTxPort      : UDP port to use when sending to peer
         uint16_t
-z --UdpRxPort      : UDP port to accept peer messages on
         uint16_t
-d --DumpPayload    : Dump Packet Payload to log file

-s --DumpToStdout   : Dump packets to Standard output

-4 --IPv4           : Enable IPv4 on wave-data

-w  --WbssPsid      : PSID for WBSS connection

@endverbatim
 *
 * @file
 *
 * CONFIG Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __BSM_OPTS_H_
#define __BSM_OPTS_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <netinet/in.h>


//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Radio A: CCH on, SCH off. Radio B: SCH off
#define BSM_OPTS_CHANNEL_MODE_SINGLE            0
/// Radio A: CCH on, SCH off. Radio B: SCH on
#define BSM_OPTS_CHANNEL_MODE_DUAL              1
/// Radio A: CCH on, SCH on. Radio B: SCH on
#define BSM_OPTS_CHANNEL_MODE_FORCED            2
/// Radio A: CCH on, SCH 'conditional on matching WSA'. Radio B: SCH on
#define BSM_OPTS_CHANNEL_MODE_CONDITIONAL       3

/// Default log filename
#define BSM_OPTS_LOG_FILENAME "bsmlog.txt"
/// Maximum length of option lists
#define BSM_OPTS_MAXFILENAMELENGTH (512)
/// Default CCHA channel number
#define BSM_OPTS_CHAN_CCHA_DEFAULT (178)
/// Default SCHA channel number
#define BSM_OPTS_CHAN_SCHA_DEFAULT (176)
/// Default SCHB channel number
#define BSM_OPTS_CHAN_SCHB_DEFAULT (174)
/// Default WSM PSID (0x10)
#define BSM_OPTS_WSM_PSID_DEFAULT   (0x10)
/// Default WBSS PSID (0x0FF)
#define BSM_OPTS_WBSS_PSID_DEFAULT   (0x0FF)
/// WBSS PSID for WBSS OFF (0x0FF)
#define BSM_OPTS_WBSS_PSID_OFF   (0x0FF)
/// Default address of UDP receiver
#define BSM_OPTS_UDP_ADDRESS_DEFAULT "0.0.0.0"
/// BSM_OPTS_UDP port to send to
#define BSM_OPTS_UDP_TX_PORT 4200
/// Default UDP port to listen on
#define BSM_OPTS_UDP_RX_PORT 4201


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// BSMOpt Error Codes
typedef enum BSMOptsErrCode
{
  BSM_OPTS_ERR_NONE = 0,
  BSM_OPTS_ERR_UNKNOWN = -1,
  BSM_OPTS_ERR_INVALIDOPTION = -2,
  BSM_OPTS_ERR_INVALIDOPTIONARG = -3,
} eBSMOptsErrCode;

/// Option tag values
typedef enum BSMOptsTag
{
  BSM_OPTS_TAG_HELP     = '?',
  BSM_OPTS_TAG_LOGFILE  = 'f',
  BSM_OPTS_TAG_LOGUDP   = 'l',
  BSM_OPTS_TAG_TXCOUNTER= 'i',
  BSM_OPTS_TAG_STDOUT   = 's',
  BSM_OPTS_TAG_DUMP     = 'd',
  BSM_OPTS_TAG_CHANNBR  = 'n',
  BSM_OPTS_TAG_CHANMODE = 'm',
  BSM_OPTS_TAG_DATARATE = 'k',
  BSM_OPTS_TAG_TXPWR    = 'p',
  BSM_OPTS_TAG_WSM_PSID = 'x',
  BSM_OPTS_TAG_WBSS_PSID= 'w',
  BSM_OPTS_TAG_CHANCCHA = 'c',
  BSM_OPTS_TAG_CHANSCHA = 'a',
  BSM_OPTS_TAG_CHANSCHB = 'b',
  BSM_OPTS_TAG_UDPADDR  = 'u',
  BSM_OPTS_TAG_UDPTXPRT = 'y',
  BSM_OPTS_TAG_UDPRXPRT = 'z',
  BSM_OPTS_TAG_IPV4     = '4',
} eBSMOptsTag;


/// User configuration (built from Cmd line opts).
typedef struct ConfigIniData
{
  /// Messages log filename
  char LogFileName[BSM_OPTS_MAXFILENAMELENGTH];
  /// Log to UDP
  bool LogUdp;

  /// CCHA channel number
  uint8_t ChanCchA;
  /// SCHA channel number
  uint8_t ChanSchA;
  /// SCHB channel number
  uint8_t ChanSchB;

  /// Channel mode
  ///  - Single:      No SCH assigned on radio A nor radio B (single radio operation on CCHA)
  ///  - Dual:        No SCH assigned on radio A (CCHA & SCHB have continious access)
  ///  - Forced:      SCH on radio A assigned by config (radio A is always channel switching)
  ///  - Conditional: SCH on radio A conditionally assigned by WBSS (based on received WSAs)
  uint8_t ChannelMode;

  /// Number of BSMs to be transmitted in total
  uint32_t TxCounter;

  /// Channel number (WSM header)
  uint8_t ChanNbr;
  /// Data rate (WSM header)
  uint8_t TxDataRate;
  /// Tx power (WSM header)
  uint8_t TxPwrLevel_dBm;
  /// WSM PSID (WSM header)
  uint32_t WsmPsid;
  /// WBSS PSID (WME)
  uint32_t WbssPsid;
  /// UDP peer address
  struct in_addr UdpAddr;
  /// UDP port to use when sending to peer
  uint16_t UdpTxPort;
  /// UDP port to accept peer messages on
  uint16_t UdpRxPort;

  /// Dump entire packet payload to file
  bool DumpPayload;
  /// Should per packet details be dumped to stdout?
  bool DumpToStdout;

  /// Should IPv4 enabled on wave-data?
  bool IPv4;
} tConfigIniData;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void BSMOpts_Usage(
  const char *pAppName);

void BSMOpts_Print(
  tConfigIniData * pConfig);

int BSMOpts_Parse (
  int Argc,
  char **ppArgv,
  tConfigIniData * pConfig);

#endif /* __BSM_OPTS_H_ */

// Close the doxygen group
/**
 * @}
 */

