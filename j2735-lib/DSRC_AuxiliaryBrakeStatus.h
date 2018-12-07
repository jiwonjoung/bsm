/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_AuxiliaryBrakeStatus_H_
#define	_DSRC_AuxiliaryBrakeStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_AuxiliaryBrakeStatus {
	DSRC_AuxiliaryBrakeStatus_unavailable	= 0,
	DSRC_AuxiliaryBrakeStatus_off	= 1,
	DSRC_AuxiliaryBrakeStatus_on	= 2,
	DSRC_AuxiliaryBrakeStatus_reserved	= 3
} e_DSRC_AuxiliaryBrakeStatus;

/* DSRC_AuxiliaryBrakeStatus */
typedef long	 DSRC_AuxiliaryBrakeStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_AuxiliaryBrakeStatus;
asn_struct_free_f DSRC_AuxiliaryBrakeStatus_free;
asn_struct_print_f DSRC_AuxiliaryBrakeStatus_print;
asn_constr_check_f DSRC_AuxiliaryBrakeStatus_constraint;
ber_type_decoder_f DSRC_AuxiliaryBrakeStatus_decode_ber;
der_type_encoder_f DSRC_AuxiliaryBrakeStatus_encode_der;
xer_type_decoder_f DSRC_AuxiliaryBrakeStatus_decode_xer;
xer_type_encoder_f DSRC_AuxiliaryBrakeStatus_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_AuxiliaryBrakeStatus_H_ */
#include <asn_internal.h>
