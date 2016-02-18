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
 * @file ARSTREAM_Sender2.h
 * @brief Stream sender over a network (v2)
 * @date 04/17/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM_SENDER2_H_
#define _ARSTREAM_SENDER2_H_

/*
 * System Headers
 */
#include <inttypes.h>

/*
 * ARSDK Headers
 */
#include <libARStream/ARSTREAM_Error.h>


/*
 * Macros
 */

/**
 * @brief Default server-side stream port
 */
#define ARSTREAM_SENDER2_DEFAULT_SERVER_STREAM_PORT     (5004)

/**
 * @brief Default server-side control port
 */
#define ARSTREAM_SENDER2_DEFAULT_SERVER_CONTROL_PORT    (5005)

/**
 * @brief Default H.264 NAL unit FIFO size
 */
#define ARSTREAM_SENDER2_DEFAULT_NALU_FIFO_SIZE         (1024)


/*
 * Types
 */

/**
 * @brief Callback status values
 */
typedef enum {
    ARSTREAM_SENDER2_STATUS_SENT = 0,   /**< Access unit or NAL unit was sent */
    ARSTREAM_SENDER2_STATUS_CANCELLED,  /**< Access unit or NAL unit was cancelled (not sent or partly sent) */
    ARSTREAM_SENDER2_STATUS_MAX,
} eARSTREAM_SENDER2_STATUS;

/**
 * @brief Callback function for access units
 * This callback function is called when buffers associated with an access unit are no longer used by the sender.
 * This occurs when packets corresponding to an access unit have all been sent or dropped.
 *
 * @param[in] status Why the call is made
 * @param[in] auUserPtr Access unit user pointer associated with the NAL units submitted to the sender
 * @param[in] userPtr Global access unit callback user pointer
 * @see eARSTREAM_SENDER2_STATUS
 */
typedef void (*ARSTREAM_Sender2_AuCallback_t) (eARSTREAM_SENDER2_STATUS status, void *auUserPtr, void *userPtr);

/**
 * @brief Callback function for NAL units
 * This callback function is called when a buffer associated with a NAL unit is no longer used by the sender.
 * This occurs when packets corresponding to a NAL unit have all been sent or dropped.
 *
 * @param[in] status Why the call is made
 * @param[in] naluUserPtr NAL unit user pointer associated with the NAL unit submitted to the sender
 * @param[in] userPtr Global NAL unit callback user pointer
 * @see eARSTREAM_SENDER2_STATUS
 */
typedef void (*ARSTREAM_Sender2_NaluCallback_t) (eARSTREAM_SENDER2_STATUS status, void *naluUserPtr, void *userPtr);

/**
 * @brief Sender2 configuration parameters
 */
typedef struct ARSTREAM_Sender2_Config_t
{
    const char *clientAddr;                         /**< Client address */
    const char *mcastAddr;                          /**< Multicast send address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast output interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    ARSTREAM_Sender2_AuCallback_t auCallback;       /**< Access unit callback function (optional, can be NULL) */
    void *auCallbackUserPtr;                        /**< Access unit callback function user pointer (optional, can be NULL) */
    ARSTREAM_Sender2_NaluCallback_t naluCallback;   /**< NAL unit callback function (optional, can be NULL) */
    void *naluCallbackUserPtr;                      /**< NAL unit callback function user pointer (optional, can be NULL) */
    int naluFifoSize;                               /**< NAL unit FIFO size, @see ARSTREAM_SENDER2_DEFAULT_NALU_FIFO_SIZE */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (example: the interface MTU) */
    int targetPacketSize;                           /**< Target network packet size in bytes */
    int maxBitrate;                                 /**< Maximum streaming bitrate in bit/s */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (optional, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds */

} ARSTREAM_Sender2_Config_t;

/**
 * @brief Sender2 NAL unit descriptor
 */
typedef struct ARSTREAM_Sender2_NaluDesc_t
{
    uint8_t *naluBuffer;                            /**< Pointer to the NAL unit buffer */
    uint32_t naluSize;                              /**< Size of the NAL unit in bytes */
    uint64_t auTimestamp;                           /**< Access unit timastamp in microseconds. All NAL units of an access unit must share the same timestamp */
    int isLastNaluInAu;                             /**< Boolean-like flag (0/1). If active, tells the sender that the NAL unit is the last of the access unit */
    int seqNumForcedDiscontinuity;                  /**< Force an added discontinuity in RTP sequence number before the NAL unit */
    void *auUserPtr;                                /**< Access unit user pointer that will be passed to the access unit callback function (optional, can be NULL) */
    void *naluUserPtr;                              /**< NAL unit user pointer that will be passed to the NAL unit callback function (optional, can be NULL) */

} ARSTREAM_Sender2_NaluDesc_t;

/**
 * @brief A Sender2 instance to allow streaming H.264 video over a network
 */
typedef struct ARSTREAM_Sender2_t ARSTREAM_Sender2_t;


/*
 * Functions declarations
 */

/**
 * @brief Creates a new Sender2
 * @warning This function allocates memory. The Sender2 must be deleted by a call to ARSTREAM_Sender2_Delete()
 *
 * @param[in] config Pointer to a configuration parameters structure
 * @param[out] error Optionnal pointer to an eARSTREAM_ERROR to hold any error information
 *
 * @return A pointer to the new ARSTREAM_Sender2_t, or NULL if an error occured
 *
 * @see ARSTREAM_Sender2_StopSender()
 * @see ARSTREAM_Sender2_Delete()
 */
ARSTREAM_Sender2_t* ARSTREAM_Sender2_New (ARSTREAM_Sender2_Config_t *config, eARSTREAM_ERROR *error);

/**
 * @brief Stops a running Sender2
 * @warning Once stopped, a Sender2 cannot be restarted
 *
 * @param[in] sender The Sender2 instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Sender2_StopSender (ARSTREAM_Sender2_t *sender);

/**
 * @brief Deletes a Sender2
 * @warning This function should NOT be called on a running Sender2
 *
 * @param sender Pointer to the ARSTREAM_Sender2_t* to delete
 *
 * @return ARSTREAM_OK if the Sender2 was deleted
 * @return ARSTREAM_ERROR_BUSY if the Sender2 is still busy and can not be stopped now (probably because ARSTREAM_Sender2_StopSender() has not been called yet)
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if sender does not point to a valid ARSTREAM_Sender2_t
 *
 * @note The function uses a double pointer, so it can set *sender to NULL after freeing it
 */
eARSTREAM_ERROR ARSTREAM_Sender2_Delete (ARSTREAM_Sender2_t **sender);

/**
 * @brief Sends a new NAL unit
 * @warning The NAL unit buffer must remain available for the sender until the NAL unit or access unit callback functions are called.
 *
 * @param[in] sender The Sender2 instance
 * @param[in] nalu Pointer to a NAL unit descriptor
 *
 * @return ARSTREAM_OK if no error happened
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender, nalu or naluBuffer pointers are invalid, or if naluSize or auTimestamp is zero
 * @return ARSTREAM_ERROR_QUEUE_FULL if the NAL unit FIFO is full
 */
eARSTREAM_ERROR ARSTREAM_Sender2_SendNewNalu (ARSTREAM_Sender2_t *sender, const ARSTREAM_Sender2_NaluDesc_t *nalu);

/**
 * @brief Sends multiple new NAL units
 * @warning The NAL unit buffers must remain available for the sender until the NAL unit or access unit callback functions are called.
 *
 * @param[in] sender The Sender2 instance
 * @param[in] nalu Pointer to a NAL unit descriptor array
 * @param[in] naluCount Number of NAL units in the array
 *
 * @return ARSTREAM_OK if no error happened
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender, nalu or naluBuffer pointers are invalid, or if a naluSize or auTimestamp is zero
 * @return ARSTREAM_ERROR_QUEUE_FULL if the NAL unit FIFO is full
 */
eARSTREAM_ERROR ARSTREAM_Sender2_SendNNewNalu (ARSTREAM_Sender2_t *sender, const ARSTREAM_Sender2_NaluDesc_t *nalu, int naluCount);

/**
 * @brief Flush all currently queued NAL units
 *
 * @param[in] sender The Sender2 instance
 *
 * @return ARSTREAM_OK if no error occured.
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender is invalid.
 */
eARSTREAM_ERROR ARSTREAM_Sender2_FlushNaluQueue (ARSTREAM_Sender2_t *sender);

/**
 * @brief Runs the stream loop of the Sender2
 * @warning This function never returns until ARSTREAM_Sender2_StopSender() is called. Thus, it should be called on its own thread.
 * @post Stop the Sender2 by calling ARSTREAM_Sender2_StopSender() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Sender2_t_Param A valid (ARSTREAM_Sender2_t *) casted as a (void *)
 */
void* ARSTREAM_Sender2_RunStreamThread (void *ARSTREAM_Sender2_t_Param);

/**
 * @brief Runs the control loop of the Sender2
 * @warning This function never returns until ARSTREAM_Sender2_StopSender() is called. Thus, it should be called on its own thread.
 * @post Stop the Sender2 by calling ARSTREAM_Sender2_StopSender() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Sender2_t_Param A valid (ARSTREAM_Sender2_t *) casted as a (void *)
 */
void* ARSTREAM_Sender2_RunControlThread (void *ARSTREAM_Sender2_t_Param);

/**
 * @brief Get the user pointer associated with the sender access unit callback function
 *
 * @param[in] sender The Sender2 instance
 *
 * @return The user pointer associated with the AU callback, or NULL if sender does not point to a valid sender
 */
void* ARSTREAM_Sender2_GetAuCallbackUserPtr (ARSTREAM_Sender2_t *sender);

/**
 * @brief Get the user pointer associated with the sender NAL unit callback function
 *
 * @param[in] sender The Sender2 instance
 * @return The user pointer associated with the NALU callback, or NULL if sender does not point to a valid sender
 */
void* ARSTREAM_Sender2_GetNaluCallbackUserPtr (ARSTREAM_Sender2_t *sender);

/**
 * @brief Get the stream monitoring
 * The monitoring data is computed form the time startTime and back timeIntervalUs microseconds at most.
 * If startTime is 0 the start time is the current time.
 * If monitoring data is not available up to timeIntervalUs, the monitoring is computed on less time and the real interval is output to realTimeIntervalUs.
 * Pointers to monitoring parameters that are not required can be left NULL.
 *
 * @param[in] sender The Sender2 instance
 * @param[in] startTime Monitoring start time in microseconds (0 means current time)
 * @param[in] timeIntervalUs Monitoring time interval (back from startTime) in microseconds
 * @param[out] realTimeIntervalUs Real monitoring time interval in microseconds (optional, can be NULL)
 * @param[out] meanAcqToNetworkTime Mean acquisition to network time during realTimeIntervalUs in microseconds (optional, can be NULL)
 * @param[out] acqToNetworkJitter Acquisition to network time jitter during realTimeIntervalUs in microseconds (optional, can be NULL)
 * @param[out] meanNetworkTime Mean network time during realTimeIntervalUs in microseconds (optional, can be NULL)
 * @param[out] networkJitter Network time jitter during realTimeIntervalUs in microseconds (optional, can be NULL)
 * @param[out] bytesSent Bytes sent during realTimeIntervalUs (optional, can be NULL)
 * @param[out] meanPacketSize Mean packet size during realTimeIntervalUs (optional, can be NULL)
 * @param[out] packetSizeStdDev Packet size standard deviation during realTimeIntervalUs (optional, can be NULL)
 * @param[out] packetsSent Packets sent during realTimeIntervalUs (optional, can be NULL)
 * @param[out] bytesDropped Bytes dropped during realTimeIntervalUs (optional, can be NULL)
 * @param[out] naluDropped NAL units dropped during realTimeIntervalUs (optional, can be NULL)
 *
 * @return ARSTREAM_OK if no error occured.
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the sender is invalid or if timeIntervalUs is 0.
 */
eARSTREAM_ERROR ARSTREAM_Sender2_GetMonitoring(ARSTREAM_Sender2_t *sender, uint64_t startTime, uint32_t timeIntervalUs, uint32_t *realTimeIntervalUs, uint32_t *meanAcqToNetworkTime,
                                               uint32_t *acqToNetworkJitter, uint32_t *meanNetworkTime, uint32_t *networkJitter, uint32_t *bytesSent, uint32_t *meanPacketSize,
                                               uint32_t *packetSizeStdDev, uint32_t *packetsSent, uint32_t *bytesDropped, uint32_t *naluDropped);


#endif /* _ARSTREAM_SENDER2_H_ */

