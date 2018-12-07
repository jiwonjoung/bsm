/**
 * @addtogroup bsm_shell_if_can BSM Shell CAN interface
 * @{
 *
 * @section bsm_shell_if_can_doc MKx CAN interfaces
 * The socketCAN layer provides both Tx and Rx access to the high-speed and
 * low-speed CAN transceivers on the MKx.
 *
 * @subsection bsm_shell_if_can_doc_cfg Configuring the CAN interfaces
 * The CAN interfaces a disabled at start-up, but can enabled with the
 * @c rc.can @c start command. Similarly the interfaces can be disabled with
 * the @c rc.can @c stop command.
 *
 * The default speeds for the @c can0 and @c can1 interfaces are 500kbps and
 * 125kbps respectively.
 *
 * @file
 *
 * CAN Interface definitions
 *
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __IF_CAN_H_
#define __IF_CAN_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>


//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Set default Speed in DE units (0.02m/s) 
#define CAN_DEF_SPEED         1500  // this will be = 1500*0.02 = 30m/s
/// default Transmission state (enum)
#define CAN_DEF_TRANSSTATE    2 // forward gears
/// default Steering Wheel angle (1.5 degrees)
#define CAN_DEF_WHEELANGLE    5
/// default Longitudinal acceleration (0.01 m/s^2)
#define CAN_DEF_ACCELLONG     140
/// default Lateral acceleration (0.01 m/s^2)
#define CAN_DEF_ACCELLAT      30
/// default Vertical acceleration (0.02G)
#define CAN_DEF_ACCELVERT     20
/// default yaw (0.01 degrees/s)
#define CAN_DEF_ACCELYAW      175
/// default Brakes status (bit field)
#define CAN_DEF_BRAKESSTAT    0
/// default Vehicle width (cm)
#define CAN_DEF_VEHWIDTH      203
/// default Vehicle length (cm)
#define CAN_DEF_VEHLENGTH     420
/// default Events (bitfield)
#define CAN_DEF_EVENTFLAGS    0x100 // lights changed


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// CAN info
typedef struct CANInfo
{
  /// Speed
  uint16_t Speed;
  /// PRNDL
  uint8_t TransmissionState;
  /// Steering Wheel Angle
  int8_t SteeringWheelAngle;

  /// SAEAcceleration - long
  int16_t AccelLong;
  /// SAEAcceleration - long
  int16_t AccelLat;
  /// SAEAcceleration - long
  int16_t AccelVert;
  /// SAEAcceleration - long
  int16_t AccelYaw;

  /// Brakes set
  uint16_t SAEBrakeSystemStatus;

  /// Vehicle width
  uint16_t VehicleWidth;
  /// Vehicle length
  uint16_t VehicleLength;

  /// Events
  uint16_t EventFlags;
} tCANInfo;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Start CAN
int CANInit(
  void);

// Stop CAN
void CANDeinit(
  int CANFd);

// Handle CAN
int CANUpdate(
  int CANFd,
  tCANInfo *pCANStore);

// Default CAN bus values
void CANDefaults(
  tCANInfo *pCANStore);

#endif /* __IF_CAN_H_ */

// Close the doxygen group
/**
 * @}
 */

