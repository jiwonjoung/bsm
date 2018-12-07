/**
 * @addtogroup bsm_shell_log BSM Shell Logging library
 * @{
 *
 * @section bsm_shell_log_udp_doc Example logging library
 * A simple logging library is provided for example purposes.
 *
 * The application uses this interface to log a string or a BSM
 * (Basic Safety Message) to the initialised destination.
 *
 * @file
 *
 * LOG Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_LOG_H_
#define __IF_LOG_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "DSRC_BasicSafetyMessage.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// No logging
#define LOG_DEST_NONE 0
/// Log to stdout
#define LOG_DEST_STDOUT 0x01
/// Log to File
#define LOG_DEST_FILE   0x02
/// Log to UDP
#define LOG_DEST_UDP    0x04


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------


/// Start LOG
int LOGInit(
  int Dest,
  char *pFile);

// Stop LOG
int LOGDeinit(
  void);

/// Write string to log
void LOGString(
  const char *pString);

/// Write BSM to log
void LOGBsm(
  const char *pString,
  DSRC_BasicSafetyMessage_t *pBsm);

#endif /* __IF_LOG_H_ */

// Close the doxygen group
/**
 * @}
 */
