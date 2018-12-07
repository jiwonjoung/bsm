/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_WheelSensorStatus_H_
#define	_DSRC_WheelSensorStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_WheelSensorStatus {
	DSRC_WheelSensorStatus_off	= 0,
	DSRC_WheelSensorStatus_on	= 1,
	DSRC_WheelSensorStatus_notDefined	= 2,
	DSRC_WheelSensorStatus_notSupoprted	= 3
} e_DSRC_WheelSensorStatus;

/* DSRC_WheelSensorStatus */
typedef long	 DSRC_WheelSensorStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_WheelSensorStatus;
asn_struct_free_f DSRC_WheelSensorStatus_free;
asn_struct_print_f DSRC_WheelSensorStatus_print;
asn_constr_check_f DSRC_WheelSensorStatus_constraint;
ber_type_decoder_f DSRC_WheelSensorStatus_decode_ber;
der_type_encoder_f DSRC_WheelSensorStatus_encode_der;
xer_type_decoder_f DSRC_WheelSensorStatus_decode_xer;
xer_type_encoder_f DSRC_WheelSensorStatus_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_WheelSensorStatus_H_ */
#include <asn_internal.h>