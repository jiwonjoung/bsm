/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_WiperStatusFront_H_
#define	_DSRC_WiperStatusFront_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_WiperStatusFront {
	DSRC_WiperStatusFront_unavailable	= 0,
	DSRC_WiperStatusFront_off	= 1,
	DSRC_WiperStatusFront_intermittent	= 2,
	DSRC_WiperStatusFront_low	= 3,
	DSRC_WiperStatusFront_high	= 4,
	DSRC_WiperStatusFront_washerInUse	= 126,
	DSRC_WiperStatusFront_automaticPresent	= 127
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_WiperStatusFront;

/* DSRC_WiperStatusFront */
typedef long	 DSRC_WiperStatusFront_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_WiperStatusFront;
asn_struct_free_f DSRC_WiperStatusFront_free;
asn_struct_print_f DSRC_WiperStatusFront_print;
asn_constr_check_f DSRC_WiperStatusFront_constraint;
ber_type_decoder_f DSRC_WiperStatusFront_decode_ber;
der_type_encoder_f DSRC_WiperStatusFront_encode_der;
xer_type_decoder_f DSRC_WiperStatusFront_decode_xer;
xer_type_encoder_f DSRC_WiperStatusFront_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_WiperStatusFront_H_ */
#include <asn_internal.h>
