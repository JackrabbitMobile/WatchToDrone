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
 * @file ARCONTROLLER_StreamPool.h
 * @brief ARCONTROLLER_StreamPool
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_STREAM_POOL_H_
#define _ARCONTROLLER_STREAM_POOL_H_

#include <stdint.h>

#include <libARController/ARCONTROLLER_Error.h>
#include <libARController/ARCONTROLLER_Frame.h>

#define ARCONTROLLER_STREAMPOOL_DEFAULT_SIZE 40

typedef struct ARCONTROLLER_StreamPool_t ARCONTROLLER_StreamPool_t;

/**
 * @brief Create a new Stream Pool.
 * @warning This function allocate memory.
 * @post ARCONTROLLER_StreamPool_Delete() must be called to delete the Stream Pool and free the memory allocated.
 * @param[in] capacity Capacity of the pool.
 * @param[out] error Executing error.
 * @return The new Stream Pool.
 * @see ARCONTROLLER_StreamPool_Delete.
 */
ARCONTROLLER_StreamPool_t *ARCONTROLLER_StreamPool_New (uint32_t capacity, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Stream Pool.
 * @warning This function free memory.
 * @param streamPool The Stream Pool to delete.
 * @see ARCONTROLLER_StreamPool_New().
 */
void ARCONTROLLER_StreamPool_Delete (ARCONTROLLER_StreamPool_t **streamPool);

/**
 * @brief Return the first free frmae of the pool.
 * @param streamPool The Stream Pool.
 * @param[out] error Executing error.
 * @return The new Stream Pool.
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_StreamPool_GetNextFreeFrame (ARCONTROLLER_StreamPool_t *streamPool, eARCONTROLLER_ERROR *error);

/**
 * @brief Return the first free frmae of the pool.
 * @param streamPool The Stream Pool.
 * @param[out] error Executing error.
 * @return The new Stream Pool.
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_StreamPool_GetFrameFromData (ARCONTROLLER_StreamPool_t *streamPool, uint8_t *frameData, eARCONTROLLER_ERROR *error);

#endif /* _ARCONTROLLER_STREAM_POOL_H_ */
