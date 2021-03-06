/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_TirePressureThresholdDetection_H_
#define	_DSRC_TirePressureThresholdDetection_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_TirePressureThresholdDetection {
	DSRC_TirePressureThresholdDetection_noData	= 0,
	DSRC_TirePressureThresholdDetection_overPressure	= 1,
	DSRC_TirePressureThresholdDetection_noWarningPressure	= 2,
	DSRC_TirePressureThresholdDetection_underPressure	= 3,
	DSRC_TirePressureThresholdDetection_extremeUnderPressure	= 4,
	DSRC_TirePressureThresholdDetection_undefined	= 5,
	DSRC_TirePressureThresholdDetection_errorIndicator	= 6,
	DSRC_TirePressureThresholdDetection_notAvailable	= 7
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_TirePressureThresholdDetection;

/* DSRC_TirePressureThresholdDetection */
typedef long	 DSRC_TirePressureThresholdDetection_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_TirePressureThresholdDetection;
asn_struct_free_f DSRC_TirePressureThresholdDetection_free;
asn_struct_print_f DSRC_TirePressureThresholdDetection_print;
asn_constr_check_f DSRC_TirePressureThresholdDetection_constraint;
ber_type_decoder_f DSRC_TirePressureThresholdDetection_decode_ber;
der_type_encoder_f DSRC_TirePressureThresholdDetection_encode_der;
xer_type_decoder_f DSRC_TirePressureThresholdDetection_decode_xer;
xer_type_encoder_f DSRC_TirePressureThresholdDetection_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_TirePressureThresholdDetection_H_ */
#include <asn_internal.h>
