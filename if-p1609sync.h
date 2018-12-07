/**
 * @addtogroup bsm_shell_if_p1609sync BSM Shell P1609Sync interface
 * @{
 *
 * @section bsm_shell_if_p1609sync_doc MKx SYNC interface
 * The MKx's P1609 stack provides a highly accurate synchronisation interface
 * for applications to remain synchronised with the P1609.4 channel intervals.
 *
 * The @c wave-sync network interface (raw socket) emits a message every 50ms
 * for both radios.
 *
 * An application would typically use receive events from this interface to
 * initiate periodic processing (like sending BSMs)
 *
 * @file
 *
 * P1609Sync Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_P1609SYNC_H_
#define __IF_P1609SYNC_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>


//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// P1609Sync info
typedef struct P1609SyncInfo
{
  /// Events
  uint32_t Events;
} tP1609SyncInfo;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Start P1609Sync
int P1609SyncInit(
  void);

// Stop P1609Sync
void P1609SyncDeinit(
  int P1609SyncFd);

// Handle P1609Sync
int P1609SyncUpdate(
  int P1609SyncFd,
  tP1609SyncInfo *pP1609SyncStore);


#endif /* __IF_P1609SYNC_H_ */

// Close the doxygen group
/**
 * @}
 */

