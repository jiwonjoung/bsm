/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_RoadSignID.h"

static asn_TYPE_member_t asn_MBR_DSRC_RoadSignID_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_RoadSignID, position),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_Position3D,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"position"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_RoadSignID, viewAngle),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_HeadingSlice,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"viewAngle"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_RoadSignID, mutcdCode),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_MUTCDCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mutcdCode"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_RoadSignID, crc),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_MsgCRC,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"crc"
		},
};
static const ber_tlv_tag_t asn_DEF_DSRC_RoadSignID_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DSRC_RoadSignID_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* position */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* viewAngle */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* mutcdCode */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* crc */
};
static asn_SEQUENCE_specifics_t asn_SPC_DSRC_RoadSignID_specs_1 = {
	sizeof(struct DSRC_RoadSignID),
	offsetof(struct DSRC_RoadSignID, _asn_ctx),
	asn_MAP_DSRC_RoadSignID_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DSRC_RoadSignID = {
	"DSRC_RoadSignID",
	"DSRC_RoadSignID",
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
	asn_DEF_DSRC_RoadSignID_tags_1,
	sizeof(asn_DEF_DSRC_RoadSignID_tags_1)
		/sizeof(asn_DEF_DSRC_RoadSignID_tags_1[0]), /* 1 */
	asn_DEF_DSRC_RoadSignID_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_RoadSignID_tags_1)
		/sizeof(asn_DEF_DSRC_RoadSignID_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DSRC_RoadSignID_1,
	4,	/* Elements count */
	&asn_SPC_DSRC_RoadSignID_specs_1	/* Additional specs */
};

