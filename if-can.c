/**
 * @addtogroup bsm_shell_if_can BSM Shell CAN interface
 * @{
 *
 * @file
 *
 * Module code for CAN interface
 *
 * @ref CANInit() : Initialise the CAN interface by creating the CAN protocol
 * socket for communications to socket-CAN, which provides access to the CAN
 * transceivers on the MKx. The initialisation provides a file descriptor
 * for the main management routine to monitor in a select() call.
 *
 * @ref CANUpdate() | When data becomes available on the file descriptor, the
 * 'update' routine is called to fetch data from the interface and write it
 * into the system store.
 *
 * For this application the CAN receiver is exercised only, and that is
 * only to fetch the data from the CAN bus, but not to parse it. Not all
 * CAN interfaces are standardised, so a default set of data values are
 * provided for vehicle information.
 *
 * @ref CANDeinit() : A de-initialisation routine is provided for the
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
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include "debug-levels.h"

#include "if-can.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

// At time of writing, these constants are not defined in the headers

#ifndef PF_CAN
/// Domain for CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
/// Family for CAN
#define AF_CAN PF_CAN
#endif


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
 * @brief CAN Init
 * @return File descriptor to monitor for CAN input
 *
 * Open CAN interface and provide the file descriptor to monitor for data
 *
 */
int CANInit(void)
{
  d_fnstart(D_DEBUG, NULL, "()\n");

  // Raw socket
  int FdSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (FdSock < 0)
  {
    // oops
    d_error(D_ERR, NULL, "CAN: socket failed: %d, %s\n",
            errno, strerror(errno));
    return -errno;
  }

  // Set index to 0, ie. ANY device
  struct sockaddr_can Addr;
  Addr.can_family  = AF_CAN;
  Addr.can_ifindex = 0;

  int rc = bind(FdSock, (struct sockaddr *)&Addr, sizeof(Addr));
  if (rc)
  {
    // oops
    d_error(D_ERR, NULL, "CAN: bind(%d, ANY) failed: %d, %s\n",
            FdSock, errno, strerror(errno));
    close(FdSock);
    return -errno;
  }

  d_fnend(D_DEBUG, NULL, "() = %d\n", FdSock);

  return FdSock;
}


/**
 * @brief CAN interface de-initialisation
 *
 * Clean up CAN interface
 * @param CANFd File descriptor to close
 *
 * Close the file descriptor
 */
void CANDeinit(
  int CANFd)
{
  d_fnstart(D_DEBUG, NULL, "(CANFd = %d)\n", CANFd);

  // tidy up interfaces
  int Ret = close(CANFd);
  if (Ret < 0)
  {
    d_error(D_ERR, NULL, "CAN: close failed: %d, %s\n",
            errno, strerror(errno));
  }

  d_fnend(D_DEBUG, NULL, "()\n");
}


/**
 * @brief Update provided store with CAN information
 * @param CANFd File descriptor to read
 * @param pCANStore Data structure for updating
 * @return Status, 0 for success, otherwise negative
 *
 * When data is available to be read, the management function calls this
 * 'update' functions to read the CAN file descriptor. The CAN message id
 * and data are acquired and would normally be interpreted to attain the
 * vehicle status. Currently this interpretation is not done due to
 * differences in CAN message types.
 *
 */
int CANUpdate(
  int CANFd,
  tCANInfo *pCANStore)
{
  d_fnstart(D_DEBUG, NULL, "(CANFd %d, pCANStore %p)\n",
            CANFd, pCANStore);

  // read the next CAN frame
  struct can_frame Frame;
  size_t NBytes = read(CANFd, &Frame, sizeof(struct can_frame));
  // did we get what we want?
  if (NBytes != sizeof(struct can_frame))
  {
    // did we get anything?
    if (NBytes > 0)
    {
      d_error(D_WARN, NULL, "CAN: read(%d) returned short: %zd < %zd\n",
              CANFd, NBytes, sizeof(struct can_frame));
      // try again.....
      // continue;
    }
    if (NBytes)
    {
      // read error
      d_error(D_ERR, NULL, "CAN: read(%d) failed: %d, %s\n",
              CANFd, errno, strerror(errno));
    }
    else
    {
      // EOF
      d_error(D_ERR, NULL, "CAN: read(%d) EOF!\n", CANFd);
    }
  }

  d_printf(D_DEBUG, NULL, "CAN message: 0x%02X\n", Frame.can_id);
  d_dump(D_DEBUG, NULL, &Frame.data, Frame.can_dlc);

  // Update Store
  // Nothing to do yet!!

  d_fnend(D_DEBUG, NULL, "()\n");

  return 0;
}


/**
 * @brief Set the CAN store values to defaults
 * @param pCANStore Data structure for updating
 *
 * Set some dummy CAN data in case no CAN exists. This can be done by
 * the management function to set some meaningful values to the CAN
 * data store in case proper acquisition of vehicle status from live CAN
 * is not possible.
 *
 */
void CANDefaults(
  tCANInfo *pCANStore)
{
  // Should update from CAN, but may not be using standardised 600-605
  // CAN messages.
  pCANStore->Speed              = CAN_DEF_SPEED;
  pCANStore->TransmissionState  = CAN_DEF_TRANSSTATE;
  pCANStore->SteeringWheelAngle = CAN_DEF_WHEELANGLE;
  pCANStore->AccelLong          = CAN_DEF_ACCELLONG;
  pCANStore->AccelLat           = CAN_DEF_ACCELLAT;
  pCANStore->AccelVert          = CAN_DEF_ACCELVERT;
  pCANStore->AccelYaw           = CAN_DEF_ACCELYAW;
  pCANStore->SAEBrakeSystemStatus  = CAN_DEF_BRAKESSTAT;
  pCANStore->VehicleWidth       = CAN_DEF_VEHWIDTH;
  pCANStore->VehicleLength      = CAN_DEF_VEHLENGTH;
  pCANStore->EventFlags         = CAN_DEF_EVENTFLAGS;
}


// Close the doxygen group
/**
 * @}
 */

