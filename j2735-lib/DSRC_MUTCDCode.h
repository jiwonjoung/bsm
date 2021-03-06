/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_MUTCDCode_H_
#define	_DSRC_MUTCDCode_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_MUTCDCode {
	DSRC_MUTCDCode_none	= 0,
	DSRC_MUTCDCode_regulatory	= 1,
	DSRC_MUTCDCode_warning	= 2,
	DSRC_MUTCDCode_maintenance	= 3,
	DSRC_MUTCDCode_motoristService	= 4,
	DSRC_MUTCDCode_guide	= 5,
	DSRC_MUTCDCode_rec	= 6
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_MUTCDCode;

/* DSRC_MUTCDCode */
typedef long	 DSRC_MUTCDCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_MUTCDCode;
asn_struct_free_f DSRC_MUTCDCode_free;
asn_struct_print_f DSRC_MUTCDCode_print;
asn_constr_check_f DSRC_MUTCDCode_constraint;
ber_type_decoder_f DSRC_MUTCDCode_decode_ber;
der_type_encoder_f DSRC_MUTCDCode_encode_der;
xer_type_decoder_f DSRC_MUTCDCode_decode_xer;
xer_type_encoder_f DSRC_MUTCDCode_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_MUTCDCode_H_ */
#include <asn_internal.h>
