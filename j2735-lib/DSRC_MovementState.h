/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_MovementState_H_
#define	_DSRC_MovementState_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_DescriptiveName.h"
#include "DSRC_LaneCount.h"
#include "DSRC_LaneSet.h"
#include "DSRC_SignalLightState.h"
#include "DSRC_PedestrianSignalState.h"
#include "DSRC_SpecialSignalState.h"
#include "DSRC_TimeMark.h"
#include "DSRC_StateConfidence.h"
#include "DSRC_ObjectCount.h"
#include "DSRC_PedestrianDetect.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_MovementState */
typedef struct DSRC_MovementState {
	DSRC_DescriptiveName_t	*movementName	/* OPTIONAL */;
	DSRC_LaneCount_t	*laneCnt	/* OPTIONAL */;
	DSRC_LaneSet_t	 laneSet;
	DSRC_SignalLightState_t	*currState	/* OPTIONAL */;
	DSRC_PedestrianSignalState_t	*pedState	/* OPTIONAL */;
	DSRC_SpecialSignalState_t	*specialState	/* OPTIONAL */;
	DSRC_TimeMark_t	 timeToChange;
	DSRC_StateConfidence_t	*stateConfidence	/* OPTIONAL */;
	DSRC_SignalLightState_t	*yellState	/* OPTIONAL */;
	DSRC_PedestrianSignalState_t	*yellPedState	/* OPTIONAL */;
	DSRC_TimeMark_t	*yellTimeToChange	/* OPTIONAL */;
	DSRC_StateConfidence_t	*yellStateConfidence	/* OPTIONAL */;
	DSRC_ObjectCount_t	*vehicleCount	/* OPTIONAL */;
	DSRC_PedestrianDetect_t	*pedDetect	/* OPTIONAL */;
	DSRC_ObjectCount_t	*pedCount	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_MovementState_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_MovementState;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_MovementState_H_ */
#include <asn_internal.h>
