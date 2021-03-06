/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_Position3D_H_
#define	_DSRC_Position3D_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_Latitude.h"
#include "DSRC_Longitude.h"
#include "DSRC_Elevation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_Position3D */
typedef struct DSRC_Position3D {
	DSRC_Latitude_t	 lat;
	DSRC_Longitude_t	 Long;
	DSRC_Elevation_t	*elevation	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_Position3D_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_Position3D;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_Position3D_H_ */
#include <asn_internal.h>
