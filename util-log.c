/**
 * @addtogroup bsm_shell_log BSM Shell Logging library
 * @{
 *
 * @file
 *
 * Module code for LOG interface
 *
 * Provide services to log actions to file or UDP. At initialisation set the
 * required LOG interfaces, either to a file, to standard out or to UDP. For
 * UDP logging the UDP destination address has to be set.
 *
 * Provide a mechanism to log a string or a Basic Safety Message to the
 * initialised destination.
 *
 */

//------------------------------------------------------------------------------
// Copyright (c) 2012 Cohda Wireless Pty Ltd
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "debug-levels.h"

#include "if-udp.h"
#include "util-log.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------

/// Buffer to hold XER format BSM
#define LOG_XER_BUF_SIZE 4096


//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------

/// LOG XER format
static int LOGDumpToBuf(
  const void *buff,
  size_t size,
  void *application_specific_key);

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------

/// LOG location
static int LOGDest = LOG_DEST_NONE;

/// LOG file
static FILE *LOGFile = 0;

/// Xer buffer
static char XerBuf[LOG_XER_BUF_SIZE];

/// LOGBuf location
static char *XerBufIndex;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

/**
 * @brief LOG Init
 *
 * Configure LOG interface and set destination
 *
 */
int LOGInit(
  int Dest,
  char *pFile)
{
  d_fnstart(D_DEBUG, NULL, "(Dest 0x%02x, pFile '%s')\n",
            Dest, pFile);

  // Log to file
  if (Dest & LOG_DEST_FILE)
  {
    LOGDest |= LOG_DEST_FILE;
    // open file
    LOGFile = fopen(pFile, "w+");

    if (LOGFile)
    {
      d_printf(D_DEBUG, NULL, "\"%s\" opened\n", pFile);
    }
    else
    {
      d_error(D_ERR, NULL, "Log file opening failed: %d, %s\n",
              errno, strerror(errno));
    }
  }

  // Log to stdout
  if (Dest & LOG_DEST_STDOUT)
  {
    LOGDest |= LOG_DEST_STDOUT;
  }

  // Log to UDP
  if (Dest & LOG_DEST_UDP)
  {
    LOGDest |= LOG_DEST_UDP;
  }

  d_fnend(D_DEBUG, NULL, "()\n");

  return 0;
}


/**
 * @brief LOG Init
 *
 * Tidy up LOG files
 *
 */
int LOGDeinit(
  void)
{
  d_fnstart(D_DEBUG, NULL, "\n");

  // close file
  if (LOGFile)
  {
    fflush(LOGFile);
    int Ret = fclose(LOGFile);
    if (Ret < 0)
    {
      d_error(D_ERR, NULL, "CAN: close failed: %d, %s\n",
              errno, strerror(errno));
    }
    LOGFile = 0;
    d_printf(D_DEBUG, NULL, "closed file\n");
  }
  else
  {
    d_printf(D_DEBUG, NULL, "wasn't open\n");
  }

  d_fnend(D_DEBUG, NULL, "()\n");

  return 0;
}


/**
 * @brief LOG provided string to LOG location
 *
 * Log provided string to set destination
 *
 */
void LOGString(
  const char *pString)
{
  d_fnstart(D_DEBUG, NULL, "(pString %s)\n",
            pString);

  // Log to file
  if (LOGDest & LOG_DEST_FILE)
  {
    if (!LOGFile) goto Exit;

    // Send to file
    int Res = fprintf(LOGFile, "%s", pString);
    if (Res < 0)
    {
      d_error(D_ERR, NULL, "Logging to file failed (%d)\n", Res);
    }
  }

  // Log to stdout
  if (LOGDest & LOG_DEST_STDOUT)
  {
    // Send to stdout
    int Res = fprintf(stdout, "%s", pString);
    if (Res < 0)
    {
      d_error(D_ERR, NULL, "Logging to stdout failed (%d)\n", Res);
    }
  }

  // Log to UDP
  if (LOGDest & LOG_DEST_UDP)
  {
    int Res = UDPSend(strlen(pString), pString);
    if (Res < 0)
    {
      d_error(D_ERR, NULL, "Logging to UDP failed: %d\n", Res);
    }
  }

Exit:
  d_fnend(D_DEBUG, NULL, "()\n");
  return;
}



/**
 * @brief LOG provided BSM to LOG location
 *
 * Log provided BSM to set destination. An optional string can be
 * provided that is prepended to the output of the XER string.
 *
 */
void LOGBsm(
  const char *pString,
  DSRC_BasicSafetyMessage_t *pBsm)
{
  d_fnstart(D_DEBUG, NULL, "(pBsm %p)\n",
            pBsm);

  // Copy initial string
  strcpy(XerBuf, pString);

  XerBufIndex = XerBuf + strlen(pString);

  // Copy XER encoded message
  asn_enc_rval_t Rc = xer_encode(
                        &asn_DEF_DSRC_BasicSafetyMessage,
                        pBsm,
                        XER_F_BASIC,
                        LOGDumpToBuf,
                        NULL);

  // Null terminate
  *XerBufIndex = '\0';

  if (Rc.encoded == -1)
  {
    d_error(D_ERR, NULL, "xer-encode failed (%zd)\n", Rc.encoded);
  }

  // And log complete string
  LOGString(XerBuf);

  d_fnend(D_DEBUG, NULL, "(buf free %zd)\n",
          XerBuf + LOG_XER_BUF_SIZE - XerBufIndex);
  return;
}


/**
 * @brief LOG utility to buffer XER string
 *
 * Provide function for XER encoder to print XER format to buffer
 *
 */
static int LOGDumpToBuf(
  const void *buff,
  size_t Size,
  void *application_specific_key)
{
  if ((XerBufIndex + Size) < (XerBuf + LOG_XER_BUF_SIZE))
  {
    memcpy(XerBufIndex, buff, Size);
    XerBufIndex += Size;
    return 0;
  }
  else
  {
    // buffer overrun
    d_error(D_ERR, NULL, "LOG buffer overrun (%zd > %zd)\n",
            XerBufIndex - XerBuf,
            Size);
    return -1;
  }

}


// Close the doxygen group
/**
 * @}
 */
