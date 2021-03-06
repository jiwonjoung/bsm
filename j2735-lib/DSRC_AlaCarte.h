/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_AlaCarte_H_
#define	_DSRC_AlaCarte_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_DSRCmsgID.h"
#include "DSRC_AllInclusive.h"
#include "DSRC_MsgCRC.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_AlaCarte */
typedef struct DSRC_AlaCarte {
	DSRC_DSRCmsgID_t	 msgID;
	DSRC_AllInclusive_t	 data;
	DSRC_MsgCRC_t	*crc	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_AlaCarte_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_AlaCarte;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_AlaCarte_H_ */
#include <asn_internal.h>
