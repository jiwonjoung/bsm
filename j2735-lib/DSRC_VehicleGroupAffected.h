/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITIS"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_VehicleGroupAffected_H_
#define	_DSRC_VehicleGroupAffected_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_VehicleGroupAffected {
	DSRC_VehicleGroupAffected_all_vehicles	= 9217,
	DSRC_VehicleGroupAffected_bicycles	= 9218,
	DSRC_VehicleGroupAffected_motorcycles	= 9219,
	DSRC_VehicleGroupAffected_cars	= 9220,
	DSRC_VehicleGroupAffected_light_vehicles	= 9221,
	DSRC_VehicleGroupAffected_cars_and_light_vehicles	= 9222,
	DSRC_VehicleGroupAffected_cars_with_trailers	= 9223,
	DSRC_VehicleGroupAffected_cars_with_recreational_trailers	= 9224,
	DSRC_VehicleGroupAffected_vehicles_with_trailers	= 9225,
	DSRC_VehicleGroupAffected_heavy_vehicles	= 9226,
	DSRC_VehicleGroupAffected_trucks	= 9227,
	DSRC_VehicleGroupAffected_buses	= 9228,
	DSRC_VehicleGroupAffected_articulated_buses	= 9229,
	DSRC_VehicleGroupAffected_school_buses	= 9230,
	DSRC_VehicleGroupAffected_vehicles_with_semi_trailers	= 9231,
	DSRC_VehicleGroupAffected_vehicles_with_double_trailers	= 9232,
	DSRC_VehicleGroupAffected_high_profile_vehicles	= 9233,
	DSRC_VehicleGroupAffected_wide_vehicles	= 9234,
	DSRC_VehicleGroupAffected_long_vehicles	= 9235,
	DSRC_VehicleGroupAffected_hazardous_loads	= 9236,
	DSRC_VehicleGroupAffected_exceptional_loads	= 9237,
	DSRC_VehicleGroupAffected_abnormal_loads	= 9238,
	DSRC_VehicleGroupAffected_convoys	= 9239,
	DSRC_VehicleGroupAffected_maintenance_vehicles	= 9240,
	DSRC_VehicleGroupAffected_delivery_vehicles	= 9241,
	DSRC_VehicleGroupAffected_vehicles_with_even_numbered_license_plates	= 9242,
	DSRC_VehicleGroupAffected_vehicles_with_odd_numbered_license_plates	= 9243,
	DSRC_VehicleGroupAffected_vehicles_with_parking_permits	= 9244,
	DSRC_VehicleGroupAffected_vehicles_with_catalytic_converters	= 9245,
	DSRC_VehicleGroupAffected_vehicles_without_catalytic_converters	= 9246,
	DSRC_VehicleGroupAffected_gas_powered_vehicles	= 9247,
	DSRC_VehicleGroupAffected_diesel_powered_vehicles	= 9248,
	DSRC_VehicleGroupAffected_lPG_vehicles	= 9249,
	DSRC_VehicleGroupAffected_military_convoys	= 9250,
	DSRC_VehicleGroupAffected_military_vehicles	= 9251
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_VehicleGroupAffected;

/* DSRC_VehicleGroupAffected */
typedef long	 DSRC_VehicleGroupAffected_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_VehicleGroupAffected;
asn_struct_free_f DSRC_VehicleGroupAffected_free;
asn_struct_print_f DSRC_VehicleGroupAffected_print;
asn_constr_check_f DSRC_VehicleGroupAffected_constraint;
ber_type_decoder_f DSRC_VehicleGroupAffected_decode_ber;
der_type_encoder_f DSRC_VehicleGroupAffected_encode_der;
xer_type_decoder_f DSRC_VehicleGroupAffected_decode_xer;
xer_type_encoder_f DSRC_VehicleGroupAffected_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_VehicleGroupAffected_H_ */
#include <asn_internal.h>
