/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_SignalRequest.h"

static asn_TYPE_member_t asn_MBR_DSRC_SignalRequest_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_SignalRequest, id),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_IntersectionID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"id"
		},
	{ ATF_POINTER, 4, offsetof(struct DSRC_SignalRequest, isCancel),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SignalReqScheme,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"isCancel"
		},
	{ ATF_POINTER, 3, offsetof(struct DSRC_SignalRequest, requestedAction),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SignalReqScheme,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"requestedAction"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_SignalRequest, inLane),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_LaneNumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"inLane"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_SignalRequest, outLane),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_LaneNumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"outLane"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_SignalRequest, type),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_NTCIPVehicleclass,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"type"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_SignalRequest, codeWord),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_CodeWord,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"codeWord"
		},
};
static const ber_tlv_tag_t asn_DEF_DSRC_SignalRequest_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DSRC_SignalRequest_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* id */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* isCancel */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* requestedAction */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* inLane */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* outLane */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* type */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* codeWord */
};
static asn_SEQUENCE_specifics_t asn_SPC_DSRC_SignalRequest_specs_1 = {
	sizeof(struct DSRC_SignalRequest),
	offsetof(struct DSRC_SignalRequest, _asn_ctx),
	asn_MAP_DSRC_SignalRequest_tag2el_1,
	7,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	6,	/* Start extensions */
	8	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DSRC_SignalRequest = {
	"DSRC_SignalRequest",
	"DSRC_SignalRequest",
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
	asn_DEF_DSRC_SignalRequest_tags_1,
	sizeof(asn_DEF_DSRC_SignalRequest_tags_1)
		/sizeof(asn_DEF_DSRC_SignalRequest_tags_1[0]), /* 1 */
	asn_DEF_DSRC_SignalRequest_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_SignalRequest_tags_1)
		/sizeof(asn_DEF_DSRC_SignalRequest_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DSRC_SignalRequest_1,
	7,	/* Elements count */
	&asn_SPC_DSRC_SignalRequest_specs_1	/* Additional specs */
};

