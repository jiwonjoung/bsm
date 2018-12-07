/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_LaneManeuverCode_H_
#define	_DSRC_LaneManeuverCode_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_LaneManeuverCode {
	DSRC_LaneManeuverCode_unknown	= 0,
	DSRC_LaneManeuverCode_uTurn	= 1,
	DSRC_LaneManeuverCode_leftTurn	= 2,
	DSRC_LaneManeuverCode_rightTurn	= 3,
	DSRC_LaneManeuverCode_straightAhead	= 4,
	DSRC_LaneManeuverCode_softLeftTurn	= 5,
	DSRC_LaneManeuverCode_softRightTurn	= 6
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_LaneManeuverCode;

/* DSRC_LaneManeuverCode */
typedef long	 DSRC_LaneManeuverCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_LaneManeuverCode;
asn_struct_free_f DSRC_LaneManeuverCode_free;
asn_struct_print_f DSRC_LaneManeuverCode_print;
asn_constr_check_f DSRC_LaneManeuverCode_constraint;
ber_type_decoder_f DSRC_LaneManeuverCode_decode_ber;
der_type_encoder_f DSRC_LaneManeuverCode_encode_der;
xer_type_decoder_f DSRC_LaneManeuverCode_decode_xer;
xer_type_encoder_f DSRC_LaneManeuverCode_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_LaneManeuverCode_H_ */
#include <asn_internal.h>
