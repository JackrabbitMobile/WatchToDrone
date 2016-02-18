/**
 * @file arstream2_rtp_receiver.h
 * @brief Parrot Streaming Library - RTP Receiver
 * @date 04/16/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM2_RTP_RECEIVER_H_
#define _ARSTREAM2_RTP_RECEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

#include <inttypes.h>
#include <libARStream2/arstream2_error.h>


/**
 * @brief Default client-side stream port
 */
#define ARSTREAM2_RTP_RECEIVER_DEFAULT_CLIENT_STREAM_PORT     (55004)


/**
 * @brief Default client-side control port
 */
#define ARSTREAM2_RTP_RECEIVER_DEFAULT_CLIENT_CONTROL_PORT    (55005)


/**
 * @brief Causes for NAL unit callback function
 */
typedef enum
{
    ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_COMPLETE = 0,       /**< NAL unit is complete (no error) */
    ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_BUFFER_TOO_SMALL,   /**< NAL unit buffer is too small */
    ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_COPY_COMPLETE,      /**< The copy of the previous NAL unit buffer is complete (used only after ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_BUFFER_TOO_SMALL) */
    ARSTREAM2_RTP_RECEIVER_CAUSE_CANCEL,                  /**< The receiver is closing, so buffer is no longer used */
    ARSTREAM2_RTP_RECEIVER_CAUSE_MAX,

} eARSTREAM2_RTP_RECEIVER_CAUSE;


/**
 * @brief Callback function for NAL units
 *
 * @param[in] cause Describes why the callback function was called
 * @param[in] naluBuffer Pointer to the NAL unit buffer
 * @param[in] naluSize NAL unit size in bytes
 * @param[in] auTimestamp Access unit timestamp
 * @param[in] auTimestampShifted Access unit timestamp in the local clock reference (0 if clock sync is not available)
 * @param[in] isFirstNaluInAu Boolean-like (0-1) flag indicating that the NAL unit is the first in an access unit
 * @param[in] isLastNaluInAu Boolean-like (0-1) flag indicating that the NAL unit is the last in an access unit
 * @param[in] missingPacketsBefore Number of missing network packets before this NAL unit (should be 0 most of the time)
 * @param[inout] newNaluBufferSize Pointer to the new NAL unit buffer size in bytes
 * @param[in] userPtr Global NAL unit callback user pointer
 *
 * @return address of a new buffer which will hold the next NAL unit
 *
 * @note If cause is ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_COMPLETE, naluBuffer contains a valid NAL unit.
 * @note If cause is ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_BUFFER_TOO_SMALL, any data already present in the previous NAL unit buffer will be copied by the receiver into the new buffer.
 * The previous NAL unit buffer will still be in use until the callback function is called again with the ARSTREAM2_RTP_RECEIVER_CAUSE_COPY_COMPLETE cause.
 * If the new buffer is still too small, the current NAL unit will be skipped.
 * @note If cause is ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_COPY_COMPLETE, the return value and newNaluBufferSize are unused.
 * @note If cause is ARSTREAM2_RTP_RECEIVER_CAUSE_CANCEL, the return value and newNaluBufferSize are unused.
 *
 * @warning If the cause is ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_BUFFER_TOO_SMALL, returning a buffer smaller than the initial value of newNaluBufferSize or a NULL pointer will skip the current NAL unit.
 */
typedef uint8_t* (*ARSTREAM2_RtpReceiver_NaluCallback_t)(eARSTREAM2_RTP_RECEIVER_CAUSE cause, uint8_t *naluBuffer, int naluSize, uint64_t auTimestamp,
                                                         uint64_t auTimestampShifted, int isFirstNaluInAu, int isLastNaluInAu,
                                                         int missingPacketsBefore, int *newNaluBufferSize, void *userPtr);


/**
 * @brief RtpReceiver configuration parameters
 */
typedef struct ARSTREAM2_RtpReceiver_Config_t
{
    const char *serverAddr;                         /**< Server address */
    const char *mcastAddr;                          /**< Multicast receive address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast input interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    ARSTREAM2_RtpReceiver_NaluCallback_t naluCallback;   /**< NAL unit callback function */
    void *naluCallbackUserPtr;                      /**< NAL unit callback function user pointer (optional, can be NULL) */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (should be provided by the server, if 0 the maximum UDP packet size is used) */
    int maxBitrate;                                 /**< Maximum streaming bitrate in bit/s (should be provided by the server, can be 0) */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (should be provided by the server, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds (should be provided by the server, can be 0) */
    int insertStartCodes;                           /**< Boolean-like (0-1) flag: if active insert a start code prefix before NAL units */

} ARSTREAM2_RtpReceiver_Config_t;


/**
 * @brief RtpReceiver RtpResender configuration parameters
 */
typedef struct ARSTREAM2_RtpReceiver_RtpResender_Config_t
{
    const char *clientAddr;                         /**< Client address */
    const char *mcastAddr;                          /**< Multicast send address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast output interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (example: the interface MTU) */
    int targetPacketSize;                           /**< Target network packet size in bytes */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (optional, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds */

} ARSTREAM2_RtpReceiver_RtpResender_Config_t;


/**
 * @brief An RtpReceiver instance to allow receiving H.264 video over a network
 */
typedef struct ARSTREAM2_RtpReceiver_t ARSTREAM2_RtpReceiver_t;


/**
 * @brief An RtpReceiver RtpResender instance to allow re-streaming H.264 video over a network
 */
typedef struct ARSTREAM2_RtpReceiver_RtpResender_t ARSTREAM2_RtpReceiver_RtpResender_t;


/**
 * @brief Creates a new RtpReceiver
 * @warning This function allocates memory. The receiver must be deleted by a call to ARSTREAM2_RtpReceiver_Delete()
 *
 * @param[in] config Pointer to a configuration parameters structure
 * @param[out] error Optionnal pointer to an eARSTREAM2_ERROR to hold any error information
 *
 * @return A pointer to the new ARSTREAM2_RtpReceiver_t, or NULL if an error occured
 *
 * @see ARSTREAM2_RtpReceiver_Stop()
 * @see ARSTREAM2_RtpReceiver_Delete()
 */
ARSTREAM2_RtpReceiver_t* ARSTREAM2_RtpReceiver_New(ARSTREAM2_RtpReceiver_Config_t *config, eARSTREAM2_ERROR *error);


/**
 * @brief Invalidate the current NAL unit buffer
 *
 * This function blocks until the current NAL unit buffer is no longer used by the receiver.
 * The NAL unit callback function will then be called with cause ARSTREAM2_RTP_RECEIVER_CAUSE_NALU_BUFFER_TOO_SMALL to get a new buffer.
 *
 * @param[in] receiver The receiver instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM2_RtpReceiver_InvalidateNaluBuffer(ARSTREAM2_RtpReceiver_t *receiver);


/**
 * @brief Stops a running RtpReceiver
 * @warning Once stopped, a receiver cannot be restarted
 *
 * @param[in] receiver The receiver instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM2_RtpReceiver_Stop(ARSTREAM2_RtpReceiver_t *receiver);


/**
 * @brief Deletes an RtpReceiver
 * @warning This function should NOT be called on a running receiver
 *
 * @param receiver Pointer to the ARSTREAM2_RtpReceiver_t* to delete
 *
 * @return ARSTREAM2_OK if the receiver was deleted
 * @return ARSTREAM2_ERROR_BUSY if the receiver is still busy and can not be stopped now (probably because ARSTREAM2_RtpReceiver_Stop() has not been called yet)
 * @return ARSTREAM2_ERROR_BAD_PARAMETERS if receiver does not point to a valid ARSTREAM2_RtpReceiver_t
 *
 * @note The function uses a double pointer, so it can set *receiver to NULL after freeing it
 */
eARSTREAM2_ERROR ARSTREAM2_RtpReceiver_Delete(ARSTREAM2_RtpReceiver_t **receiver);


/**
 * @brief Runs the stream loop of the RtpReceiver
 * @warning This function never returns until ARSTREAM2_RtpReceiver_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the receiver by calling ARSTREAM2_RtpReceiver_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM2_RtpReceiver_t_Param A valid (ARSTREAM2_RtpReceiver_t *) casted as a (void *)
 */
void* ARSTREAM2_RtpReceiver_RunStreamThread(void *ARSTREAM2_RtpReceiver_t_Param);


/**
 * @brief Runs the control loop of the RtpReceiver
 * @warning This function never returns until ARSTREAM2_RtpReceiver_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the receiver by calling ARSTREAM2_RtpReceiver_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM2_RtpReceiver_t_Param A valid (ARSTREAM2_RtpReceiver_t *) casted as a (void *)
 */
void* ARSTREAM2_RtpReceiver_RunControlThread(void *ARSTREAM2_RtpReceiver_t_Param);


/**
 * @brief Get the user pointer associated with the receiver NAL unit callback function
 *
 * @param[in] receiver The receiver instance
 * @return The user pointer associated with the NALU callback, or NULL if receiver does not point to a valid receiver
 */
void* ARSTREAM2_RtpReceiver_GetNaluCallbackUserPtr(ARSTREAM2_RtpReceiver_t *receiver);


/**
 * @brief Get the stream monitoring
 * The monitoring data is computed form the time startTime and back timeIntervalUs microseconds at most.
 * If startTime is 0 the start time is the current time.
 * If monitoring data is not available up to timeIntervalUs, the monitoring is computed on less time and the real interval is output to realTimeIntervalUs.
 * Pointers to monitoring parameters that are not required can be left NULL.
 *
 * @param[in] receiver The receiver instance
 * @param[in] startTime Monitoring start time in microseconds (0 means current time)
 * @param[in] timeIntervalUs Monitoring time interval (back from startTime) in microseconds
 * @param[out] realTimeIntervalUs Real monitoring time interval in microseconds (optional, can be NULL)
 * @param[out] receptionTimeJitter Network reception time jitter during realTimeIntervalUs in microseconds (optional, can be NULL)
 * @param[out] bytesReceived Bytes received during realTimeIntervalUs (optional, can be NULL)
 * @param[out] meanPacketSize Mean packet size during realTimeIntervalUs (optional, can be NULL)
 * @param[out] packetSizeStdDev Packet size standard deviation during realTimeIntervalUs (optional, can be NULL)
 * @param[out] packetsReceived Packets received during realTimeIntervalUs (optional, can be NULL)
 * @param[out] packetsMissed Packets missed during realTimeIntervalUs (optional, can be NULL)
 *
 * @return ARSTREAM2_OK if no error occured.
 * @return ARSTREAM2_ERROR_BAD_PARAMETERS if the receiver is invalid or if timeIntervalUs is 0.
 */
eARSTREAM2_ERROR ARSTREAM2_RtpReceiver_GetMonitoring(ARSTREAM2_RtpReceiver_t *receiver, uint64_t startTime, uint32_t timeIntervalUs, uint32_t *realTimeIntervalUs, uint32_t *receptionTimeJitter,
                                                     uint32_t *bytesReceived, uint32_t *meanPacketSize, uint32_t *packetSizeStdDev, uint32_t *packetsReceived, uint32_t *packetsMissed);


/**
 * @brief Creates a new RtpReceiver RtpResender
 * @warning This function allocates memory. The resender must be deleted by a call to ARSTREAM2_RtpReceiver_Delete() or ARSTREAM2_RtpReceiver_RtpResender_Delete()
 *
 * @param[in] receiver The receiver instance
 * @param[in] config Pointer to a resender configuration parameters structure
 * @param[out] error Optionnal pointer to an eARSTREAM2_ERROR to hold any error information
 *
 * @return A pointer to the new ARSTREAM2_RtpReceiver_RtpResender_t, or NULL if an error occured
 *
 * @see ARSTREAM2_RtpReceiver_RtpResender_Stop()
 * @see ARSTREAM2_RtpReceiver_RtpResender_Delete()
 */
ARSTREAM2_RtpReceiver_RtpResender_t* ARSTREAM2_RtpReceiver_RtpResender_New(ARSTREAM2_RtpReceiver_t *receiver, ARSTREAM2_RtpReceiver_RtpResender_Config_t *config, eARSTREAM2_ERROR *error);


/**
 * @brief Stops a running RtpReceiver RtpResender
 * @warning Once stopped, a resender cannot be restarted
 *
 * @param[in] resender The resender instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM2_RtpReceiver_RtpResender_Stop(ARSTREAM2_RtpReceiver_RtpResender_t *resender);


/**
 * @brief Deletes an RtpReceiver RtpResender
 * @warning This function should NOT be called on a running resender
 *
 * @param resender Pointer to the ARSTREAM2_RtpReceiver_RtpResender_t* to delete
 *
 * @return ARSTREAM2_OK if the resender was deleted
 * @return ARSTREAM2_ERROR_BUSY if the resender is still busy and can not be stopped now (probably because ARSTREAM2_RtpReceiver_RtpResender_Stop() has not been called yet)
 * @return ARSTREAM2_ERROR_BAD_PARAMETERS if resender does not point to a valid ARSTREAM2_RtpReceiver_RtpResender_t
 *
 * @note The function uses a double pointer, so it can set *resender to NULL after freeing it
 */
eARSTREAM2_ERROR ARSTREAM2_RtpReceiver_RtpResender_Delete(ARSTREAM2_RtpReceiver_RtpResender_t **resender);


/**
 * @brief Runs the stream loop of the RtpReceiver RtpResender
 * @warning This function never returns until ARSTREAM2_RtpReceiver_RtpResender_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the resender by calling ARSTREAM2_RtpReceiver_RtpResender_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM2_RtpReceiver_RtpResender_t_Param A valid (ARSTREAM2_RtpReceiver_RtpResender_t *) casted as a (void *)
 */
void* ARSTREAM2_RtpReceiver_RtpResender_RunStreamThread(void *ARSTREAM2_RtpReceiver_RtpResender_t_Param);


/**
 * @brief Runs the control loop of the RtpReceiver RtpResender
 * @warning This function never returns until ARSTREAM2_RtpReceiver_RtpResender_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the resender by calling ARSTREAM2_RtpReceiver_RtpResender_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM2_RtpReceiver_RtpResender_t_Param A valid (ARSTREAM2_RtpReceiver_RtpResender_t *) casted as a (void *)
 */
void* ARSTREAM2_RtpReceiver_RtpResender_RunControlThread(void *ARSTREAM2_RtpReceiver_RtpResender_t_Param);


#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* _ARSTREAM2_RTP_RECEIVER_H_ */
