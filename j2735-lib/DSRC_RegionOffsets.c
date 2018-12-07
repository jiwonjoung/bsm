/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_RegionOffsets.h"

static int
memb_xOffset_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -32767ull && value <= 32767l)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_yOffset_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -32767ull && value <= 32767l)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_zOffset_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -32767ull && value <= 32767l)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_TYPE_member_t asn_MBR_DSRC_RegionOffsets_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_RegionOffsets, xOffset),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_xOffset_constraint_1,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"xOffset"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_RegionOffsets, yOffset),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_yOffset_constraint_1,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"yOffset"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_RegionOffsets, zOffset),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_zOffset_constraint_1,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"zOffset"
		},
};
static const ber_tlv_tag_t asn_DEF_DSRC_RegionOffsets_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DSRC_RegionOffsets_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* xOffset */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* yOffset */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* zOffset */
};
static asn_SEQUENCE_specifics_t asn_SPC_DSRC_RegionOffsets_specs_1 = {
	sizeof(struct DSRC_RegionOffsets),
	offsetof(struct DSRC_RegionOffsets, _asn_ctx),
	asn_MAP_DSRC_RegionOffsets_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DSRC_RegionOffsets = {
	"DSRC_RegionOffsets",
	"DSRC_RegionOffsets",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No UPER support, use "-gen-PER" to enable */
	0, 0,	/* No APER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_DSRC_RegionOffsets_tags_1,
	sizeof(asn_DEF_DSRC_RegionOffsets_tags_1)
		/sizeof(asn_DEF_DSRC_RegionOffsets_tags_1[0]), /* 1 */
	asn_DEF_DSRC_RegionOffsets_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_RegionOffsets_tags_1)
		/sizeof(asn_DEF_DSRC_RegionOffsets_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DSRC_RegionOffsets_1,
	3,	/* Elements count */
	&asn_SPC_DSRC_RegionOffsets_specs_1	/* Additional specs */
};

