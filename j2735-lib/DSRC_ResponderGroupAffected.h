/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITIS"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_ResponderGroupAffected_H_
#define	_DSRC_ResponderGroupAffected_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_ResponderGroupAffected {
	DSRC_ResponderGroupAffected_emergency_vehicle_units	= 9729,
	DSRC_ResponderGroupAffected_federal_law_enforcement_units	= 9730,
	DSRC_ResponderGroupAffected_state_police_units	= 9731,
	DSRC_ResponderGroupAffected_county_police_units	= 9732,
	DSRC_ResponderGroupAffected_local_police_units	= 9733,
	DSRC_ResponderGroupAffected_ambulance_units	= 9734,
	DSRC_ResponderGroupAffected_rescue_units	= 9735,
	DSRC_ResponderGroupAffected_fire_units	= 9736,
	DSRC_ResponderGroupAffected_hAZMAT_units	= 9737,
	DSRC_ResponderGroupAffected_light_tow_unit	= 9738,
	DSRC_ResponderGroupAffected_heavy_tow_unit	= 9739,
	DSRC_ResponderGroupAffected_freeway_service_patrols	= 9740,
	DSRC_ResponderGroupAffected_transportation_response_units	= 9741,
	DSRC_ResponderGroupAffected_private_contractor_response_units	= 9742
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_ResponderGroupAffected;

/* DSRC_ResponderGroupAffected */
typedef long	 DSRC_ResponderGroupAffected_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_ResponderGroupAffected;
asn_struct_free_f DSRC_ResponderGroupAffected_free;
asn_struct_print_f DSRC_ResponderGroupAffected_print;
asn_constr_check_f DSRC_ResponderGroupAffected_constraint;
ber_type_decoder_f DSRC_ResponderGroupAffected_decode_ber;
der_type_encoder_f DSRC_ResponderGroupAffected_encode_der;
xer_type_decoder_f DSRC_ResponderGroupAffected_decode_xer;
xer_type_encoder_f DSRC_ResponderGroupAffected_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_ResponderGroupAffected_H_ */
#include <asn_internal.h>
