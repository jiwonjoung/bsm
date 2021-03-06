/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_DTime_H_
#define	_DSRC_DTime_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_DHour.h"
#include "DSRC_DMinute.h"
#include "DSRC_DSecond.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_DTime */
typedef struct DSRC_DTime {
	DSRC_DHour_t	 hour;
	DSRC_DMinute_t	 minute;
	DSRC_DSecond_t	 second;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_DTime_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_DTime;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_DTime_H_ */
#include <asn_internal.h>
