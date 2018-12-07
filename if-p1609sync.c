/**
 * @addtogroup bsm_shell_if_p1609sync BSM Shell P1609Sync interface
 * @{
 *
 * @file
 *
 * Module code for P1609Sync interface
 *
 * @ref P1609SyncInit() : Initialise P1609 sync interface by creating a
 * socket for WAVE_SYNC device.
 * Return a file descriptor for the the main management routine to
 * monitor in a select() call.
 *
 * @ref P1609SyncUpdate() : When data becomes available on the file
 * descriptor, the 'update' routine is called to fetch data from the
 * interface and identify the interval type.
 * This information is passed back to the management routine.
 * Note: Placing the info in a single store would create a race condition.
 *
 * @ref P1609SyncDeinit() : A de-initialisation routine is provided for the
 * management routine to call at application exit.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

// Sync interfaces
#include "linux/ieee1609/dot4/mk2mac-api-types.h"

#include "debug-levels.h"

#include "if-p1609sync.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// wave-sync device name
#define WAVE_SYNC_DEV_NAME      ("wave-sync")

/// wave-sync MTU
#define WAVE_SYNC_DEV_MTU       (sizeof(tMK2SyncDescriptor))

//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief P1609Sync Init
 * @return File descriptor to monitor for P1609 Sync input
 *
 * Open P1609Sync interface and attain the file descriptor which is
 * passed to the management function. The management function can monitor
 * this descriptor to see when data is ready to be read.
 *
 */
int P1609SyncInit(void)
{
  d_fnstart(D_DEBUG, NULL, "()\n");

  // create socket
  int P1609SyncFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (P1609SyncFd < 0)
  {
    d_error(D_WARN, NULL, "socket() failed %d '%s'\n", errno, strerror(errno));
    if (errno == 1)
    {
      // Operation not permitted
      d_printf(D_WARN, NULL, "Consider running as root?\n");
    }
    return -errno;
  }

  struct sockaddr_ll SocketAddress = { 0, };

  // and bind to it
  SocketAddress.sll_family   = AF_PACKET;
  SocketAddress.sll_protocol = htons(ETH_P_ALL);
  SocketAddress.sll_ifindex = if_nametoindex(WAVE_SYNC_DEV_NAME);

  int Res = bind(P1609SyncFd, (struct sockaddr *)&SocketAddress, sizeof(SocketAddress));
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "bind() failed %d '%s'\n", errno, strerror(errno));
    return -errno;
  }
  else
  {
    d_printf(D_DEBUG, NULL, "Opened the WAVE-SYNC socket\n");
  }

  d_fnend(D_DEBUG, NULL, "() = %d\n", P1609SyncFd);

  return P1609SyncFd;
}


/**
 * @brief P1609Sync Deinit
 * @param P1609SyncFd File descriptor to close
 *
 * Clean up P1609Sync interface
 *
 */
void P1609SyncDeinit(
  int P1609SyncFd)
{
  d_fnstart(D_DEBUG, NULL, "(P1609SyncFd = %d)\n", P1609SyncFd);

  // tidy up interfaces
  int Ret = close(P1609SyncFd);
  if (Ret < 0)
  {
    d_error(D_ERR, NULL, "P1609Sync: close failed: %d, %s\n",
            errno, strerror(errno));
  }

  d_fnend(D_DEBUG, NULL, "()\n");
}


/**
 * @brief Update provided store with P1609Sync information
 * @param P1609SyncFd File descriptor to read
 * @param pP1609SyncStore Data structure for updating
 * @return zero for success, otherwise a negative errno
 *
 * When the management function sees that data is ready to be read
 * on the P1609 Sync interface, this 'update' function is called. The
 * Sync descriptor information is read and, assuming no errors, the
 * channel interval identifier is returned.
 *
 * One issue to note is that this event occurs at the start of
 * the frame interval (CCH or SCH). Any actions performed following the
 * update function will occur right at the start of the frame which may be
 * undesirable. One solution is to include some delay or intentional jitter
 * between this synchronisation event and the actions.
 *
 */
int P1609SyncUpdate(
  int P1609SyncFd,
  tP1609SyncInfo *pP1609SyncStore)
{
  d_fnstart(D_DEBUG, NULL, "(P1609SyncFd %d, pP1609SyncStore %p)\n",
            P1609SyncFd, pP1609SyncStore);

  int Flags = 0;
  tMK2SyncDescriptor SyncDes;

  struct sockaddr_ll SocketAddress = { 0, };
  socklen_t AddrLen = sizeof(SocketAddress);

  int Res = recvfrom(P1609SyncFd, &SyncDes, WAVE_SYNC_DEV_MTU, Flags,
                     (struct sockaddr *)&SocketAddress, &AddrLen);

  if (Res < 0)
  {
    d_error(D_ERR, NULL, "recvfrom() failed %d '%s'\n",
            errno, strerror(errno));
    return -errno;
  }
  else if (Res == 0)
  {
    d_error(D_WARN, NULL, "recvfrom() got no bytes %d '%s'\n",
            errno, strerror(errno));
    return -ENODATA;
  }
  else if (Res != WAVE_SYNC_DEV_MTU)
  {
    d_error(D_ERR, NULL, "Invalid data amount, got %d bytes\n", Res);
    return -EBADE;
  }
  // else received data OK, Res is amount of data

  d_printf(D_INFO, NULL, "got WAVE_SYNC_DEV_MTU: %3d %d %d %lld\n",
           SyncDes.Channel,                  // enum (CCH, SCH etc...)
           SyncDes.ChannelNumber,            // channel number of radio
           SyncDes.ChannelUtilisationPeriod, // should be 49
           (long long int)SyncDes.TSF);      // timestamp from radio in microseconds

  d_fnend(D_DEBUG, NULL, "() = %d\n", SyncDes.Channel);

  return SyncDes.Channel;
}


// Close the doxygen group
/**
 * @}
 */


