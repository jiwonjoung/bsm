/**
 * @addtogroup bsm_shell_sec BSM Shell security related functionality
 * @{
 *
 * @section bsm_shell_doc_doc_intro BSM security module
 *
 * The BSM security code is responsible for interacting with the 1609.2
 * security libraries
 *
 * Compiling BSM Shell with Security Support
 * =========================================
 *
 * By default, bsm-shell is not compiled with security support enabled.
 *
 * IEEE 1609.2 support is provided by the Aerolink (viicsec) library.
 *
 * To enable security support recompile bsm-shell with VIICSEC=enable:
 *
 *     make BOARD=mk5 clean
 *     make BOARD=mk5 VIICSEC=enable mk5
 *
 * Then copy the resulting `bsm-shell` and `libviicsec.so.1` to the MK5:
 *
 *     scp bsm-shell ../../../aerolink/mk5/lib/libviicsec.so.1 user@IP_OF_MK5:/tmp
 *
 * Then copy the `libNxDisaptcher.so` files to the MK5:
 *
 *     scp ../../../nxp/dispatcher/mk5/libNxDispatcher.so* user@IP_OF_MK5:/tmp
 *
 *
 * Deploying Configuration and Certificates
 * ========================================
 *
 * An appropriate Aerolink configuration is also needed. The demo example used
 * in Aerolink can be used:
 *
 *     scp -r ../../../aerolink/configurations/ieee-v3 user@IP_OF_MK5:/tmp
 *
 * Finally the tools to generate certificates need to be deployed on the MK5:
 *
 *     scp ../../../aerolink/mk5/bin/ieeeAcfGenerator user@IP_OF_MK5:/tmp
 *     scp ../../../aerolink/mk5/bin/certadm user@IP_OF_MK5:/tmp
 *
 *
 * Setting it all up on the MK5
 * ============================
 *
 * Log into the MK5 and then we can set up all the configuration and
 * certificates etc.
 *
 *     ssh user@IP_OF_MK5
 *
 * Then the following commands should be run on the MK5
 *
 *     # remove any existing Aerolink configuration and state
 *     rm -rf /etc/aerolink /tmp/aerolink
 *
 *     # and replace with the demo configuration
 *     mv /tmp/ieee-v3/config /etc/aerolink
 *
 *     # then setup local aerolink state storage
 *     mv /tmp/ieee-v3/state /tmp/aerolink
 *
 *     # import root certificate into local database
 *     cd /tmp
 *     LD_LIBRARY_PATH=. ./certadm add aerolink/certificates/ieee-v3-demo-root.cert
 *
 *     # and create a BSM signing certificate (that lasts for 1 day)
 *     START_DATE=`date +"%Y-%m-%d"`
 *     DURATION=24h
 *     LD_LIBRARY_PATH=. ./ieeeAcfGenerator --certType implicit --signerCert ./aerolink/certificates/ieee-v3-demo-root.cert --signerKey ./aerolink/certificates/ieee-v3-demo-root.signKey --certId L:8:0x11-22-33-44-55-66-77-88-99:0x11-22-33-44:0x99-88-77-66-55-44-33-22-11 --cracaId 0x12-ab-34 --crlSeries 0 --start "${START_DATE}" --duration "${DURATION}" --appPermissions 0x20 --setSimultaneousCerts 96 --setCount 1 --setInterval 3h --output ./aerolink/certificates/psid-0x20
 *
 * Then you should be able to run the security enabled copy of bsm-shell as
 * normal. Note however that the WsmPSID parameter must be specified to 0x20
 * which is the standard secured BSM PSID, and `LD_LIBRARY_PATH` must be set to
 * the directory which contains `libviicsec.so.1`.
 *
 *     LD_LIBRARY_PATH=/tmp /tmp/bsm-shell --WsmPsid 0x20
 *
 * This means outgoing BSMs will be signed and any incoming BSMs will be
 * verified using Aerolink against the demo root certificate.
 *
 * @file
 *
 * Interface to the IEEE1609.2 security libraries
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

#ifndef __BSM_SEC_H__
#define __BSM_SEC_H__

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <stdint.h>
#include "bsm-opts.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

int Dot2_Init (tConfigIniData *pConfig);
void Dot2_Deinit (void);

int Dot2_Verify (uint8_t *pDot2Data, int Dot2DataLen,
                 const uint8_t **ppToBeSigned, uint32_t *pToBeSignedLen);

int Dot2_Sign (uint8_t *pToBeSigned, int ToBeSignedLen,
               uint8_t *pDot2Data, uint32_t *pDot2DataLen);

int Dot2_Inspect (uint8_t *pBuf, size_t Len);

#endif // __BSM_SEC_H__

// Close the doxygen group
/**
 * @}
 */

