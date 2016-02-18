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
 * @file ARCONTROLLER_Frame.h
 * @brief ARCONTROLLER_Frame
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_FRAME_H_
#define _ARCONTROLLER_FRAME_H_

#include <stdint.h>

#define ARCONTROLLER_FRAME_DEFAULT_CAPACITY 60000 /**< Default capacity of a new frame */

/**
 * @brief Frame used to store stream data
 */
typedef struct 
{
    uint8_t *data; /**< Data of the frame */
    uint32_t capacity; /**< Data capacity */
    uint32_t used; /**< size of the data */
    uint32_t missed;
    uint32_t width;
    uint32_t height;
    int isIFrame; /**< Flag to inform if the frame is an IFrame ; '1' is an IFrame ; '0' is not an IFrame */
    int available; /**< Flag to inform if the frame is available ; '1' the frame is free ; '0' the frame is not available */
    uint8_t *base; /**< Data not modified */
}ARCONTROLLER_Frame_t;

/**
 * @brief Create a new frame
 * @warning This function allocate memory
 * @post ARCONTROLLER_Frame_Delete() must be called to delete the frame and free the memory allocated.
 * @note The capacity of the data frame wil be ARCONTROLLER_FRAME_DEFAULT_CAPACITY.
 * @param[out] error Executing error.
 * @return the new frame
 * @see ARCONTROLLER_Frame_NewWithCapacity()
 * @see ARCONTROLLER_Frame_Delete()
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_Frame_New (eARCONTROLLER_ERROR *error);

/**
 * @brief Create a new frame with specific capacity
 * @warning This function allocate memory
 * @post ARCONTROLLER_Frame_Delete() must be called to delete the frame and free the memory allocated.
 * @param[in] defaultCapacity The default capacity of the frame.
 * @param[out] error Executing error.
 * @return the new frame.
 * @see ARCONTROLLER_Frame_New()
 * @see ARCONTROLLER_Frame_Delete()
 */
ARCONTROLLER_Frame_t *ARCONTROLLER_Frame_NewWithCapacity (uint32_t defaultCapacity, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the frame.
 * @warning This function free memory.
 * @param frame The frame to delete.
 * @see ARCONTROLLER_Frame_New().
 * @see ARCONTROLLER_Frame_NewWithCapacity().
 */
void ARCONTROLLER_Frame_Delete (ARCONTROLLER_Frame_t **frame);

/**
 * @brief Ensure the Capacity of the frame Is At Least 'minimumCapacity'.
 * @note If the capacity is too small, data frame will be re-allocated to the good Capacity.
 * @param frame The frame to delete.
 * @param[out] error Executing error.
 * @param[in] minimumCapacity The minimum capacity that the frame should have.
 * @return '1' is the capacity is at least 'minimumCapacity', otherwise '0'
 */
int ARCONTROLLER_Frame_ensureCapacityIsAtLeast (ARCONTROLLER_Frame_t *frame, uint32_t minimumCapacity, eARCONTROLLER_ERROR *error);

/**
 * @brief Free a frame.
 * @param frame The frame.r.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Frame_SetFree (ARCONTROLLER_Frame_t *frame);

#endif /* _ARCONTROLLER_FRAME_H_ */
