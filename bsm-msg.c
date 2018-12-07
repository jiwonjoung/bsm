/**
 * @addtogroup bsm_shell_msg BSM Shell BSM Message handling
 * @{
 *
 * @file
 *
 * J2735 Basic Safety Message Library
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
#include <stdbool.h>

#include "debug-levels.h"

#include "dot3-wsmp.h"

#include "bsm-msg.h"
#include "bsm-sec.h"
#include "if-p1609.h"
#include "bsm-shell.h"

#include "util-log.h"


//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// MAC address length in uint8_t
#define WSM_HDR_MAC_LEN 6

/// MAC address defined to 0xFFFFFFFFFFFF in Tx
#define WSM_HDR_MAC_TX_FILL  0xFF

/// WSM header expiry time=0, never expire
#define WSM_HDR_DEFAULT_EXPIRY_TIME 0

/// NULL test
#define IS_NULL(arg) \
  ((arg) == NULL)

/// Alloc structure size with calloc
#define ALLOC_OBJ(type, size) \
  (type *)calloc(1, (sizeof(type)*size)+1)


//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Function Prototypes
//------------------------------------------------------------------------------

/// Create WSM header
static void P1609_CreateWSMHeader(
  struct Dot3WSMPHdr *pHdr,
  uint16_t Len);

/// Send WSM
static int P1609_SendWSM(
  struct Dot3WSMPHdr *pWSM);


//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// Debug for ASN functions
int opt_debug = 0;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief Creates a SAEBasicSafetyMessage
 * @param pBlob1 Blob values
 * @param pEvents Event value
 * @return zero for success,
 *             otherwise negative
 *
 * Send a Basic Safety Message given the Blob1 values and the Events.
 * This involves creating and populating the BSM C structure, then DER
 * encoding the data to a buffer, then pre-pending the Wave Short Message
 * header to the data and sending that as a WSM via the IEEE1609_WSMP
 * protocol socket.
 *
 * Once the DER encoded data exists the BSM C structure can be freed.
 *
 */
int J2735_BSMSend(
  tBlob1 *pBlob1,
  uint16_t *pEvents)
{
  int Res;
  int BufLen = 0;
  uint8_t *pBuf = NULL;
  asn_enc_rval_t EncRVal; // Encoder return value
  DSRC_BasicSafetyMessage_t *pBsm = NULL;

  d_fnstart(D_INTERN, NULL, "pBlob %p, Events 0x%4.4x\n",
            pBlob1, *pEvents);

  Res = J2735_BSMCreate(&pBsm, pBlob1, pEvents);
  if (Res != J2735_BSM_ERR_NONE)
  {
    d_error(D_ERR, NULL, "Failed to construct SAEBasicSafetyMessage %d\n",
            Res);
    Res = J2735_RET_CREATE_FAIL;
    goto Error;
  }

  // BufSize has to be large enough to hold encoded & signed message
  BufLen = 1792;
  pBuf = calloc(1, BufLen);
  if (pBuf == NULL)
  {
    d_error(D_ERR, NULL, "Failed to allocate buffer for encoding!\n");
    goto Error;
  }

  // Encode the SAEBasicSafetyMessage to a buffer
  EncRVal = J2735_BSMEncode_To_Buffer(pBsm,
                                      (char *)pBuf + WSMP_HDR_SIZE,
                                      BufLen - WSMP_HDR_SIZE);
  if (EncRVal.encoded == -1)
  {
    d_error(D_ERR, NULL, "Failed to encode SAEBasicSafetyMessage (at %s)\n",
            EncRVal.failed_type ? EncRVal.failed_type->name : "unknown");
    Res = J2735_RET_ENCODE_FAIL;
    goto Error;
  }
  else
  {
    // Encoding finished successfully
    d_printf(D_DEBUG, NULL, "BER encoded SAEBasicSafetyMessage has %zd bytes\n",
             EncRVal.encoded);
  }

  if (EncRVal.encoded > UINT16_MAX)
  {
    d_error(D_ERR, NULL, "BSM encoded length too large (%zd)\n",
            EncRVal.encoded);
    Res = J2735_RET_SIZE_FAIL;
    goto Error;
  }

#ifdef NO_VIICSEC
  // Nothing to do. The Unsigned BSM is ready
  BufLen = EncRVal.encoded;
#else
  // Sign the data and copy it back into pBuf
  {
    uint8_t *pToBeSigned = pBuf + WSMP_HDR_SIZE;
    int ToBeSignedLen = EncRVal.encoded;
    uint32_t Dot2DataLen = BufLen - WSMP_HDR_SIZE;
    uint8_t *pDot2Data = calloc(1, Dot2DataLen);

    // 1609.2 sign the data (pDot2Data is created)
    Res = Dot2_Sign(pToBeSigned, ToBeSignedLen, pDot2Data, &Dot2DataLen);
    if (Res != 0)
    {
      free(pDot2Data);
      goto Error;
    }
    // Copy the Dot2Data back onto pBuf (after the WSMP header)
    memcpy(pBuf + WSMP_HDR_SIZE, pDot2Data, Dot2DataLen);
    BufLen = Dot2DataLen;
    free(pDot2Data);
  }
#endif

  // Log BSM to stdout/file/udp
  LOGBsm("BSM Tx...\n", pBsm);

  Res = WSMSend((struct Dot3WSMPHdr *)pBuf, (uint16_t)BufLen);
  if (Res != J2735_RET_OK)
  {
    d_error(D_ERR, NULL, "WSM send failed (%d)\n",
            Res);
    Res = J2735_RET_SEND_FAIL;
    goto Error;
  }

  // Success!
  Res = J2735_RET_OK;

Error:
  (void)J2735_BSMFree(pBsm, J2735_BSM_FREE_ALL);
  // Ony free the pBuf if we've failed
  if (Res != J2735_RET_OK)
    free(pBuf);

  d_fnend(D_INTERN, NULL, "() = %d\n", Res);
  return Res;
}


/**
 * @brief Decodes a SAEBasicSafetyMessage
 * @param pMsg BSM to decode
 * @return Positive offset within the pMsg->Data, otherwise a negative errno
 *
 * Handle the reception of a Wave Short Message.
 * Verify/Skip the 1609.2 header.
 * Decode the unsigned data with the BSM DER decoder
 * (and held in a C data structure.) The message ID field
 * can then be checked. The message is simply logged and the data then freed.
 *
 */
int J2735_BSMReceive(
  struct Dot3WSMPHdr *pMsg)
{
  int RetVal = J2735_RET_UNKNOWN;

  d_fnstart(D_INTERN, NULL, "pMsg %p\n", pMsg);

  d_printf(D_DEBUG, NULL, "recv WSM message: bytes %d\n",
           htons(pMsg->Length));
  d_printf(D_DEBUG, NULL, "rssi %d, cbr %d, chan %d,"
           " rate %d, power %d, PSID %8.8x, len %d\n",
           pMsg->Rx.RSSI,
           pMsg->Rx.CBR,
           pMsg->ChannelNumber,
           pMsg->DataRate,
           pMsg->TxPower,
           ntohl(pMsg->PSID),
           ntohs(pMsg->Length));

  char *pBER = (char *)pMsg->Data;
  uint32_t BERLen = ntohs(pMsg->Length);
  DSRC_BasicSafetyMessage_t *pBsm = NULL;
  asn_dec_rval_t RVal;
  int Offset = 0;

  // Check for signed/unsigned data
  switch (pMsg->Data[0])
  {
    case 0x02: // signed (1609.2 Dot2Data)
      Offset = Dot2_Verify((uint8_t *)pMsg->Data, ntohs(pMsg->Length),
                        (const uint8_t **)&pBER, (uint32_t *)&BERLen);
      //Dot2 header is stripped off.
      //pBER now points to start of BSM.
      if (Offset < 0)
      {
        d_error(D_ERR, NULL,"Negative offset of %d\n", Offset);
        RVal.code = RC_FAIL;
        break;
      }
      // fallthrough
    case 0x30: // unsigned (SAE J2735)
      RVal = J2735_BSMDecode(&pBsm, pBER, BERLen);
      break;
    default: // unknown
      RVal.code = RC_FAIL;
      break;
  }

  switch (RVal.code)
  {
    case RC_OK:
      // Decoded successfully
      // Check for BSM
      if (pBsm->msgID == DSRC_DSRCmsgID_basicSafetyMessage)
      {
        // Log to stdout/file/udp
        LOGBsm("BSM Rx...\n", pBsm);
        // Read the BSM and get whatever you need
        ShellGetBsmEvents(pBsm);
      }
      else
      {
        d_printf(D_DEBUG, NULL, "DSRC msgId %d received\n", (int)pBsm->msgID);
      }

      J2735_BSMFree(pBsm, J2735_BSM_FREE_ALL);
      RetVal = Offset;
      break;

    case RC_WMORE:
      // More data expected, call again
      d_printf(D_DEBUG, NULL, "RVal.code RC_WMORE\n");
      RetVal = J2735_RET_DECODE_MORE;
      break;

    case RC_FAIL:
      // Failure to decode data
      // no break, fall thru..
    default:
      // RVal.code != RC_OK
      RetVal = J2735_RET_DECODE_FAIL;
      break;
  }

  d_fnend(D_INTERN, NULL, "() = %d\n", RetVal);
  return RetVal;
}

/**
 * @brief Create a WSM header for the encoded buffer.
 * @param pHdr Pointer to the WSM header
 * @param Len Length of the BER encoded data in the buffer.
 *
 * Set the Wave Short Message header values based on user configuration
 * and defined constants.
 *
 */
static void P1609_CreateWSMHeader(
  struct Dot3WSMPHdr *pHdr,
  uint16_t Len)
{
  tWAVEConfig Cfg = P1609CtrlInfo_GetCfgState();
  uint32_t ChannelLoad;

  d_fnstart(D_INTERN, NULL, "(pHdr %p Len %d)\n", pHdr, Len);
  d_assert(pHdr != NULL);

  memset(pHdr->Tx.DA, WSM_HDR_MAC_TX_FILL, WSM_HDR_MAC_LEN);
  pHdr->Tx.Priority   = Cfg.Priority;
  pHdr->Tx.ExpiryTime = WSM_HDR_DEFAULT_EXPIRY_TIME;
  pHdr->Version       = DOT3_WSMP_VERSION_3;
  pHdr->ChannelNumber = Cfg.ChanNbr;
  pHdr->DataRate      = Cfg.DataRate;
  pHdr->TxPower       = Cfg.TxPwr;
  pHdr->PSID          = htonl(Cfg.WsmPsid);
  pHdr->Length        = htons(Len);
  pHdr->HdrExtFlags   = 0x07; // Channel | DataRate | TxPwr

  ChannelLoad = Cfg.SafetySupp;
  if (ChannelLoad != 0)
  {
    d_printf(D_DEBUG, NULL, "Channel Load: 0x%08X\n", ChannelLoad);
    pHdr->ChannelLoad = htonl(ChannelLoad);
    pHdr->HdrExtFlags |= 0x08; // ChannelLoad
  }

  d_printf(D_DEBUG, NULL, "Priority %d ChanNbr %d DataRate %d "
           "TxPwr %d PSID %08x Load %08x)\n",
           pHdr->Tx.Priority,
           pHdr->ChannelNumber,
           pHdr->DataRate,
           pHdr->TxPower,
           ntohl(pHdr->PSID),
           ntohl(pHdr->ChannelLoad));

  d_fnend(D_INTERN, NULL, "()\n");
}


/**
 * @brief Transmit WSM using socket
 * @param pWSM constructed WSM to send
 * @return Status code, zero for success, otherwise negative
 *
 * Send a Wave Short Message using the IEEE1609_WSMP
 * protocol socket
 *
 */
static int P1609_SendWSM(
  struct Dot3WSMPHdr *pWSM)
{
  int RetVal = J2735_RET_UNKNOWN;
  struct timeval Time;
  int Flags = 0;

  d_fnstart(D_INTERN, NULL, "(pWSM %p)\n", pWSM);

  int Fd = P1609CtrlInfo_GetWSMPSocketFD();

  // Some debug on the transmitted packet
  if (d_test(D_DEBUG))
  {
    gettimeofday(&Time, NULL);
    d_printf(D_DEBUG, NULL, "tx WSM message: bytes %zu (at %05d.%06d)\n",
             htons(pWSM->Length)+WSMP_HDR_SIZE,
             (int)Time.tv_sec,
             (int)Time.tv_usec);
    d_printf(D_DEBUG, NULL, "MAC %02x:%02x:%02x:%02x:%02x:%02x pri %d, "
             "exp %d, ver %d, chan %d, rate %d, power %d, "
             "PSID 0x%08x, ext 0x%02x,  load 0x%08x, len %d\n",
             pWSM->Tx.DA[0],
             pWSM->Tx.DA[1],
             pWSM->Tx.DA[2],
             pWSM->Tx.DA[3],
             pWSM->Tx.DA[4],
             pWSM->Tx.DA[5],
             pWSM->Tx.Priority,
             pWSM->Tx.ExpiryTime,
             pWSM->Version,
             pWSM->ChannelNumber,
             pWSM->DataRate,
             pWSM->TxPower,
             htonl(pWSM->PSID),
             pWSM->HdrExtFlags,
             htonl(pWSM->ChannelLoad),
             htons(pWSM->Length));
    d_dump(D_DEBUG, NULL, pWSM, htons(pWSM->Length)+WSMP_HDR_SIZE);
  }

  // Transmit the WSM
  int Res = sendto(Fd,
                   pWSM, htons(pWSM->Length)+WSMP_HDR_SIZE,
                   Flags, NULL, 0);

  if (Res < 0)
  {
    d_error(D_ERR, NULL, "sendto() failed %d '%s'\n", errno, strerror(errno));
    RetVal = J2735_RET_SEND_FAIL;
    goto Error;
  }

  RetVal = J2735_RET_OK;

Error:
  free(pWSM);

  d_fnend(D_INTERN, NULL, "() = %d\n", RetVal);
  return RetVal;
}


/**
 * @brief Creates an SAEBasicSafetyMessage and populate it
 * @param ppBsm Pointer to SAEBasicSafetyMessage_t
 * @param pBlob1 Pointer to Blob1 values
 * @param pEvents Pointer to Event value
 * @return     J2735_BSM_ERR_NONE for success,
 *             otherwise J2735_BSM_ERR_MALLOC
 *
 * Create a Basic Safety Message structure by allocating the memory
 * for the necessary components. The fields within those components are
 * set to the values determined by the input interfaces and stored
 * within the Blob1 data store.
 *
 * For optional components, the structure is only added if there is
 * data to convey within the message.
 *
 */
int J2735_BSMCreate(
  DSRC_BasicSafetyMessage_t **ppBsm,
  tBlob1 *pBlob1,
  uint16_t *pEvents)
{
  int RetVal = J2735_BSM_ERR_NONE;

  d_fnstart(D_INTERN, NULL, "(ppBsm %p, pBlob1 %p, Events 0x%4.4x)\n",
            ppBsm, pBlob1, (pEvents == NULL) ? (uint16_t)0x0000 : *pEvents);

  // Allocate memory for the main SAEBasicSafetyMessage_t
  *ppBsm = ALLOC_OBJ(DSRC_BasicSafetyMessage_t, 1);
  if (!*ppBsm)
  {
    RetVal = J2735_BSM_ERR_MALLOC;
    goto Error;
  }

  // set the msgID, SAEBasicSafetyMessage
  (*ppBsm)->msgID = DSRC_DSRCmsgID_basicSafetyMessage;

  // Allocate memory for the Part 1 Blob octet string
  if (IS_NULL((*ppBsm)->blob1.buf))
  {
    (*ppBsm)->blob1.buf = ALLOC_OBJ(uint8_t, BLOB1_SIZE);
    if (!(*ppBsm)->blob1.buf)
    {
      RetVal = J2735_BSM_ERR_MALLOC;
      goto Error;
    }
    (*ppBsm)->blob1.size = BLOB1_SIZE;
  }

  // Set the Part 1 Blob values
  if (pBlob1)
  {
    memcpy((*ppBsm)->blob1.buf, pBlob1, BLOB1_SIZE);
  }

  // Allocate memory for the Part 2 VehicleSafetyExtension
  if (pEvents && *pEvents)
  {
    // create VehicleSafetyExtension
    (*ppBsm)->safetyExt = ALLOC_OBJ(DSRC_VehicleSafetyExtension_t, 1);
    if (!(*ppBsm)->safetyExt)
    {
      RetVal = J2735_BSM_ERR_MALLOC;
      goto Error;
    }

    DSRC_VehicleSafetyExtension_t *pSafetyExt = (*ppBsm)->safetyExt;
    d_printf(D_DEBUG, NULL, "pSafetyExt %p \n", pSafetyExt);

    // Allocate memory for the VehicleSafetyExtension Event Flags
    pSafetyExt->events = ALLOC_OBJ(DSRC_EventFlags_t, 1);
    if (!pSafetyExt->events)
    {
      RetVal = J2735_BSM_ERR_MALLOC;
      goto Error;
    }

    // set EventFlags
    *pSafetyExt->events = *pEvents;

    d_printf(D_DEBUG, NULL, "pSafetyExt->events %p \n", pSafetyExt->events);
  }
  // Part 2 VehicleStatus and other optional BSM elements not created
  goto Success;

Error:
  if (*ppBsm)
  {
  free((*ppBsm)->safetyExt);
    free((*ppBsm)->blob1.buf);
  free((*ppBsm));
  }
Success:
  d_fnend(D_INTERN, NULL, "() = %d\n", RetVal);
  return RetVal;
}


/**
 * @brief BER encodes the provided SAEBasicSafetyMessage
 * C structure into the provided buffer
 * @param pBsm Pointer to SAEBasicSafetyMessage_t structure to be encoded
 * @param pBuf Pointer to the buffer to store the encoded data.
 * @param Size Length of the above buffer
 * @return Return the size of the encoded data.
 *
 * Use the DER encoder (DER: Distinguished Encoding Rules) to represent the
 * values within the C structure in an efficient manner. The encoder is
 * generated by the asn1c toolset based on the DSRC ASN code.
 *
 */
asn_enc_rval_t J2735_BSMEncode_To_Buffer(
  DSRC_BasicSafetyMessage_t *pBsm,
  char *pBuf,
  size_t Size)
{
  asn_enc_rval_t EncRVal; // Encoder return value

  d_fnstart(D_INTERN, NULL, "(pBsm %p pBuf %p Size %zd)\n", pBsm, pBuf, Size);

  //Encode the SAEBasicSafetyMessage type as BER (DER)
  EncRVal = der_encode_to_buffer(&asn_DEF_DSRC_BasicSafetyMessage,
                                 pBsm, pBuf, Size);

  d_fnend(D_INTERN, NULL, "() = %zd\n", EncRVal.encoded);
  return EncRVal;
}


/**
 * @brief BER decodes the provided buffer into the corresponding structure
 * @param ppBsm Pointer to Pointer to SAEBasicSafetyMessage_t
 * @param pBuf Pointer to Buffer to decode into
 * @param Size Size of buffer to decode into
 * @return Return value of the decoding functions
 *
 * Use the DER decoder (DER: Distinguished Encoding Rules) to expand the binary
 * data stream into a C data structure representation of the Basic Safety
 * Message. The decoder is generated by the asn1c toolset based on the DSRC
 * ASN code.
 *
 */
asn_dec_rval_t J2735_BSMDecode(
  DSRC_BasicSafetyMessage_t **ppBsm,
  char *pBuf,
  size_t Size)
{
  asn_dec_rval_t RVal; // Decoder return value

  d_fnstart(D_INTERN, NULL, "(ppBsm %p, pBuf %p, Size %zd)\n",
            ppBsm, pBuf, Size);

  // Decode the provided buffer as SAEBasicSafetyMessage type
  RVal = ber_decode(0,
                    &asn_DEF_DSRC_BasicSafetyMessage,
                    (void *)ppBsm,
                    pBuf,
                    Size);

  d_fnend(D_INTERN, NULL, "() = %zd, %d\n", RVal.consumed, RVal.code);
  return RVal;
}


/**
 * @brief Free the SAEBasicSafetyMessage data structure
 * @param pBsm Pointer to SAEBasicSafetyMessage_t
 * @param FreeOption Option J2735_BSM_FREE_ALL to free the structure and all
 * the members of the structure and thier submembers recursively. Option
 * J2735_BSM_FREE_CONTENTS_ONLY to leave the outer pointer intact.
 * @return J2735_BSM_ERR_NONE for success.
 *
 * Provide a mechanism to recursively free the components within the C data
 * structure representation.
 *
 */
int J2735_BSMFree (DSRC_BasicSafetyMessage_t *pBsm,
                   eJ2735BSMFreeOption FreeOption)
{
  d_fnstart(D_INTERN, NULL, "(pBsm %p, FreeOption %d)\n", pBsm, FreeOption);

  if (pBsm == NULL)
    goto Error;

  switch (FreeOption)
  {
    case J2735_BSM_FREE_ALL:
      // Free the SAEBasicSafetyMessage_t structure
      // and free the pointer too
      ASN_STRUCT_FREE(asn_DEF_DSRC_BasicSafetyMessage, pBsm);
      break;
    case J2735_BSM_FREE_CONTENTS_ONLY:
      ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_DSRC_BasicSafetyMessage, pBsm);
      break;
    default:
      break;
  }

Error:
  d_fnend(D_INTERN, NULL, "() = %d\n", J2735_BSM_ERR_NONE);
  return J2735_BSM_ERR_NONE;
}

/**
 * @brief Send a pre-formulated WSM
 * @param pWSM A pointer to the WSM
 *             @note The WSMP header will be (re)populated based on the cmd line config
 * @param Len Length of the WSM _data_
 *             @note This length does not include the 'struct Dot3WSMPHdr'
 * @return     J2735_RET_OK for success, otherwise non-zero errors (@ref eP1609Ret)
 *
 * @note The WSM data buffer is freed by this function
 */
int WSMSend (
  struct Dot3WSMPHdr *pWSM,
  size_t Len)
{
  int RetVal = J2735_RET_UNKNOWN;
  int Res;

  d_fnstart(D_INTERN, NULL, "pWSM %p Len %zd\n", pWSM, Len);

  if (Len > UINT16_MAX)
  {
    d_error(D_ERR, NULL, "WSM length too large (%zd)\n", Len);
    RetVal = J2735_RET_SIZE_FAIL;
    free(pWSM);
    goto Exit;
  }

  // Add the WSM Header to the buffer
  P1609_CreateWSMHeader(pWSM, Len);

  // Send
  Res = P1609_SendWSM(pWSM);
  if (Res != J2735_RET_OK)
  {
    d_error(D_ERR, NULL, "WSM send failed (%d)\n", Res);
    RetVal = J2735_RET_SEND_FAIL;
    goto Exit;
  }

  // Success!
  RetVal = J2735_RET_OK;

Exit:
  d_fnend(D_INTERN, NULL, "() = %d\n", RetVal);
  return RetVal;
}

// Close the doxygen group
/**
 * @}
 */
