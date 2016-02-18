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
 * @file ARSTREAM_Reader.h
 * @brief Stream reader on network
 * @date 03/22/2013
 * @author nicolas.brulez@parrot.com
 */

#ifndef _ARSTREAM_READER_H_
#define _ARSTREAM_READER_H_

/*
 * System Headers
 */
#include <inttypes.h>

/*
 * ARSDK Headers
 */
#include <libARNetwork/ARNETWORK_Manager.h>
#include <libARStream/ARSTREAM_Error.h>

/*
 * Macros
 */
#define ARSTREAM_READER_MAX_ACK_INTERVAL_DEFAULT (5)

/*
 * Types
 */

/**
 * @brief Causes for FrameComplete callback
 */
typedef enum {
    ARSTREAM_READER_CAUSE_FRAME_COMPLETE = 0, /**< Frame is complete (no error) */
    ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL, /**< Frame buffer is too small for the frame on the network */
    ARSTREAM_READER_CAUSE_COPY_COMPLETE, /**< Copy of previous frame buffer is complete (called only after ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL) */
    ARSTREAM_READER_CAUSE_CANCEL, /**< Reader is closing, so buffer is no longer used */
    ARSTREAM_READER_CAUSE_MAX,
} eARSTREAM_READER_CAUSE;

/**
 * @brief Callback called when a new frame is ready in a buffer
 *
 * @param[in] cause Describes why this callback was called
 * @param[in] framePointer Pointer to the frame buffer which was received
 * @param[in] frameSize Used size in framePointer buffer
 * @param[in] isFlushFrame Boolean-like (0-1) flag telling if the complete frame was a flush frame (typically an I-Frame) for the sender
 * @param[in] numberOfSkippedFrames Number of frames which were skipped between the previous call and this one. (Usually 0)
 * @param[inout] newBufferCapacity Capacity of the next buffer to use
 * @param[in] custom Custom pointer passed during ARSTREAM_Reader_New
 *
 * @return address of a new buffer which will hold the next frame
 *
 * @note If cause is ARSTREAM_READER_CAUSE_FRAME_COMPLETE, framePointer contains a valid frame.
 * @note If cause is ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL, datas will be copied into the new frame. Old frame buffer will still be in use until the callback is called again with ARSTREAM_READER_CAUSE_COPY_COMPLETE cause. If the new frame is still too small, the callback will be called again, until a suitable buffer is provided. newBufferCapacity holds a suitable capacity for the new buffer, but still has to be updated by the application.
 * @note If cause is ARSTREAM_READER_CAUSE_COPY_COMPLETE, the return value and newBufferCapacity are unused. If numberOfSkippedFrames is non-zero, then the current frame will be skipped (usually because the buffer returned after the ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL was smaller than the previous buffer).
 * @note If cause is ARSTREAM_READER_CAUSE_CANCEL, the return value and newBufferCapacity are unused
 *
 * @warning If the cause is ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL, returning a buffer shorter than 'frameSize' will cause the library to skip the current frame
 * @warning In any case, returning a NULL buffer is not supported.
 */
typedef uint8_t* (*ARSTREAM_Reader_FrameCompleteCallback_t) (eARSTREAM_READER_CAUSE cause, uint8_t *framePointer, uint32_t frameSize, int numberOfSkippedFrames, int isFlushFrame, uint32_t *newBufferCapacity, void *custom);

/**
 * @brief An ARSTREAM_Reader_t instance allow reading streamed frames from a network
 */
typedef struct ARSTREAM_Reader_t ARSTREAM_Reader_t;

/*
 * Functions declarations
 */

/**
 * @brief Sets an ARNETWORK_IOBufferParam_t to describe a stream data buffer
 * @param[in] bufferParams Pointer to the ARNETWORK_IOBufferParam_t to set
 * @param[in] bufferID ID to set in the ARNETWORK_IOBufferParam_t
 * @param[in] maxFragmentSize Maximum allowed size for a video data fragment. Video frames larger that will be fragmented.
 * @param[in] maxNumberOfFragment number maximum of fragment of one frame.
 */
void ARSTREAM_Reader_InitStreamDataBuffer (ARNETWORK_IOBufferParam_t *bufferParams, int bufferID, int maxFragmentSize, uint32_t maxNumberOfFragment);

/**
 * @brief Sets an ARNETWORK_IOBufferParam_t to describe a stream ack buffer
 * @param[in] bufferParams Pointer to the ARNETWORK_IOBufferParam_t to set
 * @param[in] bufferID ID to set in the ARNETWORK_IOBufferParam_t
 */
void ARSTREAM_Reader_InitStreamAckBuffer (ARNETWORK_IOBufferParam_t *bufferParams, int bufferID);

/**
 * @brief Creates a new ARSTREAM_Reader_t
 * @warning This function allocates memory. An ARSTREAM_Reader_t muse be deleted by a call to ARSTREAM_Reader_Delete
 *
 * @param[in] manager Pointer to a valid and connected ARNETWORK_Manager_t, which will be used to stream frames
 * @param[in] dataBufferID ID of a StreamDataBuffer available within the manager
 * @param[in] ackBufferID ID of a StreamAckBuffer available within the manager
 * @param[in] callback The callback which will be called every time a new frame is available
 * @param[in] frameBuffer The adress of the first frameBuffer to use
 * @param[in] frameBufferSize The length of the frameBuffer (to avoid overflow)
 * @param[in] maxFragmentSize Maximum allowed size for a video data fragment. Video frames larger that will be fragmented.
 * @param[in] maxAckInterval Maximum interval between sending ACKs. 0 disables only periodic ACKs. -1 disables ACKs completely.
 * If unsure, use the default value in ARSTREAM_READER_MAX_ACK_INTERVAL_DEFAULT.
 * @param[in] custom Custom pointer which will be passed to callback
 * @param[out] error Optionnal pointer to an eARSTREAM_ERROR to hold any error information
 * @return A pointer to the new ARSTREAM_Reader_t, or NULL if an error occured
 * @see ARSTREAM_Reader_InitStreamDataBuffer()
 * @see ARSTREAM_Reader_InitStreamAckBuffer()
 * @see ARSTREAM_Reader_StopReader()
 * @see ARSTREAM_Reader_Delete()
 */
ARSTREAM_Reader_t* ARSTREAM_Reader_New (ARNETWORK_Manager_t *manager, int dataBufferID, int ackBufferID, ARSTREAM_Reader_FrameCompleteCallback_t callback, uint8_t *frameBuffer, uint32_t frameBufferSize, uint32_t maxFragmentSize, int32_t maxAckInterval, void *custom, eARSTREAM_ERROR *error);

/**
 * @brief Stops a running ARSTREAM_Reader_t
 * @warning Once stopped, an ARSTREAM_Reader_t can not be restarted
 *
 * @param[in] reader The ARSTREAM_Reader_t to stop
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Reader_StopReader (ARSTREAM_Reader_t *reader);

/**
 * @brief Deletes an ARSTREAM_Reader_t
 * @warning This function should NOT be called on a running ARSTREAM_Reader_t
 *
 * @param reader Pointer to the ARSTREAM_Reader_t * to delete
 *
 * @return ARSTREAM_OK if the ARSTREAM_Reader_t was deleted
 * @return ARSTREAM_ERROR_BUSY if the ARSTREAM_Reader_t is still busy and can not be stopped now (probably because ARSTREAM_Reader_StopReader() was not called yet)
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if reader does not point to a valid ARSTREAM_Reader_t
 *
 * @note The library use a double pointer, so it can set *reader to NULL after freeing it
 */
eARSTREAM_ERROR ARSTREAM_Reader_Delete (ARSTREAM_Reader_t **reader);

/**
 * @brief Runs the data loop of the ARSTREAM_Reader_t
 * @warning This function never returns until ARSTREAM_Reader_StopReader() is called. Thus, it should be called on its own thread
 * @post Stop the ARSTREAM_Reader_t by calling ARSTREAM_Reader_StopReader() before joining the thread calling this function
 * @param[in] ARSTREAM_Reader_t_Param A valid (ARSTREAM_Reader_t *) casted as a (void *)
 */
void* ARSTREAM_Reader_RunDataThread (void *ARSTREAM_Reader_t_Param);

/**
 * @brief Runs the acknowledge loop of the ARSTREAM_Reader_t
 * @warning This function never returns until ARSTREAM_Reader_StopReader() is called. Thus, it should be called on its own thread
 * @post Stop the ARSTREAM_Reader_t by calling ARSTREAM_Reader_StopReader() before joining the thread calling this function
 * @param[in] ARSTREAM_Reader_t_Param A valid (ARSTREAM_Reader_t *) casted as a (void *)
 */
void* ARSTREAM_Reader_RunAckThread (void *ARSTREAM_Reader_t_Param);

/**
 * @brief Gets the estimated network efficiency for the ARSTREAM link
 * An efficiency of 1.0f means that we did not receive any useless packet.
 * Efficiency is computed on all frames for which the Reader got at least a packet, even if the frame was not complete.
 * @warning This function is a debug-only function and will disappear on release builds
 * @param[in] reader The ARSTREAM_Reader_t
 */
float ARSTREAM_Reader_GetEstimatedEfficiency (ARSTREAM_Reader_t *reader);

/**
 * @brief Gets the custom pointer associated with the reader
 * @param[in] reader The ARSTREAM_Reader_t
 * @return The custom pointer associated with this reader, or NULL if reader does not point to a valid reader
 */
void* ARSTREAM_Reader_GetCustom (ARSTREAM_Reader_t *reader);

#endif /* _ARSTREAM_READER_H_ */
