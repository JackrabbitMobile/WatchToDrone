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
 * @file libARDataTransfer/ARDATATRANSFER_Error.h
 * @brief libARDataTransfer error header. This file contains all error of ARDataTransfer
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _LIBARDATATRANSFER_ERROR_H_
#define _LIBARDATATRANSFER_ERROR_H_

/**
 * @brief libARDataTransfer errors known.
 */
typedef enum
{
    ARDATATRANSFER_OK = 0, /**< No error */
    ARDATATRANSFER_ERROR = -1000, /**< Unknown generic error */
    ARDATATRANSFER_ERROR_ALLOC, /**< Memory allocation error */
    ARDATATRANSFER_ERROR_BAD_PARAMETER, /**< Bad parameters error */
    ARDATATRANSFER_ERROR_NOT_INITIALIZED, /**< Not initialized error */
    ARDATATRANSFER_ERROR_ALREADY_INITIALIZED, /**< Already initialized error */
    ARDATATRANSFER_ERROR_THREAD_ALREADY_RUNNING, /**< Thread already running error */
    ARDATATRANSFER_ERROR_THREAD_PROCESSING, /**< Thread processing error */
    ARDATATRANSFER_ERROR_CANCELED, /**< Canceled received */
    ARDATATRANSFER_ERROR_SYSTEM, /**< System error */
    ARDATATRANSFER_ERROR_FTP, /**< Ftp error */
    ARDATATRANSFER_ERROR_FILE, /**< File error */
    
} eARDATATRANSFER_ERROR;


/**
 * @brief Gets the error string associated with an eARDATATRANSFER_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARDATATRANSFER_Error_ToString (eARDATATRANSFER_ERROR error);

#endif /* _LIBARDATATRANSFER_ERROR_H_ */
