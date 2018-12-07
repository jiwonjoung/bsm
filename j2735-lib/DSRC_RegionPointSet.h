/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_RegionPointSet_H_
#define	_DSRC_RegionPointSet_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_Position3D.h"
#include "DSRC_RegionList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_RegionPointSet */
typedef struct DSRC_RegionPointSet {
	DSRC_Position3D_t	*anchor	/* OPTIONAL */;
	DSRC_RegionList_t	 nodeList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_RegionPointSet_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_RegionPointSet;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_RegionPointSet_H_ */
#include <asn_internal.h>