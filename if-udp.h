/**
 * @addtogroup bsm_shell_if_udp BSM Shell UDP interface
 * @{
 *
 *
 * @section bsm_shell_if_udp_doc UDP example interface
 * A simple UDP interface library is provided for example purposes.
 *
 * The application uses this interface to stream UDP logs to a remote server.
 *
 * @file
 *
 * UDP Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2010 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_UDP_H_
#define __IF_UDP_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bsm-opts.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// UDP module status
typedef enum UdpStatus
{
  /// Not initialized
  UDP_STATUS_NONE     = 0,
  /// Running listening for peer messages & sending
  UDP_STATUS_RUNNING  = 1,
  /// Not enabled (default command line state)
  UDP_STATUS_DISABLED = 2,

  /// Socket initilization failed
  UDP_STATUS_EINIT    = -1,

} eUdpStatus;


/// UDP information
typedef struct UDPInfo
{
  /// Module status (@sa eUdpStatus)
  int Status;

  /// Socket info
  struct sockaddr_in Sock;

  /// UDP->WSM data buffer pointer
  void *pWSMTxBuf;
  /// UDP->WSM data length
  int WSMTxLen;

  /// WSM->UDP data buffer pointer
  void *pWSMRxBuf;
  /// WSM->UDP data length
  int WSMRxLen;

} tUDPInfo;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Start UDP
int UDPInit(
  tConfigIniData *pConfig);

// Stop UDP
void UDPDeinit(
  int UDPFd);

// Handle UDP
int UDPUpdate(
  int UDPFd,
  tUDPInfo *pUDPStore);

// Send on UDP
int UDPSend(
  int MsgLen,
  const char *pMsg);


#endif /* __IF_UDP_H_ */

// Close the doxygen group
/**
 * @}
 */

