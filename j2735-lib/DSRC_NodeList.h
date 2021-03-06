/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_NodeList_H_
#define	_DSRC_NodeList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_Offsets.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_NodeList */
typedef struct DSRC_NodeList {
	A_SEQUENCE_OF(DSRC_Offsets_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_NodeList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_NodeList;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_NodeList_H_ */
#include <asn_internal.h>
