/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_WiperStatus_H_
#define	_DSRC_WiperStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_WiperStatusFront.h"
#include "DSRC_WiperRate.h"
#include "DSRC_WiperStatusRear.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_WiperStatus */
typedef struct DSRC_WiperStatus {
	DSRC_WiperStatusFront_t	 statusFront;
	DSRC_WiperRate_t	 rateFront;
	DSRC_WiperStatusRear_t	*statusRear	/* OPTIONAL */;
	DSRC_WiperRate_t	*rateRear	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_WiperStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_WiperStatus;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_WiperStatus_H_ */
#include <asn_internal.h>
