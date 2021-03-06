/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_PathHistoryPointType_01_H_
#define	_DSRC_PathHistoryPointType_01_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "DSRC_PositionalAccuracy.h"
#include "DSRC_TransmissionAndSpeed.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_PathHistoryPointType-01 */
typedef struct DSRC_PathHistoryPointType_01 {
	long	 latOffset;
	long	 longOffset;
	long	*elevationOffset	/* OPTIONAL */;
	long	*timeOffset	/* OPTIONAL */;
	DSRC_PositionalAccuracy_t	*posAccuracy	/* OPTIONAL */;
	long	*heading	/* OPTIONAL */;
	DSRC_TransmissionAndSpeed_t	*speed	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_PathHistoryPointType_01_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_PathHistoryPointType_01;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_PathHistoryPointType_01_H_ */
#include <asn_internal.h>
