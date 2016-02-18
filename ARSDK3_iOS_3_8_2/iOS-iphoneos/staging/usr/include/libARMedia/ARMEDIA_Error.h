/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file ARMEDIA_Error.h
 * @brief libARMedia errors known.
 * @date 29/03/14
 * @author frederic.dhaeyer@parrot.com
 */
#ifndef _ARMEDIA_ERROR_H_
#define _ARMEDIA_ERROR_H_

/**
 * @brief libARMedia errors known.
 */
typedef enum
{
    ARMEDIA_OK = 0, /**< No error */
    ARMEDIA_ERROR = -1000, /**< Unknown generic error */
    ARMEDIA_ERROR_BAD_PARAMETER, /**< Bad parameters */
    ARMEDIA_ERROR_NOT_IMPLEMENTED, /**< Function not implemented */
    ARMEDIA_ERROR_MANAGER = -2000, /**< Unknown manager error */
    ARMEDIA_ERROR_MANAGER_ALREADY_INITIALIZED, /**< Error manager already initialized */
    ARMEDIA_ERROR_MANAGER_NOT_INITIALIZED, /**< Error manager not initialized */
    ARMEDIA_ERROR_ENCAPSULER = -3000, /**< Error encapsuler generic error */
    ARMEDIA_ERROR_ENCAPSULER_WAITING_FOR_IFRAME, /**< Error encapsuler waiting for i-frame */
    ARMEDIA_ERROR_ENCAPSULER_BAD_CODEC, /**< Codec non-supported */
    ARMEDIA_ERROR_ENCAPSULER_BAD_VIDEO_FRAME, /**< Error in video frame header */
    ARMEDIA_ERROR_ENCAPSULER_BAD_VIDEO_SAMPLE, /**< Error in audio sample header */
    ARMEDIA_ERROR_ENCAPSULER_FILE_ERROR, /**< File error while encapsulating */
    ARMEDIA_ERROR_ENCAPSULER_BAD_TIMESTAMP, /**< Timestamp is before previous sample */

} eARMEDIA_ERROR;

/**
 * @brief Gets the error string associated with an eARMEDIA_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARMEDIA_Error_ToString (eARMEDIA_ERROR error);

#endif /* _ARMEDIA_ERROR_H_ */
