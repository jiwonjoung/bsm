/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_YawRateConfidence_H_
#define	_DSRC_YawRateConfidence_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_YawRateConfidence {
	DSRC_YawRateConfidence_unavailable	= 0,
	DSRC_YawRateConfidence_degSec_100_00	= 1,
	DSRC_YawRateConfidence_degSec_010_00	= 2,
	DSRC_YawRateConfidence_degSec_005_00	= 3,
	DSRC_YawRateConfidence_degSec_001_00	= 4,
	DSRC_YawRateConfidence_degSec_000_10	= 5,
	DSRC_YawRateConfidence_degSec_000_05	= 6,
	DSRC_YawRateConfidence_degSec_000_01	= 7
} e_DSRC_YawRateConfidence;

/* DSRC_YawRateConfidence */
typedef long	 DSRC_YawRateConfidence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_YawRateConfidence;
asn_struct_free_f DSRC_YawRateConfidence_free;
asn_struct_print_f DSRC_YawRateConfidence_print;
asn_constr_check_f DSRC_YawRateConfidence_constraint;
ber_type_decoder_f DSRC_YawRateConfidence_decode_ber;
der_type_encoder_f DSRC_YawRateConfidence_encode_der;
xer_type_decoder_f DSRC_YawRateConfidence_decode_xer;
xer_type_encoder_f DSRC_YawRateConfidence_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_YawRateConfidence_H_ */
#include <asn_internal.h>
