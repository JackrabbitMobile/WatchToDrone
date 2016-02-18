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
 * @file ARNETWORK_Error.h
 * @brief libARNetwork errors known.
 * @date 05/18/2012
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARNETWORK_ERROR_H_
#define _ARNETWORK_ERROR_H_

/**
 * @brief libARNetwork errors known.
 */
typedef enum
{
    ARNETWORK_OK = 0, /**< No error */
    ARNETWORK_ERROR = -1000, /**< Unknown generic error */
    ARNETWORK_ERROR_ALLOC, /**< Memory allocation error */
    ARNETWORK_ERROR_BAD_PARAMETER, /**< Bad parameters */
    ARNETWORK_ERROR_ID_UNKNOWN, /**< Given IOBuffer identifier is unknown */
    ARNETWORK_ERROR_BUFFER_SIZE, /**< Insufficient free space in the buffer */
    ARNETWORK_ERROR_BUFFER_EMPTY, /**< Buffer is empty, nothing was read */
    ARNETWORK_ERROR_SEMAPHORE, /**< Error when using a semaphore */
    ARNETWORK_ERROR_MUTEX, /**< Error when using a mutex */
    ARNETWORK_ERROR_MUTEX_DOUBLE_LOCK, /**< A mutex is already locked by the same thread */
    ARNETWORK_ERROR_MANAGER = -2000, /**< Unknown ARNETWORK_Manager error */
    ARNETWORK_ERROR_MANAGER_NEW_IOBUFFER, /**< IOBuffer creation error */
    ARNETWORK_ERROR_MANAGER_NEW_SENDER, /**< Sender creation error */
    ARNETWORK_ERROR_MANAGER_NEW_RECEIVER, /**< Receiver creation error */
    ARNETWORK_ERROR_NEW_BUFFER, /**< Buffer creation error */
    ARNETWORK_ERROR_NEW_RINGBUFFER, /**< RingBuffer creation error */
    ARNETWORK_ERROR_IOBUFFER = -3000, /**< Unknown IOBuffer error */
    ARNETWORK_ERROR_IOBUFFER_BAD_ACK,  /**< Bad sequence number for the acknowledge */
    ARNETWORK_ERROR_RECEIVER = -5000, /**< Unknown Receiver error */
    ARNETWORK_ERROR_RECEIVER_BUFFER_END, /**< Receiver buffer too small */
    ARNETWORK_ERROR_RECEIVER_BAD_FRAME, /**< Bad frame content on network */

} eARNETWORK_ERROR;

/**
 * @brief Gets the error string associated with an eARNETWORK_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARNETWORK_Error_ToString (eARNETWORK_ERROR error);

#endif /* _ARNETWORK_ERROR_H_ */
