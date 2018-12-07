/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_SignalRequest_H_
#define	_DSRC_SignalRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_IntersectionID.h"
#include "DSRC_SignalReqScheme.h"
#include "DSRC_LaneNumber.h"
#include "DSRC_NTCIPVehicleclass.h"
#include "DSRC_CodeWord.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_SignalRequest */
typedef struct DSRC_SignalRequest {
	DSRC_IntersectionID_t	 id;
	DSRC_SignalReqScheme_t	*isCancel	/* OPTIONAL */;
	DSRC_SignalReqScheme_t	*requestedAction	/* OPTIONAL */;
	DSRC_LaneNumber_t	*inLane	/* OPTIONAL */;
	DSRC_LaneNumber_t	*outLane	/* OPTIONAL */;
	DSRC_NTCIPVehicleclass_t	 type;
	DSRC_CodeWord_t	*codeWord	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_SignalRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_SignalRequest;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_SignalRequest_H_ */
#include <asn_internal.h>
