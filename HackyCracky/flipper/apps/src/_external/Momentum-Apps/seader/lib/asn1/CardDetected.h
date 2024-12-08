/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Seader"
 * 	found in "seader.asn1"
 * 	`asn1c -D ./lib/asn1 -no-gen-example -no-gen-OER -no-gen-PER -pdu=all`
 */

#ifndef	_CardDetected_H_
#define	_CardDetected_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CardDetails.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CardDetected */
typedef struct CardDetected {
	CardDetails_t	 detectedCardDetails;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CardDetected_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CardDetected;
extern asn_SEQUENCE_specifics_t asn_SPC_CardDetected_specs_1;
extern asn_TYPE_member_t asn_MBR_CardDetected_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _CardDetected_H_ */
#include <asn_internal.h>
