/**
 * @addtogroup bsm_shell_if_vic BSM Shell Vehicle Interface Connector (VIC) interface
 * @{
 *
 * @file
 *
 * Module code for VIC interface
 *
 * @ref VICInit() : Initialises the VIC interface.
 * Saves the pointer to the data store and tests if the SysFS VIC interface
 * is available.
 *
 * @ref VICUpdate() : Reads the state of the VIC General Purpose Input Output (GPIO)
 * pins and updates the store with the state of pins 0, 1 and 2.
 *
 * @ref VICDeinit() : A de-initialisation routine is provided for the
 * management routine to call at application exit.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "if-vic.h"
#include "debug-levels.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------
#if defined(BOARD_MK4) || defined (BOARD_MK5)
#define GPIN0_PATH "/sys/class/gpio/gpio144/value"
#else
#define GPIN0_PATH "/sys/devices/system/vic_gpio/vic_gpio0/gpin0"
#endif

#if defined(BOARD_MK4)
#define GPIN1_PATH "/sys/class/gpio/gpio141/value"
#elif defined(BOARD_MK5)
#define GPIN1_PATH "/sys/class/gpio/gpio125/value"
#else
#define GPIN1_PATH "/sys/devices/system/vic_gpio/vic_gpio0/gpin1"
#endif

#if defined(BOARD_MK4) || defined (BOARD_MK5)
#define GPIN2_PATH "/sys/class/gpio/gpio142/value"
#else
#define GPIN2_PATH "/sys/devices/system/vic_gpio/vic_gpio0/gpin2"
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
static tVicInfo *vicInfo = NULL;


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief VIC Init
 * @param pVICInformation Pointer to configuration structure
 * @return 0 if successful, VIC_STATUS_EINIT in error.
 *
 * Verify that the VIC GPIO interface exists, and store the pointer
 * the the user supplied information structure.
 * Initialise the structure.
 */
int VICInit(tVicInfo *pVICInformation)
{
  int returnValue = 0;

  d_fnstart(D_DEBUG, NULL, "(pVICInformation %p)\n", pVICInformation);

  // Set the status to failed unless everything initialises successfully
  pVICInformation->status = VIC_STATUS_EINIT;
  pVICInformation->gpin0State = GPIN_UNKNOWN;
  pVICInformation->gpin1State = GPIN_UNKNOWN;
  pVICInformation->gpin2State = GPIN_UNKNOWN;

  // VIC GPIO sysfs node check for GPIN0
  if (access(GPIN0_PATH, R_OK) != 0)
  {
    d_error(D_CRIT, NULL, "VIC: Cannot access GPIN0 %d, %s\n",
            errno, strerror(errno));
    returnValue = VIC_STATUS_EINIT;
  }

  // VIC GPIO sysfs node check for GPIN1
  if (access(GPIN1_PATH, R_OK) != 0)
  {
    d_error(D_CRIT, NULL, "VIC: Cannot access GPIN1 %d, %s\n",
            errno, strerror(errno));
    returnValue = VIC_STATUS_EINIT;
  }

  // VIC GPIO sysfs node check for GPIN2
  if (access(GPIN2_PATH, R_OK) != 0)
  {
    d_error(D_CRIT, NULL, "VIC: Cannot access GPIN2 %d, %s\n",
            errno, strerror(errno));
    returnValue = VIC_STATUS_EINIT;
  }

  // Initialised, but data not yet available
  pVICInformation->status = returnValue;
  vicInfo = pVICInformation;

  return returnValue;
}


/**
 * @brief VIC Deinit
 *
 * Clean up VIC interface.
 */
void VICDeinit()
{
  // Intentionally empty
}


/**
 * @brief Update store with VIC information
 * @return 0 for no update, +1 for each state change, -1 otherwise.
 *
 * Updates the GPIN 0,1 and 2 states and stores them
 * in the information structure provided.
 * If a pin cannot be read, it will be stored in the
 * structure in GPIN_UNKNOWN state.
 * If a PIN state has changed from the last time it was read, the return
 * value is incremented (except where errors are concerned).
 */
int VICUpdate()
{
  int returnValue = 0;
  int fd;
  char gpinStateBuffer[2]; // Read only the first byte - contains state '0' or '1'

  d_fnstart(D_DEBUG, NULL, "(VICUpdate)\n");

  // No valid pointer to store results of read in to.
  if (vicInfo == NULL)
  {
    d_error(D_ERR, NULL, "VIC: VICInit() not yet run. No storage for pin states: aborting update.\n");
    return -1;
  }

  if (vicInfo->status == VIC_STATUS_EINIT)
  {
    d_printf(D_DEBUG, NULL, "VIC:  VicInfo status = %d\n", vicInfo->status);
    return -1;
  }
  // Null terminate the char buffer
  gpinStateBuffer[1] = '\0';

  // Read GPIN0
  fd = open(GPIN0_PATH, O_RDONLY);
  if (fd < 0)
  {
    d_error(D_ERR, NULL, "VIC: open GPIN0 failed: %d, %s\n",
            errno, strerror(errno));
    vicInfo->gpin0State = GPIN_UNKNOWN;
    vicInfo->status = VIC_STATUS_DISABLED;
    returnValue = -1;
  }
  else
  {
    // Read 1 byte - the first character contains a zero or one
    if (read(fd, &gpinStateBuffer, 1) == 1)
    {
      // As GPIN_LOW is 0 and GPIN_HIGH is 1 it can be read and
      // used directly since the device driver mapping is equivalent.
      int gpinState = atoi(gpinStateBuffer);
      if (vicInfo->gpin0State != gpinState)
      {
        vicInfo->gpin0State = gpinState;
        returnValue++;
      }
    }
    close(fd);
  }

  // Read GPIN1
  fd = open(GPIN1_PATH, O_RDONLY);
  if (fd < 0)
  {
    d_error(D_ERR, NULL, "VIC: open GPIN1 failed: %d, %s\n",
            errno, strerror(errno));
    vicInfo->gpin1State = GPIN_UNKNOWN;
    vicInfo->status = VIC_STATUS_DISABLED;
    returnValue = -1;
  }
  else
  {
    // Read 1 byte - the first character contains a zero or one
    if (read(fd, &gpinStateBuffer, 1) == 1)
    {
      // As GPIN_LOW is 0 and GPIN_HIGH is 1 it can be read and
      // used directly since the device driver mapping is equivalent.
      int gpinState = atoi(gpinStateBuffer);
      if (vicInfo->gpin1State != gpinState)
      {
        vicInfo->gpin1State = gpinState;
        returnValue++;
      }
    }
    close(fd);
  }

  // Read GPIN2
  fd = open(GPIN2_PATH, O_RDONLY);
  if (fd < 0)
  {
    d_error(D_ERR, NULL, "VIC: open GPIN2 failed: %d, %s\n",
            errno, strerror(errno));
    vicInfo->gpin2State = GPIN_UNKNOWN;
    vicInfo->status = VIC_STATUS_DISABLED;
    returnValue = -1;
  }
  else
  {
    // Read 1 byte - the first character contains a zero or one
    if (read(fd, &gpinStateBuffer, 1) == 1)
    {
      // As GPIN_LOW is 0 and GPIN_HIGH is 1 it can be read and
      // used directly since the device driver mapping is equivalent.
      int gpinState = atoi(gpinStateBuffer);
      if (vicInfo->gpin2State != gpinState)
      {
        vicInfo->gpin2State = gpinState;
        returnValue++;
      }
    }
    close(fd);
  }

  if (returnValue >= 0)
  {
    vicInfo->status = VIC_STATUS_RUNNING;
  }

  d_fnend(D_DEBUG, NULL, "()\n");

  return returnValue;
}


// Close the doxygen group
/**
 * @}
 */
