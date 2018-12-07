/**
 * @addtogroup bsm_shell_main BSM Shell Main Application
 * @{
 *
 * @section bsm_shell_main_doc_intro BSM shell top-level processing
 * The @c bsm-shell application does the following:
 * - Registers signal handlers (e.g. Ctrl-C)
 * - Initialises the @ref tStateStore data variables
 * - Parses user option command line variables
 * - Initialises the CAN, GPS, P1609, UDP and VIC interfaces
 * - Use poll() to wait for some input from the afore-mentioned interfaces
 *   except for VIC which is literally polled.
 * - Perform actions on each input action
 *  - CAN :
 *   - Update the @ref tCANInfo variables
 *  - GPS:
 *   - Update the @ref tGPSInfo variables
 *  - P1609:
 *   - Decode and BSM and log it's contents
 *  - P1609Sync:
 *   - Formulate a BSM based on the stored info.
 *   - Encode it into a J2735 BSM.
 *   - Send the BSM and log it's contents
 *  - VIC:
 *   - Update the @ref tVicInfo variables
 * - De-initialise interfaces when Exit condition met
 *
 * @file
 *
 * Main data structure
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __BSM_SHELL_H_
#define __BSM_SHELL_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>

#include "if-gps.h"
#include "if-can.h"
#include "if-p1609.h"
#include "if-p1609sync.h"
#include "if-udp.h"
#include "if-vic.h"


//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// Main data store stucture
typedef struct StateStore
{
  /// GPS info
  tGPSInfo GPS;
  /// CAN info
  tCANInfo CAN;
  /// P1609 WSM info
  char *pP1609WSM;
  /// P1609 WME info
  char *pP1609WME;
  /// P1609 Sync info
  tP1609SyncInfo P1609Sync;
  /// UDP info
  tUDPInfo UDP;
  /// VIC info
  tVicInfo VIC;
  ///Flag for VIC Events
  bool HardBrakingEventVIC;
  /// BSM info - blob1
  tBlob1 Blob1;
  /// BSM info - events
  uint16_t Events;
  /// Config
  tConfigIniData Config;
} tStateStore;

/// Structure Type to store Blob1 & event details from received BSM
typedef struct
{
  ///Rx Msg Id
  DSRC_DSRCmsgID_t  MsgID;
  /// BSM info - blob1
  tBlob1 Blob1;
  /// BSM info - events
  DSRC_EventFlags_t EventFlag;
  /// Flag set if hardbraking Event received
  bool HardBrakingEventRx;
  /// For extra information
  void * ExtraInfo;
} tRxBsmCache;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void  ShellGetBsmEvents(DSRC_BasicSafetyMessage_t *pBsm);

#endif  // __UTIL_BSM_H_

// Close the doxygen group
/**
 * @}
 */
