/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_MultiVehicleResponse_H_
#define	_DSRC_MultiVehicleResponse_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_MultiVehicleResponse {
	DSRC_MultiVehicleResponse_unavailable	= 0,
	DSRC_MultiVehicleResponse_singleVehicle	= 1,
	DSRC_MultiVehicleResponse_multiVehicle	= 2,
	DSRC_MultiVehicleResponse_reserved	= 3
} e_DSRC_MultiVehicleResponse;

/* DSRC_MultiVehicleResponse */
typedef long	 DSRC_MultiVehicleResponse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_MultiVehicleResponse;
asn_struct_free_f DSRC_MultiVehicleResponse_free;
asn_struct_print_f DSRC_MultiVehicleResponse_print;
asn_constr_check_f DSRC_MultiVehicleResponse_constraint;
ber_type_decoder_f DSRC_MultiVehicleResponse_decode_ber;
der_type_encoder_f DSRC_MultiVehicleResponse_encode_der;
xer_type_decoder_f DSRC_MultiVehicleResponse_decode_xer;
xer_type_encoder_f DSRC_MultiVehicleResponse_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_MultiVehicleResponse_H_ */
#include <asn_internal.h>
