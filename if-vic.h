/**
 * @addtogroup bsm_shell_if_vic BSM Shell Vehicle Interface Connector (VIC) interface
 * @{
 *
 *
 * @section bsm_shell_if_vic_doc VIC example interface
 * A simple vehicle interface connector library is provided for example purposes.
 *
 * The application uses this interface to read the state of VIC General Purpose Input Output (GPIO) pins.
 *
 * @file
 *
 * VIC Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_VIC_H_
#define __IF_VIC_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "bsm-opts.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// VIC module status
typedef enum VicStatus
{
  /// Unable to initialise VIC GPIO
  VIC_STATUS_EINIT    = -1,
  /// Not initialized
  VIC_STATUS_NONE     = 0,
  /// Running: data available
  VIC_STATUS_RUNNING  = 1,
  /// Disabled: (some) data unavailable
  VIC_STATUS_DISABLED = 2,
} eVicStatus;

typedef enum GpinState
{
  /// Error or unreadable
  GPIN_UNKNOWN = -1,
  /// Low, Off or Zero
  GPIN_LOW = 0,
  /// High, On, or One
  GPIN_HIGH = 1,
} eGpinState;

/// VIC information
typedef struct VICInfo
{
  /// Module status (@sa eVicStatus)
  eVicStatus status;
  // State of GPIN 0
  eGpinState gpin0State;
  // State of GPIN 1
  eGpinState gpin1State;
  // State of GPIN 2
  eGpinState gpin2State;
} tVicInfo;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Start VIC
int VICInit(tVicInfo *pVICInformation);

// Stop VIC
void VICDeinit();

// Handle VIC
int VICUpdate();

#endif /* __IF_VIC_H_ */

// Close the doxygen group
/**
 * @}
 */

