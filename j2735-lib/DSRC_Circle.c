/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_Circle.h"

static int
memb_radiusSteps_constraint_3(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0l && value <= 32767l)) {
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
memb_miles_constraint_3(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1l && value <= 2000l)) {
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
memb_km_constraint_3(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1l && value <= 5000l)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_TYPE_member_t asn_MBR_raduis_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_Circle__raduis, choice.radiusSteps),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_radiusSteps_constraint_3,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"radiusSteps"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_Circle__raduis, choice.miles),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_miles_constraint_3,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"miles"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_Circle__raduis, choice.km),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_km_constraint_3,
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"km"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_raduis_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* radiusSteps */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* miles */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* km */
};
static asn_CHOICE_specifics_t asn_SPC_raduis_specs_3 = {
	sizeof(struct DSRC_Circle__raduis),
	offsetof(struct DSRC_Circle__raduis, _asn_ctx),
	offsetof(struct DSRC_Circle__raduis, present),
	sizeof(((struct DSRC_Circle__raduis *)0)->present),
	asn_MAP_raduis_tag2el_3,
	3,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_raduis_3 = {
	"raduis",
	"raduis",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No UPER support, use "-gen-PER" to enable */
	0, 0,	/* No APER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_raduis_3,
	3,	/* Elements count */
	&asn_SPC_raduis_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_DSRC_Circle_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_Circle, center),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_Position3D,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"center"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_Circle, raduis),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_raduis_3,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"raduis"
		},
};
static const ber_tlv_tag_t asn_DEF_DSRC_Circle_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DSRC_Circle_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* center */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* raduis */
};
static asn_SEQUENCE_specifics_t asn_SPC_DSRC_Circle_specs_1 = {
	sizeof(struct DSRC_Circle),
	offsetof(struct DSRC_Circle, _asn_ctx),
	asn_MAP_DSRC_Circle_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DSRC_Circle = {
	"DSRC_Circle",
	"DSRC_Circle",
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
	asn_DEF_DSRC_Circle_tags_1,
	sizeof(asn_DEF_DSRC_Circle_tags_1)
		/sizeof(asn_DEF_DSRC_Circle_tags_1[0]), /* 1 */
	asn_DEF_DSRC_Circle_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_Circle_tags_1)
		/sizeof(asn_DEF_DSRC_Circle_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DSRC_Circle_1,
	2,	/* Elements count */
	&asn_SPC_DSRC_Circle_specs_1	/* Additional specs */
};

