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
 * @file ARSTREAM_Sender.h
 * @brief Stream sender over network
 * @date 03/21/2013
 * @author nicolas.brulez@parrot.com
 */

#ifndef _ARSTREAM_SENDER_H_
#define _ARSTREAM_SENDER_H_

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

/*
 * Types
 */

/**
 * @brief Callback status values
 */
typedef enum {
    ARSTREAM_SENDER_STATUS_FRAME_SENT = 0, /**< Frame was sent and acknowledged by peer */
    ARSTREAM_SENDER_STATUS_FRAME_CANCEL, /**< Frame was not sent, and was cancelled by a new frame */
    ARSTREAM_SENDER_STATUS_FRAME_LATE_ACK, /**< We received a full ack for an old frame. The callback will be called with null pointer and zero size. */
    ARSTREAM_SENDER_STATUS_MAX,
} eARSTREAM_SENDER_STATUS;

/**
 * @brief Callback type for sender informations
 * This callback is called when a frame pointer is no longer needed by the library.
 * This can occur when a frame is acknowledged, cancelled, or if a network error happened.
 *
 * This callback is also used when we receive the first "full-ack" for an old frame. In
 * this case, the framePointer and frameSize arguments are unused and set to NULL. There
 * is no way to identify the "old" frame, but the library guarantees that the LATE_ACK status
 * will only be called for previously cancelled frames, and at most once per cancelled frame.
 *
 * @param[in] status Why the call was made
 * @param[in] framePointer Pointer to the frame which was sent/cancelled
 * @param[in] frameSize Size, in bytes, of the frame
 * @param[in] custom Custom pointer passed during ARSTREAM_Sender_New
 * @warning If the status is ARSTREAM_SENDER_STATUS_FRAME_LATE_ACK, then the framePointer will be NULL.
 * @see eARSTREAM_SENDER_STATUS
 */
typedef void (*ARSTREAM_Sender_FrameUpdateCallback_t)(eARSTREAM_SENDER_STATUS status, uint8_t *framePointer, uint32_t frameSize, void *custom);

/**
 * @brief An ARSTREAM_Sender_t instance allow streaming frames over a network
 */
typedef struct ARSTREAM_Sender_t ARSTREAM_Sender_t;

/**
 * @brief Default minimum wait time for ARSTREAM_Sender_SetTimeBetweenRetries calls
 */
#define ARSTREAM_SENDER_DEFAULT_MINIMUM_TIME_BETWEEN_RETRIES_MS (15)
/**
 * @brief Default maximum wait time for ARSTREAM_Sender_SetTimeBetweenRetries calls
 */
#define ARSTREAM_SENDER_DEFAULT_MAXIMUM_TIME_BETWEEN_RETRIES_MS (50)
/**
 * @brief "Infinite" time.
 * Use this time as both minimum and maximum time in ARSTREAM_Sender_SetTimeBetweenRetries
 * to disable retries.
 * @warning Disabling the retry system will lower the reliability of the stream!
 */
#define ARSTREAM_SENDER_INFINITE_TIME_BETWEEN_RETRIES (100000)



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
void ARSTREAM_Sender_InitStreamDataBuffer (ARNETWORK_IOBufferParam_t *bufferParams, int bufferID, int maxFragmentSize, uint32_t maxFragmentPerFrame);

/**
 * @brief Sets an ARNETWORK_IOBufferParam_t to describe a stream ack buffer
 * @param[in] bufferParams Pointer to the ARNETWORK_IOBufferParam_t to set
 * @param[in] bufferID ID to set in the ARNETWORK_IOBufferParam_t
 */
void ARSTREAM_Sender_InitStreamAckBuffer (ARNETWORK_IOBufferParam_t *bufferParams, int bufferID);

/**
 * @brief Creates a new ARSTREAM_Sender_t
 * @warning This function allocates memory. An ARSTREAM_Sender_t muse be deleted by a call to ARSTREAM_Sender_Delete
 *
 * @param[in] manager Pointer to a valid and connected ARNETWORK_Manager_t, which will be used to stream frames
 * @param[in] dataBufferID ID of a StreamDataBuffer available within the manager
 * @param[in] ackBufferID ID of a StreamAckBuffer available within the manager
 * @param[in] callback The status update callback which will be called every time the status of a send-frame is updated
 * @param[in] framesBufferSize Number of frames that the ARSTREAM_Sender_t instance will be able to hold in queue
 * @param[in] maxFragmentSize Maximum allowed size for a video data fragment. Video frames larger that will be fragmented.
 * @param[in] maxNumberOfFragment number maximum of fragment of one frame.
 * @param[in] custom Custom pointer which will be passed to callback
 * @param[out] error Optionnal pointer to an eARSTREAM_ERROR to hold any error information
 * @return A pointer to the new ARSTREAM_Sender_t, or NULL if an error occured
 *
 * @note framesBufferSize should be greater than the number of frames between two I-Frames
 *
 * @see ARSTREAM_Sender_InitStreamDataBuffer()
 * @see ARSTREAM_Sender_InitStreamAckBuffer()
 * @see ARSTREAM_Sender_StopSender()
 * @see ARSTREAM_Sender_Delete()
 */
ARSTREAM_Sender_t* ARSTREAM_Sender_New (ARNETWORK_Manager_t *manager, int dataBufferID, int ackBufferID, ARSTREAM_Sender_FrameUpdateCallback_t callback, uint32_t framesBufferSize, uint32_t maxFragmentSize, uint32_t maxNumberOfFragment,  void *custom, eARSTREAM_ERROR *error);

/**
 * @brief Sets the minimum and maximum time between retries.
 * Setting a small retry time might increase reliability, at the cost of network and cpu loads.
 * Setting a high retry time might decrease reliability, but also reduce the network and cpu loads.
 * These rules apply to both the minimum and the maximum time.
 *
 * The library selects a wait time between the two bounds using data retrieved from the ARNETWORK_Manager_t.
 *
 * If the minimum and maximum wait times are equal, then the library will always use this time.
 *
 * @note To reset to default values, use ARSTREAM_SENDER_DEFAULT_MINIMUM_TIME_BETWEEN_RETRIES_MS and ARSTREAM_SENDER_DEFAULT_MAXIMUM_TIME_BETWEEN_RETRIES_MS.
 * @note To disable retries, use ARSTREAM_SENDER_INFINITE_TIME_BETWEEN_RETRIES as both minimum and maximum time.
 * @warning Setting a too low maximum wait time might create a very high network bandwidth, and a very high cpu load.
 * @warning Setting a too big minimum wait time (i.e. greater than the time between two flush frames) will effectively disable retries.
 * @param minWaitTimeMs The minimum time between two retries, in miliseconds.
 * @param maxWaitTimeMs The maximum time between two retries, in miliseconds.
 *
 * @return ARSTREAM_OK if the new time range is set.
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if sender is NULL, or if minWaitTimeMs is greater than maxWaitTimeMs.
 */
eARSTREAM_ERROR ARSTREAM_Sender_SetTimeBetweenRetries (ARSTREAM_Sender_t *sender, int minWaitTimeMs, int maxWaitTimeMs);

/**
 * @brief Stops a running ARSTREAM_Sender_t
 * @warning Once stopped, an ARSTREAM_Sender_t can not be restarted
 *
 * @param[in] sender The ARSTREAM_Sender_t to stop
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Sender_StopSender (ARSTREAM_Sender_t *sender);

/**
 * @brief Deletes an ARSTREAM_Sender_t
 * @warning This function should NOT be called on a running ARSTREAM_Sender_t
 *
 * @param sender Pointer to the ARSTREAM_Sender_t * to delete
 *
 * @return ARSTREAM_OK if the ARSTREAM_Sender_t was deleted
 * @return ARSTREAM_ERROR_BUSY if the ARSTREAM_Sender_t is still busy and can not be stopped now (probably because ARSTREAM_Sender_StopSender() was not called yet)
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if sender does not point to a valid ARSTREAM_Sender_t
 *
 * @note The library use a double pointer, so it can set *sender to NULL after freeing it
 */
eARSTREAM_ERROR ARSTREAM_Sender_Delete (ARSTREAM_Sender_t **sender);

/**
 * @brief Sends a new frame
 *
 * @param[in] sender The ARSTREAM_Sender_t which will try to send the frame
 * @param[in] frameBuffer pointer to the frame in memory
 * @param[in] frameSize size of the frame in memory
 * @param[in] flushPreviousFrames Boolean-like flag (0/1). If active, tells the sender to flush the frame queue when adding this frame.
 * @param[out] nbPreviousFrames Optionnal int pointer which will store the number of frames previously in the buffer (even if the buffer is flushed)
 * @return ARSTREAM_OK if no error happened
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender or frameBuffer pointer is invalid, or if frameSize is zero
 * @return ARSTREAM_ERROR_FRAME_TOO_LARGE if the frameSize is greater that the maximum frame size of the libARStream (typically 128000 bytes)
 * @return ARSTREAM_ERROR_QUEUE_FULL if the frame can not be added to queue. This value can not happen if flushPreviousFrames is active
 */
eARSTREAM_ERROR ARSTREAM_Sender_SendNewFrame (ARSTREAM_Sender_t *sender, uint8_t *frameBuffer, uint32_t frameSize, int flushPreviousFrames, int *nbPreviousFrames);

/**
 * @brief Flushes all currently queued frames
 *
 * @param[in] sender The ARSTREAM_Sender_t to be flushed.
 * @return ARSTREAM_OK if no error occured.
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender is invalid.
 */
eARSTREAM_ERROR ARSTREAM_Sender_FlushFramesQueue (ARSTREAM_Sender_t *sender);

/**
 * @brief Runs the data loop of the ARSTREAM_Sender_t
 * @warning This function never returns until ARSTREAM_Sender_StopSender() is called. Thus, it should be called on its own thread
 * @post Stop the ARSTREAM_Sender_t by calling ARSTREAM_Sender_StopSender() before joining the thread calling this function
 * @param[in] ARSTREAM_Sender_t_Param A valid (ARSTREAM_Sender_t *) casted as a (void *)
 */
void* ARSTREAM_Sender_RunDataThread (void *ARSTREAM_Sender_t_Param);

/**
 * @brief Runs the acknowledge loop of the ARSTREAM_Sender_t
 * @warning This function never returns until ARSTREAM_Sender_StopSender() is called. Thus, it should be called on its own thread
 * @post Stop the ARSTREAM_Sender_t by calling ARSTREAM_Sender_StopSender() before joining the thread calling this function
 * @param[in] ARSTREAM_Sender_t_Param A valid (ARSTREAM_Sender_t *) casted as a (void *)
 */
void* ARSTREAM_Sender_RunAckThread (void *ARSTREAM_Sender_t_Param);

/**
 * @brief Gets the estimated network efficiency for the ARSTREAM link
 * An efficiency of 1.0f means that we did not do any retries
 * @warning This function is a debug-only function and will disappear on release builds
 * @param[in] sender The ARSTREAM_Sender_t
 */
float ARSTREAM_Sender_GetEstimatedEfficiency (ARSTREAM_Sender_t *sender);

/**
 * @brief Gets the custom pointer associated with the sender
 * @param[in] sender The ARSTREAM_Sender_t
 * @return The custom pointer associated with this sender, or NULL if sender does not point to a valid sender
 */
void* ARSTREAM_Sender_GetCustom (ARSTREAM_Sender_t *sender);

#endif /* _ARSTREAM_SENDER_H_ */
