/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_VehicleRequestStatus.h"

int
DSRC_VehicleRequestStatus_constraint(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	size = st->size;
	
	if((size == 1l)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

/*
 * This type is implemented using OCTET_STRING,
 * so here we adjust the DEF accordingly.
 */
static void
DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(asn_TYPE_descriptor_t *td) {
	td->free_struct    = asn_DEF_OCTET_STRING.free_struct;
	td->print_struct   = asn_DEF_OCTET_STRING.print_struct;
	td->check_constraints = asn_DEF_OCTET_STRING.check_constraints;
	td->ber_decoder    = asn_DEF_OCTET_STRING.ber_decoder;
	td->der_encoder    = asn_DEF_OCTET_STRING.der_encoder;
	td->xer_decoder    = asn_DEF_OCTET_STRING.xer_decoder;
	td->xer_encoder    = asn_DEF_OCTET_STRING.xer_encoder;
	td->uper_decoder   = asn_DEF_OCTET_STRING.uper_decoder;
	td->uper_encoder   = asn_DEF_OCTET_STRING.uper_encoder;
	td->aper_decoder   = asn_DEF_OCTET_STRING.aper_decoder;
	td->aper_encoder   = asn_DEF_OCTET_STRING.aper_encoder;
	if(!td->per_constraints)
		td->per_constraints = asn_DEF_OCTET_STRING.per_constraints;
	td->elements       = asn_DEF_OCTET_STRING.elements;
	td->elements_count = asn_DEF_OCTET_STRING.elements_count;
	td->specifics      = asn_DEF_OCTET_STRING.specifics;
}

void
DSRC_VehicleRequestStatus_free(asn_TYPE_descriptor_t *td,
		void *struct_ptr, int contents_only) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	td->free_struct(td, struct_ptr, contents_only);
}

int
DSRC_VehicleRequestStatus_print(asn_TYPE_descriptor_t *td, const void *struct_ptr,
		int ilevel, asn_app_consume_bytes_f *cb, void *app_key) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	return td->print_struct(td, struct_ptr, ilevel, cb, app_key);
}

asn_dec_rval_t
DSRC_VehicleRequestStatus_decode_ber(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const void *bufptr, size_t size, int tag_mode) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	return td->ber_decoder(opt_codec_ctx, td, structure, bufptr, size, tag_mode);
}

asn_enc_rval_t
DSRC_VehicleRequestStatus_encode_der(asn_TYPE_descriptor_t *td,
		void *structure, int tag_mode, ber_tlv_tag_t tag,
		asn_app_consume_bytes_f *cb, void *app_key) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	return td->der_encoder(td, structure, tag_mode, tag, cb, app_key);
}

asn_dec_rval_t
DSRC_VehicleRequestStatus_decode_xer(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const char *opt_mname, const void *bufptr, size_t size) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	return td->xer_decoder(opt_codec_ctx, td, structure, opt_mname, bufptr, size);
}

asn_enc_rval_t
DSRC_VehicleRequestStatus_encode_xer(asn_TYPE_descriptor_t *td, void *structure,
		int ilevel, enum xer_encoder_flags_e flags,
		asn_app_consume_bytes_f *cb, void *app_key) {
	DSRC_VehicleRequestStatus_1_inherit_TYPE_descriptor(td);
	return td->xer_encoder(td, structure, ilevel, flags, cb, app_key);
}

static const ber_tlv_tag_t asn_DEF_DSRC_VehicleRequestStatus_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (4 << 2))
};
asn_TYPE_descriptor_t asn_DEF_DSRC_VehicleRequestStatus = {
	"DSRC_VehicleRequestStatus",
	"DSRC_VehicleRequestStatus",
	DSRC_VehicleRequestStatus_free,
	DSRC_VehicleRequestStatus_print,
	DSRC_VehicleRequestStatus_constraint,
	DSRC_VehicleRequestStatus_decode_ber,
	DSRC_VehicleRequestStatus_encode_der,
	DSRC_VehicleRequestStatus_decode_xer,
	DSRC_VehicleRequestStatus_encode_xer,
	0, 0,	/* No UPER support, use "-gen-PER" to enable */
	0, 0,	/* No APER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_DSRC_VehicleRequestStatus_tags_1,
	sizeof(asn_DEF_DSRC_VehicleRequestStatus_tags_1)
		/sizeof(asn_DEF_DSRC_VehicleRequestStatus_tags_1[0]), /* 1 */
	asn_DEF_DSRC_VehicleRequestStatus_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_VehicleRequestStatus_tags_1)
		/sizeof(asn_DEF_DSRC_VehicleRequestStatus_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	0, 0,	/* No members */
	0	/* No specifics */
};

