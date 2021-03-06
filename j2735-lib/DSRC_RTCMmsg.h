/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_RTCMmsg_H_
#define	_DSRC_RTCMmsg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_RTCM-Revision.h"
#include "DSRC_RTCM-ID.h"
#include "DSRC_RTCM-Payload.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DSRC_RTCMmsg */
typedef struct DSRC_RTCMmsg {
	DSRC_RTCM_Revision_t	*rev	/* OPTIONAL */;
	DSRC_RTCM_ID_t	*rtcmID	/* OPTIONAL */;
	DSRC_RTCM_Payload_t	 payload;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_RTCMmsg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_RTCMmsg;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_RTCMmsg_H_ */
#include <asn_internal.h>
