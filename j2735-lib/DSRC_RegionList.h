/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_RegionList_H_
#define	_DSRC_RegionList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_RegionOffsets.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_RegionList */
typedef struct DSRC_RegionList {
	A_SEQUENCE_OF(DSRC_RegionOffsets_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_RegionList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_RegionList;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_RegionList_H_ */
#include <asn_internal.h>
