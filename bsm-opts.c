/**
 * @addtogroup bsm_shell_opt BSM Shell Options handling
 * @{
 *
 *
 *
 * @file
 *
 * Module code for CONFIG interface
 *
 * Provide services to set user configuration from command-line
 * options.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "linux/ieee1609/dot3/dot3-wsmp.h"

#include "debug-levels.h"

#include "bsm-opts.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// Number of elements in the @ref BSMOptsList
#define BSM_OPTS_NBR_ELEM (sizeof (BSMOptsList) / sizeof (tBSMOptsEntry))


//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------

/// Per-option details
typedef struct BSMOptsEntry
{
  /// Short 'tag' for this option
  char Tag;
  /// Long 'string' for this option
  char *pLong;
  /// Long requirements: no_argument, required_argument or optional_argument
  int Req;
  /// Description
  char *pDescription;
  /// Valid option values
  char *pValues;
} tBSMOptsEntry;

//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------

/// Set default options
static void BSMOpts_Defaults(
  tConfigIniData *pBSMOpts);


//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// All the BSM options described in one place
static tBSMOptsEntry BSMOptsList[] =
{
  {   BSM_OPTS_TAG_HELP, "help", no_argument,
    "Print this usage information", "N/A" },
  {   BSM_OPTS_TAG_LOGFILE, "LogFile", required_argument,
      "Log filename", "string" },
  {   BSM_OPTS_TAG_LOGUDP, "LogUdp", no_argument,
      "Send logs to UDP peer", ""},
  {   BSM_OPTS_TAG_CHANCCHA, "ChanCCHA", required_argument,
      "CCH channel Number (on radio A)", "uint8_t"},
  {   BSM_OPTS_TAG_CHANSCHA, "ChanSCHA", required_argument,
      "SCHA channel Number (on radio A)", "uint8_t"},
  {   BSM_OPTS_TAG_CHANSCHB, "ChanSCHB", required_argument,
      "SCHB channel Number (on radio B)", "uint8_t"},
  {   BSM_OPTS_TAG_TXCOUNTER, "TxCounter", required_argument,
      "Total number of messages to transmit", "uint32" },
  {   BSM_OPTS_TAG_CHANNBR, "ChanNbr", required_argument,
      "Channel Number", "uint8_t"},
  {   BSM_OPTS_TAG_CHANMODE, "ChannelMode", required_argument,
      "Channel Mode", "0=Single 1=Dual 2=Forced 3=Conditional"},
  {   BSM_OPTS_TAG_DATARATE, "DataRate", required_argument,
      "Data rate", "uint8_t"},
  {   BSM_OPTS_TAG_TXPWR, "TxPwr", required_argument,
      "Transmit power", "uint8_t"},
  {   BSM_OPTS_TAG_WSM_PSID, "WsmPsid", required_argument,
      "WSM PSID", "uint32_t"},
  {   BSM_OPTS_TAG_WBSS_PSID, "WbssPsid", required_argument,
      "WBSS PSID", "uint32_t"},
  {   BSM_OPTS_TAG_UDPADDR, "UdpAddr", required_argument,
      "UDP peer IP (v4) Address" , "aaa.bbb.ccc.ddd" },
  {   BSM_OPTS_TAG_UDPTXPRT, "UdpTxPort", required_argument,
      "UDP port to use when sending to peer", "uint16_t"},
  {   BSM_OPTS_TAG_UDPRXPRT, "UdpRxPort", required_argument,
      "UDP port to accept peer messages on", "uint16_t"},
  {   BSM_OPTS_TAG_DUMP, "DumpPayload", no_argument,
      "Dump Packet Payload to log file", ""},
  {   BSM_OPTS_TAG_STDOUT, "DumpToStdout", no_argument,
      "Dump packets to Standard output", ""},
  {   BSM_OPTS_TAG_IPV4, "IPv4", no_argument,
      "Enable IPv4 on wave-data", ""},
  { 0, NULL, 0, NULL, NULL }
};


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief Print the application's command line usage
 * @param pAppName The program name (argv[0])
 *
 */
void BSMOpts_Usage(
  const char *pAppName)
{
  int i;

  printf("%s <Option>\n", pAppName);

  for (i = 0; BSMOptsList[i].Tag != 0; i++)
  {
    printf("-%c --%-15s: %s\n"
           "%17s\n", //"%17s  %s\n",
           BSMOptsList[i].Tag, BSMOptsList[i].pLong,
           BSMOptsList[i].pDescription,
           BSMOptsList[i].pValues);
  }

}


/**
 * @brief Print a BSM options object
 * @param pBSMOpts Options object
 *
 */
void BSMOpts_Print(
  tConfigIniData *pBSMOpts)
{
  int i;

  for (i = 0; BSMOptsList[i].Tag != 0; i++)
  {
    switch (BSMOptsList[i].Tag)
    {
      case BSM_OPTS_TAG_HELP:
        // Nothing to print
        break;

      case BSM_OPTS_TAG_LOGFILE:
        printf("--%s=%s\n", BSMOptsList[i].pLong, pBSMOpts->LogFileName);
        break;

      case BSM_OPTS_TAG_LOGUDP:
        if (pBSMOpts->LogUdp)
          printf("--%s\n", BSMOptsList[i].pLong);
        break;

      case BSM_OPTS_TAG_CHANCCHA:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->ChanCchA);
        break;

      case BSM_OPTS_TAG_CHANSCHA:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->ChanSchA);
        break;

      case BSM_OPTS_TAG_CHANSCHB:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->ChanSchB);
        break;

      case BSM_OPTS_TAG_TXCOUNTER:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->TxCounter);
        break;

      case BSM_OPTS_TAG_CHANNBR:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->ChanNbr);
        break;

      case BSM_OPTS_TAG_CHANMODE:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->ChannelMode);
        break;

      case BSM_OPTS_TAG_DATARATE:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->TxDataRate);
        break;

      case BSM_OPTS_TAG_TXPWR:
        printf("--%s=%d\n", BSMOptsList[i].pLong, pBSMOpts->TxPwrLevel_dBm);
        break;

      case BSM_OPTS_TAG_WSM_PSID:
        printf("--%s=0x%08x\n", BSMOptsList[i].pLong, pBSMOpts->WsmPsid);
        break;

      case BSM_OPTS_TAG_WBSS_PSID:
        printf("--%s=0x%08x\n", BSMOptsList[i].pLong, pBSMOpts->WbssPsid);
        break;

      case BSM_OPTS_TAG_UDPADDR:
        printf("--%s=%s\n", BSMOptsList[i].pLong, inet_ntoa(pBSMOpts->UdpAddr));
        break;

      case BSM_OPTS_TAG_UDPTXPRT:
        printf("--%s=0x%04x\n", BSMOptsList[i].pLong, pBSMOpts->UdpTxPort);
        break;

      case BSM_OPTS_TAG_UDPRXPRT:
        printf("--%s=0x%04x\n", BSMOptsList[i].pLong, pBSMOpts->UdpRxPort);
        break;

      case BSM_OPTS_TAG_DUMP:
        if (pBSMOpts->DumpPayload)
          printf("--%s\n", BSMOptsList[i].pLong);
        break;

      case BSM_OPTS_TAG_STDOUT:
        if (pBSMOpts->DumpToStdout)
          printf("--%s\n", BSMOptsList[i].pLong);
        break;

      case BSM_OPTS_TAG_IPV4:
        if (pBSMOpts->IPv4)
          printf("--%s\n", BSMOptsList[i].pLong);
        break;

      default:
        break;
    }
  }
}


/**
 * @brief Parse command line options
 * @param Argc Number of arguments
 * @param ppArgv command-line Arguments
 * @param pBSMOpts Options object
 * @return Zero if successful, otherwise a negative errno
 *
 */
int BSMOpts_Parse (
  int Argc,
  char **ppArgv,
  tConfigIniData *pBSMOpts)
{
  struct option LongOptions[BSM_OPTS_NBR_ELEM];
  char ShortOptions[((BSM_OPTS_NBR_ELEM * 3) + 1)];
  char *pShort;
  int i;
  int Mode;
  int Err;
  int OptTag = 0;
  int OptIndex = 0;

  // Short option specification
  for (i = 0, pShort = ShortOptions; BSMOptsList[i].Tag != 0; i++)
  {
    // Short option string for this opt: "x", "x:" or "x::"
    *(pShort++) = BSMOptsList[i].Tag;
    if (BSMOptsList[i].Req == no_argument)
      ; // Nothing to do
    else if (BSMOptsList[i].Req == required_argument)
      *(pShort++) = ':';
    else if (BSMOptsList[i].Req == optional_argument)
    {
      *(pShort++) = ':';
      *(pShort++) = ':';
    }
  }
  *pShort = '\0';

  // Long option specification
  for (i = 0; BSMOptsList[i].Tag != 0; i++)
  {
    LongOptions[i].name        = BSMOptsList[i].pLong;
    LongOptions[i].has_arg     = BSMOptsList[i].Req;
    LongOptions[i].flag        = 0;
    LongOptions[i].val         = BSMOptsList[i].Tag;
  }

  // Apply the default settings first
  BSMOpts_Defaults(pBSMOpts);

  while (1)
  {
    Err = BSM_OPTS_ERR_NONE; // set the error for this arg to none

    OptTag = getopt_long_only(Argc, ppArgv, ShortOptions, LongOptions, &OptIndex);
    d_printf(D_DEBUG, NULL, "OptTag %d OptionIndex %d optarg %s\n",
             OptTag, OptIndex, optarg);
    if (OptTag == -1)
      return 0;

    switch (OptTag)
    {
      case BSM_OPTS_TAG_HELP:
        Err = BSM_OPTS_ERR_UNKNOWN;
        goto Error;
        break;

      case BSM_OPTS_TAG_LOGFILE:
        strcpy(pBSMOpts->LogFileName, optarg);
        break;

      case BSM_OPTS_TAG_LOGUDP:
        pBSMOpts->LogUdp = true;
        break;

      case BSM_OPTS_TAG_CHANCCHA:
        pBSMOpts->ChanCchA = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_CHANSCHA:
        pBSMOpts->ChanSchA = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_CHANSCHB:
        pBSMOpts->ChanSchB = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_TXCOUNTER:
        pBSMOpts->TxCounter = strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_CHANNBR:
        pBSMOpts->ChanNbr = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_CHANMODE:
        Mode = 0xFF & strtoul(optarg, NULL, 0);
        if (Mode == BSM_OPTS_CHANNEL_MODE_SINGLE)
          pBSMOpts->ChannelMode = BSM_OPTS_CHANNEL_MODE_SINGLE;
        else if (Mode == BSM_OPTS_CHANNEL_MODE_DUAL)
          pBSMOpts->ChannelMode = BSM_OPTS_CHANNEL_MODE_DUAL;
        else if (Mode == BSM_OPTS_CHANNEL_MODE_FORCED)
          pBSMOpts->ChannelMode = BSM_OPTS_CHANNEL_MODE_FORCED;
        else if (Mode == BSM_OPTS_CHANNEL_MODE_CONDITIONAL)
          pBSMOpts->ChannelMode = BSM_OPTS_CHANNEL_MODE_CONDITIONAL;
        else
          goto Error;
        break;

      case BSM_OPTS_TAG_DATARATE:
        pBSMOpts->TxDataRate = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_TXPWR:
        pBSMOpts->TxPwrLevel_dBm = 0xFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_WSM_PSID:
        pBSMOpts->WsmPsid = 0xFFFFFFFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_WBSS_PSID:
        pBSMOpts->WbssPsid = 0xFFFFFFFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_UDPADDR:
        if (inet_aton(optarg, &pBSMOpts->UdpAddr) == 0)
        {
          Err = BSM_OPTS_ERR_INVALIDOPTIONARG;
          goto Error;
        }
        break;

      case BSM_OPTS_TAG_UDPTXPRT:
        pBSMOpts->UdpTxPort = 0xFFFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_UDPRXPRT:
        pBSMOpts->UdpRxPort = 0xFFFF & strtoul(optarg, NULL, 0);
        break;

      case BSM_OPTS_TAG_DUMP:
        pBSMOpts->DumpPayload = true;
        break;

      case BSM_OPTS_TAG_STDOUT:
        pBSMOpts->DumpToStdout = true;
        break;

      case BSM_OPTS_TAG_IPV4:
        pBSMOpts->IPv4 = true;
        break;

      default:
        Err = BSM_OPTS_ERR_INVALIDOPTION;
        break;
    }

    if (Err != BSM_OPTS_ERR_NONE)
      goto Error;

  } // while getopt is not finished

  return 0;
Error:
  if (Err == BSM_OPTS_ERR_INVALIDOPTIONARG)
    fprintf(stderr, "invalid argument %s of option %s\n",
            optarg, ppArgv[optind-2] );
  if (Err == BSM_OPTS_ERR_INVALIDOPTION)
    fprintf(stderr, "invalid option %s\n", ppArgv[optind-2] );
  return -1;
}


/**
 * @brief Set the options to default values
 * @param pBSMOpts Options object
 *
 */
static void BSMOpts_Defaults(
  tConfigIniData *pBSMOpts)
{
  strncpy(pBSMOpts->LogFileName,
          BSM_OPTS_LOG_FILENAME,
          BSM_OPTS_MAXFILENAMELENGTH);
  pBSMOpts->LogUdp = false;
  pBSMOpts->ChanCchA = BSM_OPTS_CHAN_CCHA_DEFAULT;
  pBSMOpts->ChanSchA = BSM_OPTS_CHAN_SCHA_DEFAULT;
  pBSMOpts->ChanSchB = BSM_OPTS_CHAN_SCHB_DEFAULT;
  pBSMOpts->TxCounter = 0;
  pBSMOpts->ChanNbr = pBSMOpts->ChanCchA;
  pBSMOpts->ChannelMode = BSM_OPTS_CHANNEL_MODE_DUAL;
  pBSMOpts->TxDataRate = DOT3_WSMP_WSM_DATARATE_6MBPS;
  pBSMOpts->TxPwrLevel_dBm = DOT3_WSMP_WSM_TXPWR_MAX;
  pBSMOpts->WsmPsid = BSM_OPTS_WSM_PSID_DEFAULT;
  pBSMOpts->WbssPsid = BSM_OPTS_WBSS_PSID_DEFAULT;
  (void)inet_aton(BSM_OPTS_UDP_ADDRESS_DEFAULT, &pBSMOpts->UdpAddr);
  pBSMOpts->UdpTxPort = BSM_OPTS_UDP_TX_PORT;
  pBSMOpts->UdpRxPort = BSM_OPTS_UDP_RX_PORT;
  pBSMOpts->DumpPayload = false;
  pBSMOpts->DumpToStdout = false;
  pBSMOpts->IPv4 = false;
}


// Close the doxygen group
/**
 * @}
 */

