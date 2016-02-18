/**
 * @file arstream2_stream_receiver.h
 * @brief Parrot Streaming Library - Stream Receiver
 * @date 08/04/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM2_STREAM_RECEIVER_H_
#define _ARSTREAM2_STREAM_RECEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

#include <inttypes.h>
#include <libARStream2/arstream2_error.h>
#include <libARStream2/arstream2_rtp_receiver.h>
#include <libARStream2/arstream2_rtp_sender.h>
#include <libARStream2/arstream2_h264_filter.h>


/**
 * @brief Default client-side stream port
 */
#define ARSTREAM2_STREAM_RECEIVER_DEFAULT_CLIENT_STREAM_PORT      ARSTREAM2_RTP_RECEIVER_DEFAULT_CLIENT_STREAM_PORT

/**
 * @brief Default client-side control port
 */
#define ARSTREAM2_STREAM_RECEIVER_DEFAULT_CLIENT_CONTROL_PORT     ARSTREAM2_RTP_RECEIVER_DEFAULT_CLIENT_CONTROL_PORT

/**
 * @brief Default server-side resender stream port
 */
#define ARSTREAM2_STREAM_RECEIVER_RESENDER_DEFAULT_SERVER_STREAM_PORT      ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_STREAM_PORT

/**
 * @brief Default server-side resender control port
 */
#define ARSTREAM2_STREAM_RECEIVER_RESENDER_DEFAULT_SERVER_CONTROL_PORT     ARSTREAM2_RTP_SENDER_DEFAULT_SERVER_CONTROL_PORT


/**
 * @brief ARSTREAM2 StreamReceiver instance handle.
 */
typedef void* ARSTREAM2_StreamReceiver_Handle;


/**
 * @brief ARSTREAM2 StreamReceiver resender handle.
 */
typedef void* ARSTREAM2_StreamReceiver_ResenderHandle;


/**
 * @brief ARSTREAM2 StreamReceiver configuration for initialization.
 */
typedef struct
{
    const char *serverAddr;                                         /**< Server address */
    const char *mcastAddr;                                          /**< Multicast receive address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                                     /**< Multicast input interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                                           /**< Server stream port, @see ARSTREAM2_STREAM_RECEIVER_DEFAULT_CLIENT_STREAM_PORT */
    int serverControlPort;                                          /**< Server control port, @see ARSTREAM2_STREAM_RECEIVER_DEFAULT_CLIENT_CONTROL_PORT */
    int clientStreamPort;                                           /**< Client stream port */
    int clientControlPort;                                          /**< Client control port */
    int maxPacketSize;                                              /**< Maximum network packet size in bytes (should be provided by the server, if 0 the maximum UDP packet size is used) */
    int maxBitrate;                                                 /**< Maximum streaming bitrate in bit/s (should be provided by the server, can be 0) */
    int maxLatencyMs;                                               /**< Maximum acceptable total latency in milliseconds (should be provided by the server, can be 0) */
    int maxNetworkLatencyMs;                                        /**< Maximum acceptable network latency in milliseconds (should be provided by the server, can be 0) */
    int waitForSync;                                                /**< if true, wait for SPS/PPS sync before outputting access anits */
    int outputIncompleteAu;                                         /**< if true, output incomplete access units */
    int filterOutSpsPps;                                            /**< if true, filter out SPS and PPS NAL units */
    int filterOutSei;                                               /**< if true, filter out SEI NAL units */
    int replaceStartCodesWithNaluSize;                              /**< if true, replace the NAL units start code with the NALU size */
    int generateSkippedPSlices;                                     /**< if true, generate skipped P slices to replace missing slices */
    int generateFirstGrayIFrame;                                    /**< if true, generate a first gray I frame to initialize the decoding (waitForSync must be enabled) */

} ARSTREAM2_StreamReceiver_Config_t;


/**
 * @brief ARSTREAM2 StreamReceiver resender configuration parameters.
 */
typedef struct ARSTREAM2_StreamReceiver_ResenderConfig_t
{
    const char *clientAddr;                         /**< Client address */
    const char *mcastAddr;                          /**< Multicast send address (optional, NULL for no multicast) */
    const char *mcastIfaceAddr;                     /**< Multicast output interface address (required if mcastAddr is not NULL) */
    int serverStreamPort;                           /**< Server stream port, @see ARSTREAM2_STREAM_RECEIVER_RESENDER_DEFAULT_SERVER_STREAM_PORT */
    int serverControlPort;                          /**< Server control port, @see ARSTREAM2_STREAM_RECEIVER_RESENDER_DEFAULT_SERVER_CONTROL_PORT */
    int clientStreamPort;                           /**< Client stream port */
    int clientControlPort;                          /**< Client control port */
    int maxPacketSize;                              /**< Maximum network packet size in bytes (example: the interface MTU) */
    int targetPacketSize;                           /**< Target network packet size in bytes */
    int maxLatencyMs;                               /**< Maximum acceptable total latency in milliseconds (optional, can be 0) */
    int maxNetworkLatencyMs;                        /**< Maximum acceptable network latency in milliseconds */

} ARSTREAM2_StreamReceiver_ResenderConfig_t;


/**
 * @brief Initialize a StreamReceiver instance.
 *
 * The library allocates the required resources. The user must call ARSTREAM2_StreamReceiver_Free() to free the resources.
 *
 * @param streamReceiverHandle Pointer to the handle used in future calls to the library.
 * @param config The instance configuration.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_Init(ARSTREAM2_StreamReceiver_Handle *streamReceiverHandle, ARSTREAM2_StreamReceiver_Config_t *config);


/**
 * @brief Free a StreamReceiver instance.
 *
 * The library frees the allocated resources. On success the streamReceiverHandle is set to NULL.
 *
 * @param streamReceiverHandle Pointer to the instance handle.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_Free(ARSTREAM2_StreamReceiver_Handle *streamReceiverHandle);


/**
 * @brief Run a StreamReceiver filter thread.
 *
 * The instance must be correctly allocated using ARSTREAM2_StreamReceiver_Init().
 * @warning This function never returns until ARSTREAM2_StreamReceiver_Stop() is called. The tread can then be joined.
 *
 * @param streamReceiverHandle Instance handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_StreamReceiver_RunFilterThread(void *streamReceiverHandle);


/**
 * @brief Run a StreamReceiver stream thread.
 *
 * The instance must be correctly allocated using ARSTREAM2_StreamReceiver_Init().
 * @warning This function never returns until ARSTREAM2_StreamReceiver_Stop() is called. The tread can then be joined.
 *
 * @param streamReceiverHandle Instance handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_StreamReceiver_RunStreamThread(void *streamReceiverHandle);


/**
 * @brief Run a StreamReceiver control thread.
 *
 * The instance must be correctly allocated using ARSTREAM2_StreamReceiver_Init().
 * @warning This function never returns until ARSTREAM2_StreamReceiver_Stop() is called. The tread can then be joined.
 *
 * @param streamReceiverHandle Instance handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_StreamReceiver_RunControlThread(void *streamReceiverHandle);


/**
 * @brief Start the filter.
 *
 * The function starts processing the RTP input.
 * The processing can be stopped using ARSTREAM2_StreamReceiver_PauseFilter().
 *
 * @param streamReceiverHandle Instance handle.
 * @param spsPpsCallback SPS/PPS callback function.
 * @param spsPpsCallbackUserPtr SPS/PPS callback user pointer.
 * @param getAuBufferCallback Get access unit buffer callback function.
 * @param getAuBufferCallbackUserPtr Get access unit buffer callback user pointer.
 * @param auReadyCallback Access unit ready callback function.
 * @param auReadyCallbackUserPtr Access unit ready callback user pointer.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_StartFilter(ARSTREAM2_StreamReceiver_Handle streamReceiverHandle, ARSTREAM2_H264Filter_SpsPpsCallback_t spsPpsCallback, void* spsPpsCallbackUserPtr,
                                                      ARSTREAM2_H264Filter_GetAuBufferCallback_t getAuBufferCallback, void* getAuBufferCallbackUserPtr,
                                                      ARSTREAM2_H264Filter_AuReadyCallback_t auReadyCallback, void* auReadyCallbackUserPtr);


/**
 * @brief Pause the filter.
 *
 * The function stops processing the RTP input.
 * The callback functions provided to ARSTREAM2_StreamReceiver_StartFilter() will not be called any more.
 * The filter can be started again by a new call to ARSTREAM2_StreamReceiver_StartFilter().
 *
 * @param streamReceiverHandle Instance handle.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_PauseFilter(ARSTREAM2_StreamReceiver_Handle streamReceiverHandle);


/**
 * @brief Stop a StreamReceiver instance.
 *
 * The function ends the threads before they can be joined.
 *
 * @param streamReceiverHandle Instance handle.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_Stop(ARSTREAM2_StreamReceiver_Handle streamReceiverHandle);


/**
 * @brief Get the SPS and PPS buffers.
 *
 * The buffers are filled by the function and must be provided by the user. The size of the buffers are given
 * by a first call to the function with both buffer pointers null.
 * When the buffer pointers are not null the size pointers must point to the values of the user-allocated buffer sizes.
 *
 * @param streamReceiverHandle Instance handle.
 * @param spsBuffer SPS buffer pointer.
 * @param spsSize pointer to the SPS size.
 * @param ppsBuffer PPS buffer pointer.
 * @param ppsSize pointer to the PPS size.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 * @return -2 if SPS/PPS are not available (no sync).
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_GetSpsPps(ARSTREAM2_StreamReceiver_Handle streamReceiverHandle, uint8_t *spsBuffer, int *spsSize, uint8_t *ppsBuffer, int *ppsSize);


/**
 * @brief Initialize a new resender.
 *
 * The library allocates the required resources. The user must call ARSTREAM2_StreamReceiver_Free() or ARSTREAM2_StreamReceiver_FreeResender() to free the resources.
 *
 * @param streamReceiverHandle StreamReceiver instance handle.
 * @param resenderHandle Pointer to the resender handle used in future calls to the library.
 * @param config The resender configuration.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 *
 * @see ARSTREAM2_StreamReceiver_StopResender()
 * @see ARSTREAM2_StreamReceiver_FreeResender()
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_InitResender(ARSTREAM2_StreamReceiver_Handle streamReceiverHandle, ARSTREAM2_StreamReceiver_ResenderHandle *resenderHandle, ARSTREAM2_StreamReceiver_ResenderConfig_t *config);


/**
 * @brief Free a resender.
 *
 * The library frees the allocated resources. On success the resenderHandle is set to NULL.
 *
 * @param resenderHandle Pointer to the resender handle.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_FreeResender(ARSTREAM2_StreamReceiver_ResenderHandle *resenderHandle);


/**
 * @brief Run a resender stream thread.
 *
 * The resender must be correctly allocated using ARSTREAM2_StreamReceiver_InitResender().
 * @warning This function never returns until ARSTREAM2_StreamReceiver_StopResender() is called. The tread can then be joined.
 *
 * @param resenderHandle Resender handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_StreamReceiver_RunResenderStreamThread(void *resenderHandle);


/**
 * @brief Run a resender control thread.
 *
 * The resender must be correctly allocated using ARSTREAM2_StreamReceiver_InitResender().
 * @warning This function never returns until ARSTREAM2_StreamReceiver_StopResender() is called. The tread can then be joined.
 *
 * @param resenderHandle Resender handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_StreamReceiver_RunResenderControlThread(void *resenderHandle);


/**
 * @brief Stop a resender.
 *
 * The function ends the resender threads before they can be joined.
 *
 * @param resenderHandle Resender handle.
 *
 * @return 0 if no error occurred.
 * @return -1 if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_StreamReceiver_StopResender(ARSTREAM2_StreamReceiver_ResenderHandle resenderHandle);


#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* #ifndef _ARSTREAM2_STREAM_RECEIVER_H_ */
