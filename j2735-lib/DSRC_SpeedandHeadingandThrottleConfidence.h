/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_SpeedandHeadingandThrottleConfidence_H_
#define	_DSRC_SpeedandHeadingandThrottleConfidence_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_SpeedandHeadingandThrottleConfidence */
typedef OCTET_STRING_t	 DSRC_SpeedandHeadingandThrottleConfidence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_SpeedandHeadingandThrottleConfidence;
asn_struct_free_f DSRC_SpeedandHeadingandThrottleConfidence_free;
asn_struct_print_f DSRC_SpeedandHeadingandThrottleConfidence_print;
asn_constr_check_f DSRC_SpeedandHeadingandThrottleConfidence_constraint;
ber_type_decoder_f DSRC_SpeedandHeadingandThrottleConfidence_decode_ber;
der_type_encoder_f DSRC_SpeedandHeadingandThrottleConfidence_encode_der;
xer_type_decoder_f DSRC_SpeedandHeadingandThrottleConfidence_decode_xer;
xer_type_encoder_f DSRC_SpeedandHeadingandThrottleConfidence_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_SpeedandHeadingandThrottleConfidence_H_ */
#include <asn_internal.h>