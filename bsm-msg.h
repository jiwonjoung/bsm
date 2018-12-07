/**
 * @addtogroup bsm_shell_msg BSM Shell BSM Message handling
 * @{
 *
 * @section bsm_shell_msg_doc_intro BSM message handling module
 *
 * The BSM handling code is responsible for wrapping/encapsulating the
 * @c asn1c generated code to DER encode/decode J2735 BSMs
 *  - Message memory management (alloc & free)
 *  - J2735 sub-structure manipulation
 *
 * It also takes responsibility for submitting the BSM to the P1609.3 WSMP interface
 *
 * @file
 *
 * Interface to the J2735 Basic Safety Message Library
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __UTIL_BSM_H_
#define __UTIL_BSM_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#include "dot3-wsmp.h"

#include "DSRC_BasicSafetyMessage.h" /* BasicSafetyMessage ASN.1 type */

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Blob1 is sent as a sinlge octet blob, consisting of 38 packed bytes.
#define BLOB1_SIZE 38

/// Blob element MsgCnt size
#define BLOB1_MSGCOUNT_SZ              1
/// Blob element TempID size
#define BLOB1_ID_SZ                    4
/// Blob element SecMark size
#define BLOB1_SECMARK_SZ               2
/// Blob element PosLat size
#define BLOB1_POS_LAT_SZ               4
/// Blob element PosLong size
#define BLOB1_POS_LONG_SZ              4
/// Blob element Elev size
#define BLOB1_POS_ELEV_SZ              2
/// Blob element Accuracy size
#define BLOB1_POS_ACCURACY_SZ          4
/// Blob element Speed size
#define BLOB1_MOTION_SPEED_SZ          2
/// Blob element Heading size
#define BLOB1_MOTION_SPEED_HEADING_SZ  2
/// Blob element Angle size
#define BLOB1_MOTION_SPEED_ANGLE_SZ    1
/// Blob element Accel size
#define BLOB1_MOTION_SPEED_ACCELSET_SZ 7
/// Blob element Brakes size
#define BLOB1_CONTROL_BRAKES_SZ        2
/// Blob element Vehicle size
#define BLOB1_BASIC_VEHICLESIZE_SZ     3

/// Unavailable time
#define UNAVAIL_DE_TIME 0xFFFF

/// Max latitude
#define MAX_DE_LAT      900000000
/// Min latitude
#define MIN_DE_LAT     -900000000
/// Unavailable latitude
#define UNAVAIL_DE_LAT  900000001
/// Conv GPS lat to DE units
#define CONV_DE_CAN_LAT 1e7

/// Max longitude
#define MAX_DE_LON      1800000000
/// Min longitude
#define MIN_DE_LON     -1800000000
/// Unavailable longitude
#define UNAVAIL_DE_LON  1800000001
/// Convert GPS lon to DE units
#define CONV_DE_CAN_LON 1e7

/// Max elevation (supported by J2735)
#define MAX_CAN_ELEV    6143.9
/// Min elevation (supported by J2735)
#define MIN_CAN_ELEV    -409.5
/// Max elevation
#define MAX_DE_ELEV     0xEFFF //6143.9
/// Min elevation
#define MIN_DE_ELEV     0xF001 //-409.5
/// Unavailable elevation
#define UNAVAIL_DE_ELEV 0xF000
/// Convert GPS elev to DE units
#define CONV_DE_CAN_ELEV 1e1

/// Convert GPS heading to DE units
#define CONV_DE_CAN_HEAD 8e1
/// Unavailable heading
#define UNAVAIL_DE_HEAD CONV_DE_CAN_HEAD*360

/// Unavailable positional accuracy
#define UNAVAIL_DE_POSACC 0xFFFFFFFF

/// Max speed
#define MAX_DE_SPEED 8190
/// Unavailable speed
#define UNAVAIL_DE_SPEED 8191

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/// Error codes
typedef enum J2735BSMErrorCode
{
  J2735_BSM_ERR_NONE       = 0,
  J2735_BSM_ERR_MALLOC     = 1,
  J2735_BSM_ERR_FILEOPEN   = 2,
  J2735_BSM_ERR_CONFIGFILE = 3,
} eJ2735BSMErrorCode;

/// Free methods
typedef enum J2735BSMFreeOption
{
  J2735_BSM_FREE_ALL = 0,
  J2735_BSM_FREE_CONTENTS_ONLY = 1,
} eJ2735BSMFreeOption;

/// Error codes for message creation
typedef enum P1609Ret
{
  J2735_RET_OK = 0,
  J2735_RET_ALLOC_FAIL = -1,
  J2735_RET_CREATE_FAIL = -2,
  J2735_RET_ENCODE_FAIL = -3,
  J2735_RET_DECODE_FAIL = -4,
  J2735_RET_FREE_FAIL = -5,
  J2735_RET_SEND_FAIL = -6,
  J2735_RET_SIZE_FAIL = -7,
  J2735_RET_DECODE_MORE = -8,
  J2735_RET_UNKNOWN = -255
} eP1609Ret;

/// Lights values
typedef enum SAEExteriorLights
{
  J2735_BSM_ALLLIGHTSOFF            = 0,
  J2735_BSM_LOWBEAMHEADLIGHTSON     = 1,
  J2735_BSM_HIGHBEAMHEADLIGHTSON    = 2,
  J2735_BSM_LEFTTURNSIGNALON        = 4,
  J2735_BSM_RIGHTTURNSIGNALON       = 8,
  J2735_BSM_HAZARDSIGNALON          = 12,
  J2735_BSM_AUTOMATICLIGHTCONTROLON = 16,
  J2735_BSM_DAYTIMERUNNINGLIGHTSON  = 32,
  J2735_BSM_FOGLIGHTON              = 64,
  J2735_BSM_PARKINGLIGHTSON         = 128,
} eSAEExteriorLights;

/// Event codes
typedef enum EventFlags
{
  J2735_BSM_EVENT_NONE                      = 0,
  J2735_BSM_EVENT_HazardLights              = 1,
  J2735_BSM_EVENT_StopLineViolation         = 2,
  J2735_BSM_EVENT_ABSactivated              = 4,
  J2735_BSM_EVENT_TractionControlLoss       = 8,
  J2735_BSM_EVENT_StabilityControlactivated = 16,
  J2735_BSM_EVENT_HazardousMaterials        = 32,
  J2735_BSM_EVENT_EmergencyResponse         = 64,
  J2735_BSM_EVENT_HardBraking               = 128,
  J2735_BSM_EVENT_LightsChanged             = 256,
  J2735_BSM_EVENT_WipersChanged             = 512,
  J2735_BSM_EVENT_FlatTire                  = 1024,
  J2735_BSM_EVENT_DisabledVehicle           = 2048,
  J2735_BSM_EVENT_AirBagDeployment          = 4096,
} eEventFlags;


/// part I Blob
typedef struct Blob1
{
  // Part 1 Content, sent at all times
  /// MsgCount          -- 1 byte
  uint8_t   MsgCnt;
  /// TemporaryID       -- 4 bytes
  uint32_t  Id;
  /// The curret second -- 2 bytes
  uint16_t  SecMark;

  // pos  PositionLocal3D
  /// Latitude          -- 4 bytes
  int32_t   Lat;
  /// Longitude         -- 4 bytes
  int32_t   Long;
  /// Elevation         -- 2 bytes
  int16_t   Elev;
  /// Position Accuracy -- 4 bytes
  uint32_t  Accuracy;

  // motion Motion
  /// Speed and Transmission -- 2 bytes
  uint16_t  Speed;
  /// Heading                -- 2 bytes
  uint16_t  Heading;
  /// Steering Wheel Angle   -- 1 byte
  int8_t    Angle;

  // accelSet SAEAccelerationSet4Way                   -- 7 bytes
  /// Long
  int16_t  AccelLong;
  /// Lat
  int16_t  AccelLatSet;
  /// Vert
  int8_t   AccelVert;
  /// Yaw
  int16_t  AccelYaw;

  // control Control
  /// SAEBrakeSystemStatus      -- 2 bytes
  uint16_t  Brakes;
  // -- wheelBrakes  SAEBrakeAppliedStatus,
  // --                    -x- 4 bits
  // -- wheelBrakesUnavailable  BOOL
  // --                    -x- 1 bit (1=true)
  // -- spareBit
  // --                    -x- 1 bit, set to zero
  // -- traction           TractionControlState,
  // --                    -x- 2 bits
  // -- abs                SAEAntiLockBrakeStatus,
  // --                    -x- 2 bits
  // -- scs                SAEStabilityControlStatus,
  // --                    -x- 2 bits
  // -- brakeBoost         SAEBrakeBoostApplied,
  // --                    -x- 2 bits
  // -- auxBrakes          SAEAuxiliaryBrakeStatus,
  // --                    -x- 2 bits

  // basic Vehicle Basic
  // size SAEVehicleSize   -- 3 bytes
  /// Width
  uint8_t VehicleWidth_MSB;
  /// Length
  uint16_t  VehicleLength_WidthLSB;
} __attribute__ ((packed)) tBlob1;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

int J2735_BSMSend(
  tBlob1 *pBlob1,
  uint16_t *pEvents);

int J2735_BSMReceive(
  struct Dot3WSMPHdr *pMsg);

int J2735_BSMCreate(
  DSRC_BasicSafetyMessage_t **ppBsm,
  tBlob1 *pBlob1,
  uint16_t *pEvents);

asn_enc_rval_t J2735_BSMEncode_To_Buffer(
  DSRC_BasicSafetyMessage_t *pBsm,
  char *pBuf,
  size_t Size);

asn_dec_rval_t J2735_BSMDecode(
  DSRC_BasicSafetyMessage_t **ppBsm,
  char *pBuf,
  size_t Size);

int J2735_BSMFree(
  DSRC_BasicSafetyMessage_t *pBsm,
  eJ2735BSMFreeOption FreeOption);

int WSMSend (
  struct Dot3WSMPHdr *pHdr,
  size_t Len);

#endif // __UTIL_BSM_H_

// Close the doxygen group
/**
 * @}
 */
