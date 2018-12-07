/**
 * @addtogroup bsm_shell_sec BSM Shell security related functionality
 * @{
 *
 * @file
 *
 * IEEE1609.2 Library interactions
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <sys/time.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#ifndef NO_VIICSEC
#include "viicsec.h"
#endif

#include "bsm-sec.h"
#include "debug-levels.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------


#define DOT2_CONTENT_TYPE_UNSECURED   (0)
#define DOT2_CONTENT_TYPE_SIGNED      (1)
#define DOT2_CONTENT_TYPE_ENCRYPTED   (2)
#define DOT2_CONTENT_TYPE_CERT_REQ    (3)
#define DOT2_CONTENT_TYPE_CERT_RSP    (4)
#define DOT2_CONTENT_TYPE_ANON_RSP    (5)
#define DOT2_CONTENT_TYPE_CERT_ERR    (6)
#define DOT2_CONTENT_TYPE_CRL_REQ     (7)
#define DOT2_CONTENT_TYPE_CRL         (8)
#define DOT2_CONTENT_TYPE_SIGNED_PART (9)
#define DOT2_CONTENT_TYPE_SIGNED_WSA  (11)

#define DOT2_SIGNER_TYPE_SELF         (0)
#define DOT2_SIGNER_TYPE_DIGEST_224   (1)
#define DOT2_SIGNER_TYPE_DIGEST_256   (2)
#define DOT2_SIGNER_TYPE_CERT         (3)
#define DOT2_SIGNER_TYPE_CHAIN        (4)
#define DOT2_SIGNER_TYPE_DIGEST_OTHER (5)

#define DOT2_SUBJECT_TYPE_MSG_ANON   (0)
#define DOT2_SUBJECT_TYPE_MSG_ID     (1)
#define DOT2_SUBJECT_TYPE_MSG_ID_LOC (2)
#define DOT2_SUBJECT_TYPE_CSR        (3)
#define DOT2_SUBJECT_TYPE_WSA        (4)
#define DOT2_SUBJECT_TYPE_WSA_CSR    (5)
#define DOT2_SUBJECT_TYPE_MSG_CA     (6)
#define DOT2_SUBJECT_TYPE_WSA_CA     (7)
#define DOT2_SUBJECT_TYPE_CRL_SIGN   (8)
#define DOT2_SUBJECT_TYPE_ROOT_CA    (255)

#define DOT2_MESSAGE_FLAGS_FRAGMENT     (1<<0)
#define DOT2_MESSAGE_FLAGS_USE_GEN_TIME (1<<1)
#define DOT2_MESSAGE_FLAGS_EXPIRES      (1<<2)
#define DOT2_MESSAGE_FLAGS_USE_LOCATION (1<<3)
#define DOT2_MESSAGE_FLAGS_EXTENSIONS   (1<<4)

#define DOT2_CERT_CONT_FLAGS_START_VALIDITY    (1<<0)
#define DOT2_CERT_CONT_FLAGS_LIFETIME_DURATION (1<<1)
#define DOT2_CERT_CONT_FLAGS_ENC_KEY           (1<<2)

#define DOT2_PKALG_ECDSA_224_224 (0)
#define DOT2_PKALG_ECDSA_256_256 (1)
#define DOT2_PKALG_ECIES_256     (2)

#define DOT2_ECC_KEY_TYPE_X_CORD_ONLY (0)
#define DOT2_ECC_KEY_TYPE_COMPRESSED_LSB_Y0 (2)
#define DOT2_ECC_KEY_TYPE_COMPRESSED_LSB_Y1 (3)
#define DOT2_ECC_KEY_TYPE_UNCOMPRESSED (4)

#define DOT2_REGION_TYPE_FROM_ISSUER (0)
#define DOT2_REGION_TYPE_CIRCLE      (1)
#define DOT2_REGION_TYPE_RECTANGLE   (2)
#define DOT2_REGION_TYPE_POLYGON     (3)
#define DOT2_REGION_TYPE_NONE        (4)

#define DOT2_SYMM_ALGO_AES_CCM_128   (0)

//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------

#ifndef NO_VIICSEC
typedef SecuredMessageParserC WSParser;
typedef SecuredMessageGeneratorC WSGenerator;
typedef SecurityContextC WSContext;
#else
typedef void * SecurityContextC;
#endif

/// Dot2 state storage
typedef struct Dot2
{
  /// PSID of messages
  uint32_t PSID;
  /// Verification context
  SecurityContextC pContext;

} tDot2;

//------------------------------------------------------------------------------
// Local Function Prototypes
//------------------------------------------------------------------------------

int Dot2_InspectDot2 (uint8_t *pBuf, int *pLen);

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// Dot2 state
static struct Dot2 _Dot2 = {0, };

/// Dot2 state access pointer
static struct Dot2 *pDot2 = &_Dot2;


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief Dot2 Init
 * @return Zero for success, otherwise a negative errno
 *
 */
int Dot2_Init (tConfigIniData *pConfig)
{
  int Res;

  d_fnstart(D_DEBUG, NULL, "()\n");

  // PSID
  pDot2->PSID = pConfig->WsmPsid;

#ifndef NO_VIICSEC
  {
    AEROLINK_RESULT Result;
    Result = securityServices_initialize();
    if (Result != WS_SUCCESS)
    {
      d_printf(D_ERR, NULL, "SecurityServices_initialize() FAIL result %s\n",
               ws_errid(Result));
      goto Error;
    }
    d_printf(D_WARN, NULL, "Aerolink version %s\n", securityServices_getVersion());

    // Security Context
    Result = sc_open("obu.wsc", &pDot2->pContext);
    if (Result != WS_SUCCESS)
    {
      d_printf(D_ERR, NULL, "sc_open(%s) failed: %s\n", "smdemo.wsc", ws_errid(Result));
      goto Error;
    }
  }

  Res = 0;
Error:
#else
  Res = 0;
#endif

  d_fnend(D_DEBUG, NULL, "() = %d\n", Res);
  return Res;
}


/**
 * @brief Dot2 Deinit
 * @param pDot2 Handle to free
 *
 * Clean up Dot2 library
 *
 */
void Dot2_Deinit (void)
{
#ifdef NO_VIICSEC
  return;
#else
  d_fnstart(D_DEBUG, NULL, "()\n");

  if (pDot2->pContext != NULL)
    sc_close(pDot2->pContext);

  securityServices_shutdown();

  d_fnend(D_DEBUG, NULL, "()\n");
#endif
}


/**
 * @brief IEE1609.2 verify some Dot2Data
 * @param pDot2Data A pointer to signed data
 * @param Dot2DataLen Length of the pDot2Data buffer
 * @param ppToBeSigned A buffer pointer populated by this function (required to be free'ed)
 * @param ptoBesignedLen Length of the buffer returned above
 * @return Zero (or +ve Offset of Data for signed packet) for success, otherwise a negative errno
 *
 * @note *ppToBeSigned is within pDot2Data
 */
int Dot2_Verify (uint8_t *pDot2Data, int Dot2DataLen,
                 const uint8_t **ppToBeSigned, uint32_t *pToBeSignedLen)
{
#ifdef NO_VIICSEC
  int Offset = Dot2_Inspect(pDot2Data, Dot2DataLen);
  *ppToBeSigned = pDot2Data + Offset;
  *pToBeSignedLen = Dot2DataLen - Offset;
  return Offset;
#else
  int Res = -ENOSYS;
  AEROLINK_RESULT Result;
  MessageSecurityType CertType;
  WSParser Wsmp = NULL;

  // Create a parser using the verify context
  Result = smp_new(pDot2->pContext, &Wsmp);
  if (Result != WS_SUCCESS)
  {
    d_error(D_ERR, NULL, "Error creating new SMP: %s\n", ws_errid(Result));
    Res = -ENOMEM;
    goto Error;
  }

  Result = smp_extract(Wsmp, pDot2Data, Dot2DataLen, &CertType,
                       ppToBeSigned, pToBeSignedLen);
  if (Result != WS_SUCCESS)
  {
    d_error(D_ERR, NULL, "Error extracting message: %s\n", ws_errid(Result));
    Res = -EINVAL;
    goto Error;
  }
  Result = smp_checkRelevance(Wsmp);
  if (Result != WS_SUCCESS)
  {
    d_error(D_ERR, NULL, "Error checking message relevance: %s\n", ws_errid(Result));
    Res = -EINVAL;
    goto Error;
  }
  Result = smp_checkConsistency(Wsmp);
  if (Result != WS_SUCCESS)
  {
    d_error(D_ERR, NULL, "Error checking message consistency: %s\n", ws_errid(Result));
    Res = -EINVAL;
    goto Error;
  }

  static const int Dot2ValidateProbability = (int)(RAND_MAX / 256);
  // verify every 1 in 256 that pass the initial checks
  if (rand() < Dot2ValidateProbability)
  {
    Result = smp_verifySignatures(Wsmp, NULL, 0);
    switch (Result)
    { // WS_ERR_xxx defined in ws_errno.h
      case WS_SUCCESS:
        d_printf(D_WARN, NULL, "Validated!\n");
        Res = 0;
        break;
      default:
        d_error(D_ERR, NULL, "Failed to verify signature: %d: %s, %s\n",
                Result, ws_errid(Result), ws_strerror(Result));
        Res = -EINVAL;
        break;
    }
  }
  else
  {
    Res = 0;
  }

Error:
  if (Wsmp != NULL)
    smp_delete(Wsmp);
  return Res;
#endif
}


/**
 * @brief IEE1609.2 sign some ToBeSignedData
 * @param pToBeSigned A pointer to unsigned data
 * @param ToBeSignedLen Length of the pToBeSigned buffer
 * @param pDot2Data A buffer pointer populated by this function (required to be free'ed)
 * @param pDot2DataLen Length of the buffer returned above
 * @return Zero for success, otherwise a negative errno
 *
 */
int Dot2_Sign (uint8_t *pToBeSigned, int ToBeSignedLen,
               uint8_t *pDot2Data, uint32_t *pDot2DataLen)
{
#ifdef NO_VIICSEC
  (void)pDot2;
  memcpy(pDot2Data, pToBeSigned, ToBeSignedLen);
  *pDot2DataLen = ToBeSignedLen;
  return 0;
#else
  int Res = -ENOSYS;
  AEROLINK_RESULT Result;
  SIGNER_TYPE_OVERRIDE Sto = STO_AUTO;
  static int Cnt = 0;

  // Create a generator, that uses an auto internal WSC
  WSGenerator Wsmg;

  Result = smg_new(pDot2->pContext, &Wsmg);
  if (Result != WS_SUCCESS)
  {
    d_printf(D_ERR, NULL, "Error creating new SMG: %s\n", ws_errid(Result));
    goto Error;
  }

  // Attach a cert every 10th WSM otherwise attach a digest - or could use
  // STO_AUTO to use the security profile's default
  if (((++Cnt) % 10) == 0)
    Sto = STO_CERTIFICATE;
  else
    Sto = STO_DIGEST;
  Result = smg_sign(Wsmg, pDot2->PSID, NULL, 0, Sto,
                    pToBeSigned, ToBeSignedLen,
                    NULL, 0,
                    pDot2Data, pDot2DataLen);
  if (Result != WS_SUCCESS)
  {
    d_error(D_ERR, NULL, "Failed to sign message with PSID %d: %s, %s\n",
            pDot2->PSID, ws_errid(Result), ws_strerror(Result));
    Res = -EAGAIN;
  }
  else
    Res = 0;

  smg_delete(Wsmg);

Error:
  return Res;
#endif
}


/**
 * @brief Find the offset of the ToBeSigned information within Dot2Data
 * @param pBuf A pointer to Dot2Data buffer
 * @param Len Event value
 * @return Positive offset within pBuf, otherwise a naegative errno
 *
 */
int Dot2_Inspect (uint8_t *pBuf, size_t Len)
{
  return Dot2_InspectDot2 (pBuf, (int *)&Len);
}

static int Dot2_InspectDot2Opaque (uint8_t *pBuf, int *pLen, int *pOffset)
{
  uint8_t Tmp;
  int Cnt = 0;

  (void)pLen;

  Tmp = pBuf[*pOffset];
  if ((Tmp & 0x80) == 0x00)
  {
    Cnt |= pBuf[*pOffset + 0] << 0;
    Cnt &= 0x0000007f;
    *pOffset += 1;
  }
  else if ((Tmp & 0xC0) == 0x80)
  {
    Cnt |= pBuf[*pOffset + 0] << 8;
    Cnt |= pBuf[*pOffset + 1] << 0;
    Cnt &= 0x00003fff;
    *pOffset += 2;
  }
  else if ((Tmp & 0xE0) == 0xC0)
  {
    Cnt |= pBuf[*pOffset + 0] << 16;
    Cnt |= pBuf[*pOffset + 1] << 8;
    Cnt |= pBuf[*pOffset + 2] << 0;
    Cnt &= 0x001fffff;
    *pOffset += 3;
  }
  else if ((Tmp & 0xF0) == 0xE0)
  {
    Cnt |= pBuf[*pOffset + 0] << 24;
    Cnt |= pBuf[*pOffset + 1] << 16;
    Cnt |= pBuf[*pOffset + 2] << 8;
    Cnt |= pBuf[*pOffset + 3] << 0;
    Cnt &= 0x0fffffff;
    *pOffset += 4;
  }
  else
  {
    d_error(D_WARN, NULL, "Unknown <var> length\n");
    d_dump(D_WARN, NULL, pBuf + *pOffset, 8);
  }

  if (Cnt > *pLen)
  {
    d_error(D_WARN, NULL, "Crazy <var> length\n");
    d_dump(D_WARN, NULL, pBuf + *pOffset - 4, 8);
    Cnt = 0;
  }

  d_printf(D_TST, NULL, "Cnt %d\n", Cnt);
  return Cnt;
}

static void Dot2_InspectDot2PSID (uint8_t *pBuf, int *pLen, int *pOffset)
{
  uint8_t Tmp;

  (void)pLen;

  // 0 to 4 bytes of PSID
  Tmp = pBuf[*pOffset];
  if ((Tmp & 0xf0) == 0xf0)
  {
    d_printf(D_WARN, NULL, "Unknown PSID encoding\n");
    d_dump(D_WARN, NULL, pBuf + *pOffset, 4);
  }
  else if ((Tmp & 0xe0) == 0xe0)
  {
    *pOffset += 4;
  }
  else if ((Tmp & 0xc0) == 0xc0)
  {
    *pOffset += 3;
  }
  else if ((Tmp & 0x80) == 0x80)
  {
    *pOffset += 2;
  }
  else
  {
    *pOffset += 1;
  }

}

static void Dot2_InspectDot2ECCPublicKey (uint8_t *pBuf, int *pLen, int *pOffset, uint8_t PkAlg)
{
  uint8_t KeyType;

  (void)pLen;

  switch (PkAlg)
  {
    case DOT2_PKALG_ECDSA_224_224:
    { // EccPublicKey public_key
      KeyType = pBuf[*pOffset];
      *pOffset += 1;
      // ECC key X
      *pOffset += 28;
      if (KeyType == DOT2_ECC_KEY_TYPE_UNCOMPRESSED)
      {
        // ECC key Y
        *pOffset += 28;
      }
    }
    break;
    case DOT2_PKALG_ECDSA_256_256:
    { // EccPublicKey public_key
      KeyType = pBuf[*pOffset];
      *pOffset += 1;
      // ECC key Y1
      *pOffset += 32;
      if (KeyType == DOT2_ECC_KEY_TYPE_UNCOMPRESSED)
      {
        // ECC key Y0
        *pOffset += 32;
      }
    }
    break;
    case DOT2_PKALG_ECIES_256:
      ; // SymmAlgorithm supported_symm_alg;
      ; // EccPublicKey public_key;
      break;
    default:
      break;
  }
}


static void Dot2_InspectDot2PublicKey (uint8_t *pBuf, int *pLen, int *pOffset)
{
  uint8_t PkAlg;

  (void)pLen;

  PkAlg = pBuf[*pOffset];
  *pOffset += 1;

  switch (PkAlg)
  {
    case DOT2_PKALG_ECDSA_224_224:
    case DOT2_PKALG_ECDSA_256_256:
      Dot2_InspectDot2ECCPublicKey(pBuf, pLen, pOffset, PkAlg);
      break;
    case DOT2_PKALG_ECIES_256:
      ; // SymmAlgorithm supported_symm_alg;
      ; // EccPublicKey public_key;
      break;
    default:
    {
      int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
      *pOffset += Tmp;
    }
    break;
  }
}


static void Dot2_InspectDot2SignedCert (uint8_t *pBuf, int *pLen, int *pOffset)
{
  uint8_t CertVer;
  uint8_t CF;

  { // Certificate version_and_type
    CertVer = pBuf[*pOffset];
    *pOffset += 1;
    d_printf(D_TST, NULL, "CertVer %d\n", CertVer);
  }

  { // ToBeSignedCertificate
    uint8_t SubjectType;

    { // SubjectType
      SubjectType = pBuf[*pOffset];
      *pOffset += 1;
      d_printf(D_TST, NULL, "SubjectType %d\n", SubjectType);
    }

    { // CertificateContentFlags
      CF = pBuf[*pOffset];
      *pOffset += 1;
      d_printf(D_TST, NULL, "CF %d\n", CF);
    }

    { // signer
      switch (SubjectType)
      {
        case DOT2_SUBJECT_TYPE_MSG_ANON:
        case DOT2_SUBJECT_TYPE_MSG_ID:
        case DOT2_SUBJECT_TYPE_MSG_ID_LOC:
        case DOT2_SUBJECT_TYPE_CSR:
        case DOT2_SUBJECT_TYPE_WSA:
        case DOT2_SUBJECT_TYPE_WSA_CSR:
        case DOT2_SUBJECT_TYPE_CRL_SIGN:
        default:
          // Signer Id
          *pOffset += 8;
          // PK algorithm
          *pOffset += 1;
          break;
        case DOT2_SUBJECT_TYPE_MSG_CA:
        case DOT2_SUBJECT_TYPE_WSA_CA:
        case DOT2_SUBJECT_TYPE_ROOT_CA:
          break;
      }
    }

    { // CertSpecificData (scope)
      switch (SubjectType)
      {
        case DOT2_SUBJECT_TYPE_MSG_ANON:
        case DOT2_SUBJECT_TYPE_MSG_ID:
        case DOT2_SUBJECT_TYPE_MSG_ID_LOC:
          if (SubjectType == DOT2_SUBJECT_TYPE_MSG_ANON)
          {
            int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
            *pOffset += Tmp;
          }
          else
          { // IdentifiedNotLocalizedScope
            d_printf(D_TST, NULL, "IdentifiedNotLocalizedScope\n");
            d_dump(D_TST, NULL, pBuf + *pOffset, 8);
            int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
            *pOffset += Tmp;
          }

          { // PsidSspArray
            uint8_t Type = pBuf[*pOffset];
            *pOffset += 1;

            switch (Type)
            {
              case 0x00: // from issuer
                d_error(D_WARN, NULL,
                        "PsidSsp ArrayType set to 'from_issuer' (unsupported)\n");
                break;
              case 0x01: // specified
                // PsidSsp
              {
                int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
                *pOffset += Tmp;
              }
              break;
              default: // unknown
                d_error(D_WARN, NULL, "Unknown PsidSsp ArrayType 0x%02x\n", Type);
                d_dump(D_WARN, NULL, pBuf + *pOffset - 4, 8);
                break;
            }
          }
          if ((SubjectType == DOT2_SUBJECT_TYPE_MSG_ID_LOC) ||
              (SubjectType == DOT2_SUBJECT_TYPE_MSG_ANON))
          { // GeographicRegion

            // Region type
            uint8_t Type = pBuf[*pOffset];
            *pOffset += 1;
            d_printf(D_TST, NULL, "Region type %d\n", Type);

            switch (Type)
            {
              case DOT2_REGION_TYPE_NONE:
                break;
              case DOT2_REGION_TYPE_FROM_ISSUER:
              case DOT2_REGION_TYPE_CIRCLE:
              case DOT2_REGION_TYPE_RECTANGLE:
              case DOT2_REGION_TYPE_POLYGON:
                d_error(D_WARN, NULL, "Unsupported RegionType\n");
                break;
              default:
              {
                int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
                *pOffset += Tmp;
              }
            }
          }
          break;

        case DOT2_SUBJECT_TYPE_CSR:
        case DOT2_SUBJECT_TYPE_WSA:
        case DOT2_SUBJECT_TYPE_WSA_CSR:
        case DOT2_SUBJECT_TYPE_CRL_SIGN:
        case DOT2_SUBJECT_TYPE_MSG_CA:
        case DOT2_SUBJECT_TYPE_WSA_CA:
        case DOT2_SUBJECT_TYPE_ROOT_CA:
          d_error(D_WARN, NULL,
                  "Unsupported SubjectType in CertSpecificData\n");
          break;
      }
    }

  }

  { // Time32 (expiration)
    *pOffset += 4;
  }

  if (CF & DOT2_CERT_CONT_FLAGS_START_VALIDITY)
  {
    if (CF & DOT2_CERT_CONT_FLAGS_LIFETIME_DURATION)
    {
      *pOffset += 2; // CertificateDuration lifetime;
    }
    else
    {
      *pOffset += 4; // Time32 start_validity;

    }
  }

  { // CrlSeries
    *pOffset += 4;
  }

  switch (CertVer)
  {
    case 0x02: // explicit certificate
      Dot2_InspectDot2PublicKey(pBuf, pLen, pOffset);
      break;

    case 0x03: // implicit certificate
      break;

    default: // unknown
    {
      int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
      *pOffset += Tmp;
    }
    break;
  }

  if (CF & DOT2_CERT_CONT_FLAGS_ENC_KEY)
  { // PublicKey
    Dot2_InspectDot2PublicKey(pBuf, pLen, pOffset);
  }

  if (((CF & DOT2_CERT_CONT_FLAGS_START_VALIDITY) == 0) &&
      ((CF & DOT2_CERT_CONT_FLAGS_ENC_KEY) == 0))
  {
    int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
    *pOffset += Tmp;
  }

  switch (CertVer)
  {
    case 0x02: // explicit certificate
    { // Signature
      *pOffset += 64;
    }
    break;

    case 0x03: // explicit certificate
      Dot2_InspectDot2ECCPublicKey(pBuf, pLen, pOffset,
                                   DOT2_PKALG_ECDSA_256_256);
      break;

    default: // unknown
    {
      int Tmp = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
      *pOffset += Tmp;
    }
    break;
  }

}



static void Dot2_InspectDot2SignedMessage (uint8_t *pBuf, int *pLen, int *pOffset)
{
  uint8_t MF;

  { // MessageFlags
    MF = pBuf[*pOffset];
    *pOffset += 1;
  }

  { // Psid
    Dot2_InspectDot2PSID(pBuf, pLen, pOffset);
  }

  { // opaque data<var>
    int Cnt;

    Cnt = Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
    *pLen = Cnt;

    // *pOffset is now at the start of the unsigned message
    d_printf(D_DEBUG, NULL, "Unsigned %d bytes at offset %d\n",
             Cnt, *pOffset);

    // Check that it is J2735 encoded data (starts with 0x30)
    if (pBuf[*pOffset] != 0x30)
    {
      d_printf(D_WARN, NULL, "Didn't find SAE J2735 at offset %d\n", *pOffset);
      d_dump(D_WARN, NULL, pBuf + *pOffset, 8);
    }

    //*pOffset += Cnt;
  }

  { // MessageFlags indicated elements

    if (MF & DOT2_MESSAGE_FLAGS_FRAGMENT)
    {
      d_printf(D_WARN, NULL,
               "MessageFlags: 'fragment' is set (unsupported)\n");
    }

    if (MF & DOT2_MESSAGE_FLAGS_USE_GEN_TIME)
    {
      // Gen time
      //*pOffset += 8;
      // Gen confidence
      //*pOffset += 1;
    }

    if (MF & DOT2_MESSAGE_FLAGS_EXPIRES)
    {
      // Expires
      //*pOffset += 8;
    }

    if (MF & DOT2_MESSAGE_FLAGS_USE_LOCATION)
    {
      // 3D Lat
      //*pOffset += 4;
      // 3D Lon
      //*pOffset += 4;
      // 3D Elev
      //*pOffset += 2;
      // 3D Conf
      //*pOffset += 1;
    }

    if (MF & DOT2_MESSAGE_FLAGS_EXTENSIONS)
    {
      // Ext type
      //*pOffset += 1;
      // Ext value
      // (void)Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
    }

    // Unknown
    if (MF == 0)
    {
      // Unknown
      // (void)Dot2_InspectDot2Opaque(pBuf, pLen, pOffset);
    }

  }

}


static void Dot2_InspectDot2Signature (uint8_t *pBuf, int *pLen, int *pOffset)
{
  (void)pBuf;
  (void)pLen;
  (void)pOffset;
  // Don't so anything this is after the unsigned message
}


static void Dot2_InspectDot2Signed (uint8_t *pBuf, int *pLen, int *pOffset)
{

  // SignerIdentifierType
  uint8_t Type = pBuf[*pOffset];
  *pOffset += 1;

  switch (Type)
  {
    case DOT2_SIGNER_TYPE_CERT:
      Dot2_InspectDot2SignedCert(pBuf, pLen, pOffset);
      Dot2_InspectDot2SignedMessage(pBuf, pLen, pOffset);
      Dot2_InspectDot2Signature(pBuf, pLen, pOffset);
      break;

    case DOT2_SIGNER_TYPE_DIGEST_224:
    case DOT2_SIGNER_TYPE_DIGEST_256:
      // Cert ID
      *pOffset += 8;
      Dot2_InspectDot2SignedMessage(pBuf, pLen, pOffset);
      Dot2_InspectDot2Signature(pBuf, pLen, pOffset);
      break;

    case DOT2_SIGNER_TYPE_CHAIN:
    case DOT2_SIGNER_TYPE_SELF:
    case DOT2_SIGNER_TYPE_DIGEST_OTHER:
    default:
      d_error(D_WARN, NULL, "Unexpected SignerIdentifierType 0x%02X\n",
              Type);
      break;
  }

}


static void Dot2_InspectDot2Unsecured (uint8_t *pBuf, int *pLen, int *pOffset)
{
  int Cnt = 0;

  Cnt |= pBuf[*pOffset + 0] << 24;
  Cnt |= pBuf[*pOffset + 1] << 16;
  Cnt |= pBuf[*pOffset + 2] << 8;
  Cnt |= pBuf[*pOffset + 3] << 0;
  *pOffset += 4;

  *pLen = Cnt;

  // *pOffset is now at the start of the unsigned message
  d_printf(D_DEBUG, NULL, "SAE J2735 %d bytes at offset %d",
           Cnt, *pOffset);

  // Check that it is J2735 encoded data (starts with 0x30)
  if (pBuf[*pOffset] != 0x30)
  {
    d_printf(D_WARN, NULL, "Didn't find SAE J2735 at offset %d", *pOffset);
    d_dump(D_WARN, NULL, pBuf + *pOffset, 8);
  }

}


void Dot2_InspectDot2Draft9(uint8_t *pBuf, int *pLen, int *pOffset)
{
  // ContentType
  uint8_t Type = pBuf[*pOffset];
  *pOffset += 1;

  switch (Type)
  {
    case DOT2_CONTENT_TYPE_UNSECURED:
      Dot2_InspectDot2Unsecured(pBuf, pLen, pOffset);
      break;
    case DOT2_CONTENT_TYPE_SIGNED:
      Dot2_InspectDot2Signed(pBuf, pLen, pOffset);
      break;
    case DOT2_CONTENT_TYPE_ENCRYPTED:
      break;
    case DOT2_CONTENT_TYPE_CRL_REQ:
      break;
    case DOT2_CONTENT_TYPE_CRL:
      break;
    case DOT2_CONTENT_TYPE_CERT_REQ:
    case DOT2_CONTENT_TYPE_CERT_RSP:
    case DOT2_CONTENT_TYPE_ANON_RSP:
    case DOT2_CONTENT_TYPE_CERT_ERR:
    case DOT2_CONTENT_TYPE_SIGNED_PART:
    case DOT2_CONTENT_TYPE_SIGNED_WSA:
    default:
      d_error(D_WARN, NULL, "Unexpected ContentType 0x%02X\n", Type);
      break;
  }

  // Post unsigned message processing
  ; // TODO
}


int Dot2_InspectDot2 (uint8_t *pBuf, int *pLen)
{
  int Offset = 0;

  switch (pBuf[Offset])
  {
    case 0x30: // SAE J2735 (no 1609.2)
      Offset = 0;
      break;
    case 0x02: // P1609.2 D9 (full-use)
      Offset++;
      Dot2_InspectDot2Draft9(pBuf, pLen, &Offset);
      break;
    case 0x01: // P1609.2 D2 (trial-use)
    default:
      d_error(D_NOTICE, NULL, "Unknown or not implemented 1609.2 version 0x%02x\n",
              pBuf[Offset]);
      Offset = -1;
      break;
  }

  return Offset;
}


// Close the doxygen group
/**
 * @}
 */
