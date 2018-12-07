/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "DSRC_R36_Source.ASN"
 * 	`asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names`
 */

#ifndef	_DSRC_ExitService_H_
#define	_DSRC_ExitService_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "DSRC_ITIScodes.h"
#include <IA5String.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DSRC_ExitService_item_PR {
	DSRC_ExitService_item_PR_NOTHING,	/* No components present */
	DSRC_ExitService_item_PR_itis,
	DSRC_ExitService_item_PR_text
} DSRC_ExitService_item_PR;

/* DSRC_ExitService */
typedef struct DSRC_ExitService {
	A_SEQUENCE_OF(struct DSRC_ExitService__Member {
		struct DSRC_ExitService_item {
			DSRC_ExitService_item_PR present;
			union DSRC_ExitService_item_u {
				DSRC_ITIScodes_t	 itis;
				IA5String_t	 text;
			} choice;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} item;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} ) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DSRC_ExitService_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DSRC_ExitService;

#ifdef __cplusplus
}
#endif

#endif	/* _DSRC_ExitService_H_ */
#include <asn_internal.h>
