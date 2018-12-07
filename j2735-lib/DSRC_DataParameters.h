/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_DataParameters_H_
#define	_DSRC_DataParameters_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_DataParameters */
typedef struct DSRC_DataParameters {
	IA5String_t	*processMethod	/* OPTIONAL */;
	IA5String_t	*processAgency	/* OPTIONAL */;
	IA5String_t	*lastCheckedDate	/* OPTIONAL */;
	IA5String_t	*geiodUsed	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_DataParameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_DataParameters;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_DataParameters_H_ */
#include <asn_internal.h>
