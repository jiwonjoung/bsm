/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_SpecialLaneAttributes_H_
#define	_DSRC_SpecialLaneAttributes_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_SpecialLaneAttributes {
	DSRC_SpecialLaneAttributes_noData	= 0,
	DSRC_SpecialLaneAttributes_egressPath	= 1,
	DSRC_SpecialLaneAttributes_railRoadTrack	= 2,
	DSRC_SpecialLaneAttributes_transitOnlyLane	= 4,
	DSRC_SpecialLaneAttributes_hovLane	= 8,
	DSRC_SpecialLaneAttributes_busOnly	= 16,
	DSRC_SpecialLaneAttributes_vehiclesEntering	= 32,
	DSRC_SpecialLaneAttributes_vehiclesLeaving	= 64,
	DSRC_SpecialLaneAttributes_reserved	= 128
} e_DSRC_SpecialLaneAttributes;

/* DSRC_SpecialLaneAttributes */
typedef long	 DSRC_SpecialLaneAttributes_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_SpecialLaneAttributes;
asn_struct_free_f DSRC_SpecialLaneAttributes_free;
asn_struct_print_f DSRC_SpecialLaneAttributes_print;
asn_constr_check_f DSRC_SpecialLaneAttributes_constraint;
ber_type_decoder_f DSRC_SpecialLaneAttributes_decode_ber;
der_type_encoder_f DSRC_SpecialLaneAttributes_encode_der;
xer_type_decoder_f DSRC_SpecialLaneAttributes_decode_xer;
xer_type_encoder_f DSRC_SpecialLaneAttributes_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_SpecialLaneAttributes_H_ */
#include <asn_internal.h>
