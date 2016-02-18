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
 * @file ARCONTROLLER_StreamQueue.h
 * @brief ARCONTROLLER_StreamQueue
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_STREAM_QUEUE_H_
#define _ARCONTROLLER_STREAM_QUEUE_H_

#include <stdint.h>

#include <libARController/ARCONTROLLER_Error.h>
#include <libARController/ARCONTROLLER_Frame.h>
#include <libARController/ARCONTROLLER_StreamPool.h>

typedef struct ARCONTROLLER_StreamQueue_t ARCONTROLLER_StreamQueue_t;

/**
 * @brief Create a new Stream Queue.
 * @warning This function allocate memory.
 * @post ARCONTROLLER_StreamQueue_Delete() must be called to delete the Stream Queue and free the memory allocated.
 * @param[in] flushOnIFrame Flag to activate the flush of the frame queue when a IFrame is pushed ; '1' the flush is activated ; '0' the flush is  not activated.
 * @param[out] error Executing error.
 * @return The new Stream Queue.
 * @see ARCONTROLLER_StreamQueue_Delete.
 */
ARCONTROLLER_StreamQueue_t *ARCONTROLLER_StreamQueue_New (int flushOnIFrame, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Stream Queue.
 * @warning This function free memory.
 * @param streamQueue The Stream Queue to delete.
 * @see ARCONTROLLER_StreamQueue_New().
 */
void ARCONTROLLER_StreamQueue_Delete (ARCONTROLLER_StreamQueue_t **streamQueue);

/**
 * @brief Push a frame to the queue.
 * @param streamQueue The Stream Queue.
 * @param[in] frame The frame to push.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_StreamQueue_Push (ARCONTROLLER_StreamQueue_t *streamQueue, ARCONTROLLER_Frame_t *frame);

/**
 * @brief Pop a frame from queue.
 * @warning This is a blocking function, waits a frame push if the queue is empty. 
 * @param streamQueue The Stream Queue.
 * @param[out] error Executing error.
 * @return The frame pop ; Can be null if an error occurs.
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_StreamQueue_Pop (ARCONTROLLER_StreamQueue_t *streamQueue, eARCONTROLLER_ERROR *error);

/**
 * @brief Try to pop a frame from queue.
 * @note If the queue is empty, returns null, and error is equals to ARCONTROLLER_ERROR_STREAMQUEUE_EMPTY.
 * @param streamQueue The Stream Queue.
 * @param[out] error Executing error.
 * @return The frame pop ; Can be null if an error occurs.
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_StreamQueue_TryPop (ARCONTROLLER_StreamQueue_t *streamQueue, eARCONTROLLER_ERROR *error);

/**
 * @brief Pop a frame from queue, with timeout.
 * @warning This is a blocking function; If the queue is empty yet after the timeout, returns null, and error is equals to ARCONTROLLER_ERROR_STREAMQUEUE_EMPTY.
 * @param streamQueue The Stream Queue.
 * @param[out] error Executing error.
 * @return The frame pop ; Can be null if an error occurs.
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_StreamQueue_PopWithTimeout (ARCONTROLLER_StreamQueue_t *streamQueue, uint32_t timeoutMs, eARCONTROLLER_ERROR *error);

/**
 * @brief Remove all frame of the queue.
 * @param streamQueue The Stream Queue.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_StreamQueue_Flush (ARCONTROLLER_StreamQueue_t *streamQueue);

#endif /* _ARCONTROLLER_STREAM_QUEUE_H_ */
