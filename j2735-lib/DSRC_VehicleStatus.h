/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_VehicleStatus_H_
#define	_DSRC_VehicleStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_ExteriorLights.h"
#include "DSRC_LightbarInUse.h"
#include "DSRC_BrakeSystemStatus.h"
#include "DSRC_BrakeAppliedPressure.h"
#include "DSRC_CoefficientOfFriction.h"
#include "DSRC_SunSensor.h"
#include "DSRC_RainSensor.h"
#include "DSRC_AmbientAirTemperature.h"
#include "DSRC_AmbientAirPressure.h"
#include "DSRC_FullPositionVector.h"
#include "DSRC_ThrottlePosition.h"
#include "DSRC_SpeedandHeadingandThrottleConfidence.h"
#include "DSRC_SpeedConfidence.h"
#include "DSRC_VehicleIdent.h"
#include "DSRC_J1939data.h"
#include "DSRC_GPSstatus.h"
#include "DSRC_WiperStatusFront.h"
#include "DSRC_WiperRate.h"
#include "DSRC_WiperStatusRear.h"
#include <constr_SEQUENCE.h>
#include "DSRC_SteeringWheelAngle.h"
#include "DSRC_SteeringWheelAngleConfidence.h"
#include "DSRC_SteeringWheelAngleRateOfChange.h"
#include "DSRC_DrivingWheelAngle.h"
#include "DSRC_AccelerationSet4Way.h"
#include "DSRC_VerticalAccelerationThreshold.h"
#include "DSRC_YawRateConfidence.h"
#include "DSRC_AccelerationConfidence.h"
#include "DSRC_ConfidenceSet.h"
#include "DSRC_ObstacleDistance.h"
#include "DSRC_ObstacleDirection.h"
#include "DSRC_DDateTime.h"
#include "DSRC_VehicleHeight.h"
#include "DSRC_BumperHeights.h"
#include "DSRC_VehicleMass.h"
#include "DSRC_TrailerWeight.h"
#include "DSRC_VehicleType.h"
#include "DSRC_EssPrecipYesNo.h"
#include "DSRC_EssPrecipRate.h"
#include "DSRC_EssPrecipSituation.h"
#include "DSRC_EssSolarRadiation.h"
#include "DSRC_EssMobileFriction.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_VehicleStatus */
typedef struct DSRC_VehicleStatus {
	DSRC_ExteriorLights_t	*lights	/* OPTIONAL */;
	DSRC_LightbarInUse_t	*lightBar	/* OPTIONAL */;
	struct DSRC_VehicleStatus__wipers {
		DSRC_WiperStatusFront_t	 statusFront;
		DSRC_WiperRate_t	 rateFront;
		DSRC_WiperStatusRear_t	*statusRear	/* OPTIONAL */;
		DSRC_WiperRate_t	*rateRear	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *wipers;
	DSRC_BrakeSystemStatus_t	*brakeStatus	/* OPTIONAL */;
	DSRC_BrakeAppliedPressure_t	*brakePressure	/* OPTIONAL */;
	DSRC_CoefficientOfFriction_t	*roadFriction	/* OPTIONAL */;
	DSRC_SunSensor_t	*sunData	/* OPTIONAL */;
	DSRC_RainSensor_t	*rainData	/* OPTIONAL */;
	DSRC_AmbientAirTemperature_t	*airTemp	/* OPTIONAL */;
	DSRC_AmbientAirPressure_t	*airPres	/* OPTIONAL */;
	struct DSRC_VehicleStatus__steering {
		DSRC_SteeringWheelAngle_t	 angle;
		DSRC_SteeringWheelAngleConfidence_t	*confidence	/* OPTIONAL */;
		DSRC_SteeringWheelAngleRateOfChange_t	*rate	/* OPTIONAL */;
		DSRC_DrivingWheelAngle_t	*wheels	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *steering;
	struct DSRC_VehicleStatus__accelSets {
		DSRC_AccelerationSet4Way_t	*accel4way	/* OPTIONAL */;
		DSRC_VerticalAccelerationThreshold_t	*vertAccelThres	/* OPTIONAL */;
		DSRC_YawRateConfidence_t	*yawRateCon	/* OPTIONAL */;
		DSRC_AccelerationConfidence_t	*hozAccelCon	/* OPTIONAL */;
		DSRC_ConfidenceSet_t	*confidenceSet	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *accelSets;
	struct DSRC_VehicleStatus__object {
		DSRC_ObstacleDistance_t	 obDist;
		DSRC_ObstacleDirection_t	 obDirect;
		DSRC_DDateTime_t	 dateTime;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *object;
	DSRC_FullPositionVector_t	*fullPos	/* OPTIONAL */;
	DSRC_ThrottlePosition_t	*throttlePos	/* OPTIONAL */;
	DSRC_SpeedandHeadingandThrottleConfidence_t	*speedHeadC	/* OPTIONAL */;
	DSRC_SpeedConfidence_t	*speedC	/* OPTIONAL */;
	struct DSRC_VehicleStatus__vehicleData {
		DSRC_VehicleHeight_t	 height;
		DSRC_BumperHeights_t	 bumpers;
		DSRC_VehicleMass_t	 mass;
		DSRC_TrailerWeight_t	 trailerWeight;
		DSRC_VehicleType_t	 type;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *vehicleData;
	DSRC_VehicleIdent_t	*vehicleIdent	/* OPTIONAL */;
	DSRC_J1939data_t	*j1939data	/* OPTIONAL */;
	struct DSRC_VehicleStatus__weatherReport {
		DSRC_EssPrecipYesNo_t	 isRaining;
		DSRC_EssPrecipRate_t	*rainRate	/* OPTIONAL */;
		DSRC_EssPrecipSituation_t	*precipSituation	/* OPTIONAL */;
		DSRC_EssSolarRadiation_t	*solarRadiation	/* OPTIONAL */;
		DSRC_EssMobileFriction_t	*friction	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *weatherReport;
	DSRC_GPSstatus_t	*gpsStatus	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_VehicleStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_VehicleStatus;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_VehicleStatus_H_ */
#include <asn_internal.h>