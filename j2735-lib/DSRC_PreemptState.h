/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_PreemptState_H_
#define	_DSRC_PreemptState_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_PreemptState {
	DSRC_PreemptState_none	= 0,
	DSRC_PreemptState_other	= 1,
	DSRC_PreemptState_notActive	= 2,
	DSRC_PreemptState_notActiveWithCall	= 3,
	DSRC_PreemptState_entryStarted	= 4,
	DSRC_PreemptState_trackService	= 5,
	DSRC_PreemptState_dwell	= 6,
	DSRC_PreemptState_linkActive	= 7,
	DSRC_PreemptState_existStarted	= 8,
	DSRC_PreemptState_maximumPresence	= 9,
	DSRC_PreemptState_ackowledgedButOverridden	= 10
	/*
	 * Enumeration is extensible
	 */
} e_DSRC_PreemptState;

/* DSRC_PreemptState */
typedef long	 DSRC_PreemptState_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_PreemptState;
asn_struct_free_f DSRC_PreemptState_free;
asn_struct_print_f DSRC_PreemptState_print;
asn_constr_check_f DSRC_PreemptState_constraint;
ber_type_decoder_f DSRC_PreemptState_decode_ber;
der_type_encoder_f DSRC_PreemptState_encode_der;
xer_type_decoder_f DSRC_PreemptState_decode_xer;
xer_type_encoder_f DSRC_PreemptState_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_PreemptState_H_ */
#include <asn_internal.h>
