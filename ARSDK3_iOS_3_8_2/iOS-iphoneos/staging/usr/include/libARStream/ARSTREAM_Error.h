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
 * @file ARSTREAM_Error.h
 * @brief Error codes for ARSTREAM_xxx calls
 * @date 06/19/2013
 * @author nicolas.brulez@parrot.com
 */

#ifndef _ARSTREAM_ERROR_H_
#define _ARSTREAM_ERROR_H_

/*
 * System Headers
 */

/*
 * ARSDK Headers
 */

/*
 * Macros
 */

/*
 * Types
 */

/**
 * @brief Error codes for ARSTREAM_xxx calls
 */
typedef enum {
    ARSTREAM_OK = 0, /**< No error */
    ARSTREAM_ERROR_BAD_PARAMETERS, /**< Bad parameters */
    ARSTREAM_ERROR_ALLOC, /**< Unable to allocate data */
    ARSTREAM_ERROR_FRAME_TOO_LARGE, /**< Bad parameter : frame too large */
    ARSTREAM_ERROR_BUSY, /**< Object is busy and can not be deleted yet */
    ARSTREAM_ERROR_QUEUE_FULL, /**< Frame queue is full */
} eARSTREAM_ERROR;

/**
 * @brief Gets the error string associated with an eARSTREAM_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARSTREAM_Error_ToString (eARSTREAM_ERROR error);

#endif /* _ARSTREAM_ERROR_H_ */
