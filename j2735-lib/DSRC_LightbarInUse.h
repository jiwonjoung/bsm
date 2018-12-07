/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_LightbarInUse_H_
#define	_DSRC_LightbarInUse_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_LightbarInUse {
	DSRC_LightbarInUse_unavailable	= 0,
	DSRC_LightbarInUse_notInUse	= 1,
	DSRC_LightbarInUse_inUse	= 2,
	DSRC_LightbarInUse_sirenInUse	= 3,
	DSRC_LightbarInUse_yellowCautionLights	= 4,
	DSRC_LightbarInUse_schooldBusLights	= 5,
	DSRC_LightbarInUse_arrowSignsActive	= 6,
	DSRC_LightbarInUse_slowMovingVehicle	= 7,
	DSRC_LightbarInUse_freqStops	= 8,
	DSRC_LightbarInUse_reserved	= 9
} e_DSRC_LightbarInUse;

/* DSRC_LightbarInUse */
typedef long	 DSRC_LightbarInUse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_LightbarInUse;
asn_struct_free_f DSRC_LightbarInUse_free;
asn_struct_print_f DSRC_LightbarInUse_print;
asn_constr_check_f DSRC_LightbarInUse_constraint;
ber_type_decoder_f DSRC_LightbarInUse_decode_ber;
der_type_encoder_f DSRC_LightbarInUse_encode_der;
xer_type_decoder_f DSRC_LightbarInUse_decode_xer;
xer_type_encoder_f DSRC_LightbarInUse_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_LightbarInUse_H_ */
#include <asn_internal.h>
