/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_URL_Base_H_
#define	_DSRC_URL_Base_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_URL-Base */
typedef IA5String_t	 DSRC_URL_Base_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_URL_Base;
asn_struct_free_f DSRC_URL_Base_free;
asn_struct_print_f DSRC_URL_Base_print;
asn_constr_check_f DSRC_URL_Base_constraint;
ber_type_decoder_f DSRC_URL_Base_decode_ber;
der_type_encoder_f DSRC_URL_Base_encode_der;
xer_type_decoder_f DSRC_URL_Base_decode_xer;
xer_type_encoder_f DSRC_URL_Base_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_URL_Base_H_ */
#include <asn_internal.h>
