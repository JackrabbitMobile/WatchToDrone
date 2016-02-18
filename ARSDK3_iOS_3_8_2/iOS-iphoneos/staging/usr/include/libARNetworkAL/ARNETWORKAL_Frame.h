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
 * @file ARNETWORKAL_Frame.h
 * @brief define the network frame protocol
 * @date 04/30/2013
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARNETWORKAL_FRAME_H_
#define _ARNETWORKAL_FRAME_H_

#include <inttypes.h>

/**
 * @brief type of frame send by the ARNETWORKAL_Manager
 */
typedef enum
{
    ARNETWORKAL_FRAME_TYPE_UNINITIALIZED = 0, /**< Unknown type. Don't use */
    ARNETWORKAL_FRAME_TYPE_ACK, /**< Acknowledgment type. Internal use only */
    ARNETWORKAL_FRAME_TYPE_DATA, /**< Data type. Main type for data that does not require an acknowledge */
    ARNETWORKAL_FRAME_TYPE_DATA_LOW_LATENCY, /**< Low latency data type. Should only be used when needed */
    ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK, /**< Data that should have an acknowledge type. This type can have a long latency */
    ARNETWORKAL_FRAME_TYPE_MAX, /**< Unused, iterator maximum value */
}eARNETWORKAL_FRAME_TYPE;

/**
 * @brief Internal media-independant frame representation used by the ARNETWORKAL_Manager.
 * @warning The actual format of the frames exchanged on the network may differ.
 */
typedef struct
{
    uint8_t type; /**< frame type eARNETWORK_FRAME_TYPE */
    uint8_t id; /**< identifier of the buffer sending the frame */
    uint8_t seq; /**< sequence number of the frame */
    uint32_t size; /**< size of the frame */
    uint8_t *dataPtr; /**< pointer on the data of the frame */
}
// Doxygen does not like the __attribute__ tag
#ifndef DOXYGEN
__attribute__((__packed__))
#endif
ARNETWORKAL_Frame_t;

#endif /** _ARNETWORKAL_FRAME_H_ */
