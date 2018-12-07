/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_LayerType_H_
#define	_DSRC_LayerType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_LayerType {
	DSRC_LayerType_none	= 0,
	DSRC_LayerType_mixedContent	= 1,
	DSRC_LayerType_generalMapData	= 2,
	DSRC_LayerType_intersectionData	= 3,
	DSRC_LayerType_curveData	= 4,
	DSRC_LayerType_roadwaySectionData	= 5,
	DSRC_LayerType_parkingAreaData	= 6,
	DSRC_LayerType_sharedLaneData	= 7
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_LayerType;

/* DSRC_LayerType */
typedef long	 DSRC_LayerType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_LayerType;
asn_struct_free_f DSRC_LayerType_free;
asn_struct_print_f DSRC_LayerType_print;
asn_constr_check_f DSRC_LayerType_constraint;
ber_type_decoder_f DSRC_LayerType_decode_ber;
der_type_encoder_f DSRC_LayerType_encode_der;
xer_type_decoder_f DSRC_LayerType_decode_xer;
xer_type_encoder_f DSRC_LayerType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_LayerType_H_ */
#include <asn_internal.h>