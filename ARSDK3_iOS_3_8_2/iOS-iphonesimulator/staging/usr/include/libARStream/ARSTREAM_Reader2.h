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
 * @file ARSTREAM_Reader2.h
 * @brief Stream reader on a network (v2)
 * @date 04/16/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM_READER2_H_
#define _ARSTREAM_READER2_H_

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
 * @brief Default client-side stream port
 */
#define ARSTREAM_READER2_DEFAULT_CLIENT_STREAM_PORT     (55004)

/**
 * @brief Default client-side control port
 */
#define ARSTREAM_READER2_DEFAULT_CLIENT_CONTROL_PORT    (55005)


/*
 * Types
 */

/**
 * @brief Causes for NAL unit callback function
 */
typedef enum
{
    ARSTREAM_READER2_CAUSE_NALU_COMPLETE = 0,       /**< NAL unit is complete (no error) */
    ARSTREAM_READER2_CAUSE_NALU_BUFFER_TOO_SMALL,   /**< NAL unit buffer is too small */
    ARSTREAM_READER2_CAUSE_NALU_COPY_COMPLETE,      /**< The copy of the previous NAL unit buffer is complete (used only after ARSTREAM_READER2_CAUSE_NALU_BUFFER_TOO_SMALL) */
    ARSTREAM_READER2_CAUSE_CANCEL,                  /**< The reader is closing, so buffer is no longer used */
    ARSTREAM_READER2_CAUSE_MAX,

} eARSTREAM_READER2_CAUSE;

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
 * @note If cause is ARSTREAM_READER2_CAUSE_NALU_COMPLETE, naluBuffer contains a valid NAL unit.
 * @note If cause is ARSTREAM_READER2_CAUSE_NALU_BUFFER_TOO_SMALL, any data already present in the previous NAL unit buffer will be copied by the reader into the new buffer.
 * The previous NAL unit buffer will still be in use until the callback function is called again with the ARSTREAM_READER2_CAUSE_COPY_COMPLETE cause.
 * If the new buffer is still too small, the current NAL unit will be skipped.
 * @note If cause is ARSTREAM_READER2_CAUSE_NALU_COPY_COMPLETE, the return value and newNaluBufferSize are unused.
 * @note If cause is ARSTREAM_READER2_CAUSE_CANCEL, the return value and newNaluBufferSize are unused.
 *
 * @warning If the cause is ARSTREAM_READER2_CAUSE_NALU_BUFFER_TOO_SMALL, returning a buffer smaller than the initial value of newNaluBufferSize or a NULL pointer will skip the current NAL unit.
 */
typedef uint8_t* (*ARSTREAM_Reader2_NaluCallback_t) (eARSTREAM_READER2_CAUSE cause, uint8_t *naluBuffer, int naluSize, uint64_t auTimestamp,
                                                     uint64_t auTimestampShifted, int isFirstNaluInAu, int isLastNaluInAu,
                                                     int missingPacketsBefore, int *newNaluBufferSize, void *userPtr);

/**
 * @brief Reader2 configuration parameters
 */
typedef struct ARSTREAM_Reader2_Config_t
{
    const char *serverAddr;                         /**< Server address */
    const char *mcastAddr;                          /**< Multicast receive address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast input interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    ARSTREAM_Reader2_NaluCallback_t naluCallback;   /**< NAL unit callback function */
    void *naluCallbackUserPtr;                      /**< NAL unit callback function user pointer (optional, can be NULL) */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (should be provided by the server, if 0 the maximum UDP packet size is used) */
    int maxBitrate;                                 /**< Maximum streaming bitrate in bit/s (should be provided by the server, can be 0) */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (should be provided by the server, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds (should be provided by the server, can be 0) */
    int insertStartCodes;                           /**< Boolean-like (0-1) flag: if active insert a start code prefix before NAL units */

} ARSTREAM_Reader2_Config_t;

/**
 * @brief Reader2 Resender configuration parameters
 */
typedef struct ARSTREAM_Reader2_Resender_Config_t
{
    const char *clientAddr;                         /**< Client address */
    const char *mcastAddr;                          /**< Multicast send address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast output interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM_SENDER2_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (example: the interface MTU) */
    int targetPacketSize;                           /**< Target network packet size in bytes */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (optional, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds */

} ARSTREAM_Reader2_Resender_Config_t;

/**
 * @brief A Reader2 instance to allow receiving H.264 video over a network
 */
typedef struct ARSTREAM_Reader2_t ARSTREAM_Reader2_t;

/**
 * @brief A Reader2 Resender instance to allow re-streaming H.264 video over a network
 */
typedef struct ARSTREAM_Reader2_Resender_t ARSTREAM_Reader2_Resender_t;


/*
 * Functions declarations
 */

/**
 * @brief Creates a new Reader2
 * @warning This function allocates memory. The Reader2 must be deleted by a call to ARSTREAM_Reader2_Delete()
 *
 * @param[in] config Pointer to a configuration parameters structure
 * @param[out] error Optionnal pointer to an eARSTREAM_ERROR to hold any error information
 *
 * @return A pointer to the new ARSTREAM_Reader2_t, or NULL if an error occured
 *
 * @see ARSTREAM_Reader2_StopReader()
 * @see ARSTREAM_Reader2_Delete()
 */
ARSTREAM_Reader2_t* ARSTREAM_Reader2_New (ARSTREAM_Reader2_Config_t *config, eARSTREAM_ERROR *error);

/**
 * @brief Invalidate the current NAL unit buffer
 *
 * This function blocks until the current NAL unit buffer is no longer used by the Reader2.
 * The NAL unit callback function will then be called with cause ARSTREAM_READER2_CAUSE_NALU_BUFFER_TOO_SMALL to get a new buffer.
 *
 * @param[in] reader The Reader2 instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Reader2_InvalidateNaluBuffer (ARSTREAM_Reader2_t *reader);

/**
 * @brief Stops a running Reader2
 * @warning Once stopped, a Reader2 cannot be restarted
 *
 * @param[in] reader The Reader2 instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Reader2_StopReader (ARSTREAM_Reader2_t *reader);

/**
 * @brief Deletes a Reader2
 * @warning This function should NOT be called on a running Reader2
 *
 * @param reader Pointer to the ARSTREAM_Reader2_t* to delete
 *
 * @return ARSTREAM_OK if the Reader2 was deleted
 * @return ARSTREAM_ERROR_BUSY if the Reader2 is still busy and can not be stopped now (probably because ARSTREAM_Reader2_StopReader() has not been called yet)
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if reader does not point to a valid ARSTREAM_Reader2_t
 *
 * @note The function uses a double pointer, so it can set *reader to NULL after freeing it
 */
eARSTREAM_ERROR ARSTREAM_Reader2_Delete (ARSTREAM_Reader2_t **reader);

/**
 * @brief Runs the stream loop of the Reader2
 * @warning This function never returns until ARSTREAM_Reader2_StopReader() is called. Thus, it should be called on its own thread.
 * @post Stop the Reader2 by calling ARSTREAM_Reader2_StopReader() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Reader2_t_Param A valid (ARSTREAM_Reader2_t *) casted as a (void *)
 */
void* ARSTREAM_Reader2_RunStreamThread (void *ARSTREAM_Reader2_t_Param);

/**
 * @brief Runs the control loop of the Reader2
 * @warning This function never returns until ARSTREAM_Reader2_StopReader() is called. Thus, it should be called on its own thread.
 * @post Stop the Reader2 by calling ARSTREAM_Reader2_StopReader() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Reader2_t_Param A valid (ARSTREAM_Reader2_t *) casted as a (void *)
 */
void* ARSTREAM_Reader2_RunControlThread (void *ARSTREAM_Reader2_t_Param);

/**
 * @brief Get the user pointer associated with the reader NAL unit callback function
 *
 * @param[in] reader The Reader2 instance
 * @return The user pointer associated with the NALU callback, or NULL if sender does not point to a valid reader
 */
void* ARSTREAM_Reader2_GetNaluCallbackUserPtr (ARSTREAM_Reader2_t *reader);

/**
 * @brief Get the stream monitoring
 * The monitoring data is computed form the time startTime and back timeIntervalUs microseconds at most.
 * If startTime is 0 the start time is the current time.
 * If monitoring data is not available up to timeIntervalUs, the monitoring is computed on less time and the real interval is output to realTimeIntervalUs.
 * Pointers to monitoring parameters that are not required can be left NULL.
 *
 * @param[in] reader The Reader2 instance
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
 * @return ARSTREAM_OK if no error occured.
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if the reader is invalid or if timeIntervalUs is 0.
 */
eARSTREAM_ERROR ARSTREAM_Reader2_GetMonitoring (ARSTREAM_Reader2_t *reader, uint64_t startTime, uint32_t timeIntervalUs, uint32_t *realTimeIntervalUs, uint32_t *receptionTimeJitter,
                                                uint32_t *bytesReceived, uint32_t *meanPacketSize, uint32_t *packetSizeStdDev, uint32_t *packetsReceived, uint32_t *packetsMissed);

/**
 * @brief Creates a new Reader2 Resender
 * @warning This function allocates memory. The Reader2 Resender must be deleted by a call to ARSTREAM_Reader2_Delete() or ARSTREAM_Reader2_Resender_Delete()
 *
 * @param[in] reader The Reader2 instance
 * @param[in] config Pointer to a resender configuration parameters structure
 * @param[out] error Optionnal pointer to an eARSTREAM_ERROR to hold any error information
 *
 * @return A pointer to the new ARSTREAM_Reader2_Resender_t, or NULL if an error occured
 *
 * @see ARSTREAM_Reader2_Resender_Stop()
 * @see ARSTREAM_Reader2_Resender_Delete()
 */
ARSTREAM_Reader2_Resender_t* ARSTREAM_Reader2_Resender_New (ARSTREAM_Reader2_t *reader, ARSTREAM_Reader2_Resender_Config_t *config, eARSTREAM_ERROR *error);

/**
 * @brief Stops a running Reader2 Resender
 * @warning Once stopped, a Reader2 Resender cannot be restarted
 *
 * @param[in] resender The Reader2 Resender instance
 *
 * @note Calling this function multiple times has no effect
 */
void ARSTREAM_Reader2_Resender_Stop (ARSTREAM_Reader2_Resender_t *resender);

/**
 * @brief Deletes a Reader2 Resender
 * @warning This function should NOT be called on a running Reader2 Resender
 *
 * @param resender Pointer to the ARSTREAM_Reader2_Resender_t* to delete
 *
 * @return ARSTREAM_OK if the Reader2 Resender was deleted
 * @return ARSTREAM_ERROR_BUSY if the Reader2 Resender is still busy and can not be stopped now (probably because ARSTREAM_Reader2_Resender_Stop() has not been called yet)
 * @return ARSTREAM_ERROR_BAD_PARAMETERS if resender does not point to a valid ARSTREAM_Reader2_Resender_t
 *
 * @note The function uses a double pointer, so it can set *resender to NULL after freeing it
 */
eARSTREAM_ERROR ARSTREAM_Reader2_Resender_Delete (ARSTREAM_Reader2_Resender_t **resender);

/**
 * @brief Runs the stream loop of the Reader2 Resender
 * @warning This function never returns until ARSTREAM_Reader2_Resender_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the Reader2 Resender by calling ARSTREAM_Reader2_Resender_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Reader2_Resender_t_Param A valid (ARSTREAM_Reader2_Resender_t *) casted as a (void *)
 */
void* ARSTREAM_Reader2_Resender_RunStreamThread (void *ARSTREAM_Reader2_Resender_t_Param);

/**
 * @brief Runs the control loop of the Reader2 Resender
 * @warning This function never returns until ARSTREAM_Reader2_Resender_Stop() is called. Thus, it should be called on its own thread.
 * @post Stop the Reader2 Resender by calling ARSTREAM_Reader2_Resender_Stop() before joining the thread calling this function.
 *
 * @param[in] ARSTREAM_Reader2_Resender_t_Param A valid (ARSTREAM_Reader2_Resender_t *) casted as a (void *)
 */
void* ARSTREAM_Reader2_Resender_RunControlThread (void *ARSTREAM_Reader2_Resender_t_Param);


#endif /* _ARSTREAM_READER2_H_ */

