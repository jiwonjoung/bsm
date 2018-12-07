/**
 * @addtogroup bsm_shell_if_udp BSM Shell UDP interface
 * @{
 *
 * @file
 *
 * Module code for UDP interface
 *
 * @ref UDPInit() : Initialise the UDP interface by creating the socket for
 * UDP communications. The initialisation provides a file descriptor
 * for the main management routine to monitor in a select() call.
 * A socket for UDP transmission is created.
 *
 * @ref UDPUpdate() : When data becomes available on the file descriptor,
 * the 'update' routine is called to fetch data from the interface.
 * The received UDP frame is not stored.
 *
 * @ref UDPSend() : The UDP send function sends the provided data string via
 * the UDP transmission socket.
 *
 * @ref UDPDeinit() : A de-initialisation routine is provided for the
 * management routine to call at application exit.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "dot3-wsmp.h"
#include "if-udp.h"
#include "debug-levels.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------

/// Default size of buffer for received UDP frame
#define UDP_RX_BUF_SIZE 1472

//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// Tx socket of peer
static int TxSkt = -1;

/// Tx address of peer
static struct in_addr TxIPv4Addr = { 0, };

/// Tx port
static uint16_t TxPort = BSM_OPTS_UDP_TX_PORT;

/// Rx buffer size
static int RxBufSize = UDP_RX_BUF_SIZE;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief UDP Init
 * @param pConfig Pointer to user configuration
 *
 * Open UDP interface and set up the UDP handler. The function returns
 * a file descriptor for the management function to monitor when
 * data is ready to be read.
 *
 */
int UDPInit(tConfigIniData *pConfig)
{
  d_fnstart(D_DEBUG, NULL, "(pConfig %p)\n", pConfig);

  /// UDP datagram socket
  int FdSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (FdSock < 0)
  {
    // oops
    d_error(D_CRIT, NULL, "CAN: RX socket failed %d, %s\n",
            errno, strerror(errno));
    FdSock = -errno;
    goto Exit;
  }

  struct sockaddr_in RxAddr = {0};
  RxAddr.sin_family      = AF_INET;
  RxAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  RxAddr.sin_port        = htons(pConfig->UdpRxPort);

  int rc = bind(FdSock, (struct sockaddr *)&RxAddr, sizeof(RxAddr));
  if (rc < 0)
  {
    // oops
    d_error(D_ERR, NULL, "UDP: bind(%d, ANY) RX failed: %d, %s\n",
            FdSock, errno, strerror(errno));
    close(FdSock);
    FdSock = -errno;
    goto Exit;
  }
  else
  {
    d_printf(D_DEBUG, NULL, "UDP: bind on RX port %d\n",
             pConfig->UdpRxPort);
  }

  // Tx socket
  TxSkt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (TxSkt == -1)
  {
    // oops
    d_error(D_ERR, NULL, "UDP TX socket failed: %d, %s\n",
            errno, strerror(errno));
  }
  else
  {
    d_printf(D_DEBUG, NULL, "UDP: TX port %d\n",
             pConfig->UdpTxPort);
  }

  TxIPv4Addr = pConfig->UdpAddr;
  TxPort = pConfig->UdpTxPort;

Exit:
  d_fnend(D_DEBUG, NULL, "() = %d\n", FdSock);

  return FdSock;
}


/**
 * @brief UDP Deinit
 * @param UDPFd File descriptor to close
 *
 * Clean up UDP interface
 *
 */
void UDPDeinit(
  int UDPFd)
{
  d_fnstart(D_DEBUG, NULL, "(UDPFd %d)\n", UDPFd);

  // tidy up interfaces
  int Res = close(UDPFd);
  if (Res < 0)
  {
    d_error(D_ERR, NULL, "UDP: close failed: %d, %s\n",
            errno, strerror(errno));
  }

  d_fnend(D_DEBUG, NULL, "()\n");
}


/**
 * @brief Update provided store with UDP information
 * @param UDPFd File descriptor to read
 * @param pUDPStore Data structure for updating
 * @return zero for success, otherwise a negative errno
 *
 * When data is ready to be read, the management function calls this
 * 'update' function to store the received data.
 *
 */
int UDPUpdate(
  int UDPFd,
  tUDPInfo *pUDPStore)
{
  d_fnstart(D_DEBUG, NULL, "(UDPFd %d, pUDPStore %p)\n",
            UDPFd, pUDPStore);

  // read the next UDP frame
  char *pBuf = (char *)calloc(1, RxBufSize);

  if (pBuf == NULL)
    goto Error;

  // Move the data pointer past the 1609.3 header (save us doing a copy)
  char *pRxPtr = pBuf + sizeof(struct Dot3WSMPHdr);
  size_t RxLen = RxBufSize - sizeof(struct Dot3WSMPHdr) -1;
  size_t NbrBytes = read(UDPFd, pRxPtr, RxLen);

  // Did we get anything?
  if (NbrBytes == -1)
  {
    d_error(D_ERR, NULL, "UDP: read(%d) failed: %d, %s\n",
            UDPFd, errno, strerror(errno));
    goto Error;
  }
  else if (NbrBytes == 0)
  {
    // EOF
    d_error(D_ERR, NULL, "UDP: read(%d) EOF!\n", UDPFd);
    goto Error;
  }

  d_printf(D_DEBUG, NULL, "Received %zd bytes\n", NbrBytes);

  // Store the info for later use in the UdpAction() function
  if (pUDPStore->pWSMTxBuf == NULL)
  {
    // pWSMTxBuf is the exclusion value, so WSMTxLen is set first to avoid a race
    // This would be important if this was a multi-threaded app... which it isn't
    pUDPStore->WSMTxLen = NbrBytes;
    pUDPStore->pWSMTxBuf = pBuf;
    goto Success;
  }
  else
  {
    d_error(D_WARN, NULL, "Unable to queue UDP packet for OTA Tx (EBUSY)\n");
  }

Error:
  if (pBuf != NULL)
    free(pBuf);
Success:
  d_fnend(D_DEBUG, NULL, "()\n");
  return 0;
}


/**
 * @brief Send data to UDP port
 * @param MsgLen Length of message to send
 * @param pMsg Message to send
 * @return zero for success, otherwise a negative errno
 *
 * Provides a mechanism to send an information string over the UDP
 * connection. The receiver address & port is set in the user configuration.
 *
 */
int UDPSend(
  int MsgLen,
  const char *pMsg)
{
  d_fnstart(D_DEBUG, NULL, "(MsgLen %d, Msg %p)\n",
            MsgLen, pMsg);

  int Ret = 0;

  struct sockaddr_in TxAddr = {0};
  TxAddr.sin_family = AF_INET;
  TxAddr.sin_port   = htons(TxPort);
  TxAddr.sin_addr   = TxIPv4Addr;

  // And send
  if (TxSkt != -1)
  {
    d_printf(D_DEBUG, NULL, "Sending %d bytes: %p\n", MsgLen, pMsg);

    ssize_t Res = sendto(TxSkt, pMsg, MsgLen, 0,
                         (const struct sockaddr *)&TxAddr, sizeof(TxAddr));

    if (Res < 0)
    {
      d_error(D_ERR, NULL, "Failed to write to TX socket (%d,%s)\n",
              errno, strerror(errno));
    }
    Ret = 0;
  }
  else
  {
    // No socket, indicate error
    d_error(D_ERR, NULL, "No Tx Socket exists\n");
    Ret = -EBADFD;
  }

  d_fnend(D_DEBUG, NULL, "()\n");

  return Ret;
}


// Close the doxygen group
/**
 * @}
 */


