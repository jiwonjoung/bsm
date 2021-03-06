/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#include "DSRC_VehicleStatus.h"

static asn_TYPE_member_t asn_MBR_wipers_4[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__wipers, statusFront),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_WiperStatusFront,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"statusFront"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__wipers, rateFront),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_WiperRate,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"rateFront"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_VehicleStatus__wipers, statusRear),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_WiperStatusRear,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"statusRear"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_VehicleStatus__wipers, rateRear),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_WiperRate,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"rateRear"
		},
};
static const ber_tlv_tag_t asn_DEF_wipers_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_wipers_tag2el_4[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* statusFront */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rateFront */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* statusRear */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* rateRear */
};
static asn_SEQUENCE_specifics_t asn_SPC_wipers_specs_4 = {
	sizeof(struct DSRC_VehicleStatus__wipers),
	offsetof(struct DSRC_VehicleStatus__wipers, _asn_ctx),
	asn_MAP_wipers_tag2el_4,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_wipers_4 = {
	"wipers",
	"wipers",
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
	asn_DEF_wipers_tags_4,
	sizeof(asn_DEF_wipers_tags_4)
		/sizeof(asn_DEF_wipers_tags_4[0]) - 1, /* 1 */
	asn_DEF_wipers_tags_4,	/* Same as above */
	sizeof(asn_DEF_wipers_tags_4)
		/sizeof(asn_DEF_wipers_tags_4[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_wipers_4,
	4,	/* Elements count */
	&asn_SPC_wipers_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_steering_16[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__steering, angle),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SteeringWheelAngle,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"angle"
		},
	{ ATF_POINTER, 3, offsetof(struct DSRC_VehicleStatus__steering, confidence),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SteeringWheelAngleConfidence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"confidence"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_VehicleStatus__steering, rate),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SteeringWheelAngleRateOfChange,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"rate"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_VehicleStatus__steering, wheels),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_DrivingWheelAngle,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"wheels"
		},
};
static const ber_tlv_tag_t asn_DEF_steering_tags_16[] = {
	(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_steering_tag2el_16[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* angle */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* confidence */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rate */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* wheels */
};
static asn_SEQUENCE_specifics_t asn_SPC_steering_specs_16 = {
	sizeof(struct DSRC_VehicleStatus__steering),
	offsetof(struct DSRC_VehicleStatus__steering, _asn_ctx),
	asn_MAP_steering_tag2el_16,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_steering_16 = {
	"steering",
	"steering",
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
	asn_DEF_steering_tags_16,
	sizeof(asn_DEF_steering_tags_16)
		/sizeof(asn_DEF_steering_tags_16[0]) - 1, /* 1 */
	asn_DEF_steering_tags_16,	/* Same as above */
	sizeof(asn_DEF_steering_tags_16)
		/sizeof(asn_DEF_steering_tags_16[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_steering_16,
	4,	/* Elements count */
	&asn_SPC_steering_specs_16	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_accelSets_21[] = {
	{ ATF_POINTER, 5, offsetof(struct DSRC_VehicleStatus__accelSets, accel4way),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_AccelerationSet4Way,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accel4way"
		},
	{ ATF_POINTER, 4, offsetof(struct DSRC_VehicleStatus__accelSets, vertAccelThres),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_VerticalAccelerationThreshold,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vertAccelThres"
		},
	{ ATF_POINTER, 3, offsetof(struct DSRC_VehicleStatus__accelSets, yawRateCon),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_YawRateConfidence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"yawRateCon"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_VehicleStatus__accelSets, hozAccelCon),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_AccelerationConfidence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"hozAccelCon"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_VehicleStatus__accelSets, confidenceSet),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_ConfidenceSet,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"confidenceSet"
		},
};
static const ber_tlv_tag_t asn_DEF_accelSets_tags_21[] = {
	(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_accelSets_tag2el_21[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* accel4way */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* vertAccelThres */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* yawRateCon */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* hozAccelCon */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* confidenceSet */
};
static asn_SEQUENCE_specifics_t asn_SPC_accelSets_specs_21 = {
	sizeof(struct DSRC_VehicleStatus__accelSets),
	offsetof(struct DSRC_VehicleStatus__accelSets, _asn_ctx),
	asn_MAP_accelSets_tag2el_21,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_accelSets_21 = {
	"accelSets",
	"accelSets",
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
	asn_DEF_accelSets_tags_21,
	sizeof(asn_DEF_accelSets_tags_21)
		/sizeof(asn_DEF_accelSets_tags_21[0]) - 1, /* 1 */
	asn_DEF_accelSets_tags_21,	/* Same as above */
	sizeof(asn_DEF_accelSets_tags_21)
		/sizeof(asn_DEF_accelSets_tags_21[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_accelSets_21,
	5,	/* Elements count */
	&asn_SPC_accelSets_specs_21	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_object_27[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__object, obDist),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_ObstacleDistance,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"obDist"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__object, obDirect),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_ObstacleDirection,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"obDirect"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__object, dateTime),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_DDateTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"dateTime"
		},
};
static const ber_tlv_tag_t asn_DEF_object_tags_27[] = {
	(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_object_tag2el_27[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* obDist */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* obDirect */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* dateTime */
};
static asn_SEQUENCE_specifics_t asn_SPC_object_specs_27 = {
	sizeof(struct DSRC_VehicleStatus__object),
	offsetof(struct DSRC_VehicleStatus__object, _asn_ctx),
	asn_MAP_object_tag2el_27,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_object_27 = {
	"object",
	"object",
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
	asn_DEF_object_tags_27,
	sizeof(asn_DEF_object_tags_27)
		/sizeof(asn_DEF_object_tags_27[0]) - 1, /* 1 */
	asn_DEF_object_tags_27,	/* Same as above */
	sizeof(asn_DEF_object_tags_27)
		/sizeof(asn_DEF_object_tags_27[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_object_27,
	3,	/* Elements count */
	&asn_SPC_object_specs_27	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_vehicleData_35[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__vehicleData, height),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_VehicleHeight,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"height"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__vehicleData, bumpers),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_BumperHeights,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bumpers"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__vehicleData, mass),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_VehicleMass,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mass"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__vehicleData, trailerWeight),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_TrailerWeight,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"trailerWeight"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__vehicleData, type),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_VehicleType,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"type"
		},
};
static const ber_tlv_tag_t asn_DEF_vehicleData_tags_35[] = {
	(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_vehicleData_tag2el_35[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* height */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* bumpers */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* mass */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* trailerWeight */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* type */
};
static asn_SEQUENCE_specifics_t asn_SPC_vehicleData_specs_35 = {
	sizeof(struct DSRC_VehicleStatus__vehicleData),
	offsetof(struct DSRC_VehicleStatus__vehicleData, _asn_ctx),
	asn_MAP_vehicleData_tag2el_35,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_vehicleData_35 = {
	"vehicleData",
	"vehicleData",
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
	asn_DEF_vehicleData_tags_35,
	sizeof(asn_DEF_vehicleData_tags_35)
		/sizeof(asn_DEF_vehicleData_tags_35[0]) - 1, /* 1 */
	asn_DEF_vehicleData_tags_35,	/* Same as above */
	sizeof(asn_DEF_vehicleData_tags_35)
		/sizeof(asn_DEF_vehicleData_tags_35[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_vehicleData_35,
	5,	/* Elements count */
	&asn_SPC_vehicleData_specs_35	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_weatherReport_43[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DSRC_VehicleStatus__weatherReport, isRaining),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_EssPrecipYesNo,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"isRaining"
		},
	{ ATF_POINTER, 4, offsetof(struct DSRC_VehicleStatus__weatherReport, rainRate),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_EssPrecipRate,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"rainRate"
		},
	{ ATF_POINTER, 3, offsetof(struct DSRC_VehicleStatus__weatherReport, precipSituation),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_EssPrecipSituation,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"precipSituation"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_VehicleStatus__weatherReport, solarRadiation),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_EssSolarRadiation,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"solarRadiation"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_VehicleStatus__weatherReport, friction),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_EssMobileFriction,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"friction"
		},
};
static const ber_tlv_tag_t asn_DEF_weatherReport_tags_43[] = {
	(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_weatherReport_tag2el_43[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* isRaining */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* rainRate */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* precipSituation */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* solarRadiation */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* friction */
};
static asn_SEQUENCE_specifics_t asn_SPC_weatherReport_specs_43 = {
	sizeof(struct DSRC_VehicleStatus__weatherReport),
	offsetof(struct DSRC_VehicleStatus__weatherReport, _asn_ctx),
	asn_MAP_weatherReport_tag2el_43,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_weatherReport_43 = {
	"weatherReport",
	"weatherReport",
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
	asn_DEF_weatherReport_tags_43,
	sizeof(asn_DEF_weatherReport_tags_43)
		/sizeof(asn_DEF_weatherReport_tags_43[0]) - 1, /* 1 */
	asn_DEF_weatherReport_tags_43,	/* Same as above */
	sizeof(asn_DEF_weatherReport_tags_43)
		/sizeof(asn_DEF_weatherReport_tags_43[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_weatherReport_43,
	5,	/* Elements count */
	&asn_SPC_weatherReport_specs_43	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_DSRC_VehicleStatus_1[] = {
	{ ATF_POINTER, 22, offsetof(struct DSRC_VehicleStatus, lights),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_ExteriorLights,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lights"
		},
	{ ATF_POINTER, 21, offsetof(struct DSRC_VehicleStatus, lightBar),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_LightbarInUse,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lightBar"
		},
	{ ATF_POINTER, 20, offsetof(struct DSRC_VehicleStatus, wipers),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_wipers_4,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"wipers"
		},
	{ ATF_POINTER, 19, offsetof(struct DSRC_VehicleStatus, brakeStatus),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_BrakeSystemStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"brakeStatus"
		},
	{ ATF_POINTER, 18, offsetof(struct DSRC_VehicleStatus, brakePressure),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_BrakeAppliedPressure,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"brakePressure"
		},
	{ ATF_POINTER, 17, offsetof(struct DSRC_VehicleStatus, roadFriction),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_CoefficientOfFriction,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"roadFriction"
		},
	{ ATF_POINTER, 16, offsetof(struct DSRC_VehicleStatus, sunData),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SunSensor,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sunData"
		},
	{ ATF_POINTER, 15, offsetof(struct DSRC_VehicleStatus, rainData),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_RainSensor,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"rainData"
		},
	{ ATF_POINTER, 14, offsetof(struct DSRC_VehicleStatus, airTemp),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_AmbientAirTemperature,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"airTemp"
		},
	{ ATF_POINTER, 13, offsetof(struct DSRC_VehicleStatus, airPres),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_AmbientAirPressure,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"airPres"
		},
	{ ATF_POINTER, 12, offsetof(struct DSRC_VehicleStatus, steering),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		0,
		&asn_DEF_steering_16,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"steering"
		},
	{ ATF_POINTER, 11, offsetof(struct DSRC_VehicleStatus, accelSets),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		0,
		&asn_DEF_accelSets_21,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accelSets"
		},
	{ ATF_POINTER, 10, offsetof(struct DSRC_VehicleStatus, object),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		0,
		&asn_DEF_object_27,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"object"
		},
	{ ATF_POINTER, 9, offsetof(struct DSRC_VehicleStatus, fullPos),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_FullPositionVector,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"fullPos"
		},
	{ ATF_POINTER, 8, offsetof(struct DSRC_VehicleStatus, throttlePos),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_ThrottlePosition,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"throttlePos"
		},
	{ ATF_POINTER, 7, offsetof(struct DSRC_VehicleStatus, speedHeadC),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SpeedandHeadingandThrottleConfidence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"speedHeadC"
		},
	{ ATF_POINTER, 6, offsetof(struct DSRC_VehicleStatus, speedC),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_SpeedConfidence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"speedC"
		},
	{ ATF_POINTER, 5, offsetof(struct DSRC_VehicleStatus, vehicleData),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		0,
		&asn_DEF_vehicleData_35,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vehicleData"
		},
	{ ATF_POINTER, 4, offsetof(struct DSRC_VehicleStatus, vehicleIdent),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_VehicleIdent,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vehicleIdent"
		},
	{ ATF_POINTER, 3, offsetof(struct DSRC_VehicleStatus, j1939data),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_J1939data,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"j1939data"
		},
	{ ATF_POINTER, 2, offsetof(struct DSRC_VehicleStatus, weatherReport),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		0,
		&asn_DEF_weatherReport_43,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"weatherReport"
		},
	{ ATF_POINTER, 1, offsetof(struct DSRC_VehicleStatus, gpsStatus),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DSRC_GPSstatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"gpsStatus"
		},
};
static const ber_tlv_tag_t asn_DEF_DSRC_VehicleStatus_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DSRC_VehicleStatus_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* lights */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* lightBar */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* wipers */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* brakeStatus */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* brakePressure */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* roadFriction */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* sunData */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* rainData */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* airTemp */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* airPres */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* steering */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* accelSets */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* object */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* fullPos */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* throttlePos */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* speedHeadC */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* speedC */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* vehicleData */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* vehicleIdent */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* j1939data */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* weatherReport */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 } /* gpsStatus */
};
static asn_SEQUENCE_specifics_t asn_SPC_DSRC_VehicleStatus_specs_1 = {
	sizeof(struct DSRC_VehicleStatus),
	offsetof(struct DSRC_VehicleStatus, _asn_ctx),
	asn_MAP_DSRC_VehicleStatus_tag2el_1,
	22,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	21,	/* Start extensions */
	23	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DSRC_VehicleStatus = {
	"DSRC_VehicleStatus",
	"DSRC_VehicleStatus",
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
	asn_DEF_DSRC_VehicleStatus_tags_1,
	sizeof(asn_DEF_DSRC_VehicleStatus_tags_1)
		/sizeof(asn_DEF_DSRC_VehicleStatus_tags_1[0]), /* 1 */
	asn_DEF_DSRC_VehicleStatus_tags_1,	/* Same as above */
	sizeof(asn_DEF_DSRC_VehicleStatus_tags_1)
		/sizeof(asn_DEF_DSRC_VehicleStatus_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DSRC_VehicleStatus_1,
	22,	/* Elements count */
	&asn_SPC_DSRC_VehicleStatus_specs_1	/* Additional specs */
};

