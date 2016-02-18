/**
 * @file arstream2_h264_filter.h
 * @brief Parrot Streaming Library - H.264 Filter
 * @date 08/04/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM2_H264_FILTER_H_
#define _ARSTREAM2_H264_FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

#include <inttypes.h>
#include <libARStream2/arstream2_error.h>
#include <libARStream2/arstream2_rtp_receiver.h>


/**
 * @brief ARSTREAM2 H264Filter instance handle.
 */
typedef void* ARSTREAM2_H264Filter_Handle;


/**
 * @brief AU synchronization type.
 */
typedef enum
{
    ARSTREAM2_H264_FILTER_AU_SYNC_TYPE_NONE = 0,    /**< The Access Unit is not a synchronization point */
    ARSTREAM2_H264_FILTER_AU_SYNC_TYPE_IDR,         /**< The Access Unit is an IDR picture */
    ARSTREAM2_H264_FILTER_AU_SYNC_TYPE_IFRAME,      /**< The Access Unit is an I-frame */
    ARSTREAM2_H264_FILTER_AU_SYNC_TYPE_PIR_START,   /**< The Access Unit is a Periodic Intra Refresh start */
    ARSTREAM2_H264_FILTER_AU_SYNC_TYPE_MAX,

} eARSTREAM2_H264_FILTER_AU_SYNC_TYPE;


/**
 * @brief ARSTREAM2 H264Filter configuration for initialization.
 */
typedef struct
{
    int waitForSync;                                                /**< if true, wait for SPS/PPS sync before outputting access anits */
    int outputIncompleteAu;                                         /**< if true, output incomplete access units */
    int filterOutSpsPps;                                            /**< if true, filter out SPS and PPS NAL units */
    int filterOutSei;                                               /**< if true, filter out SEI NAL units */
    int replaceStartCodesWithNaluSize;                              /**< if true, replace the NAL units start code with the NALU size */
    int generateSkippedPSlices;                                     /**< if true, generate skipped P slices to replace missing slices */
    int generateFirstGrayIFrame;                                    /**< if true, generate a first gray I frame to initialize the decoding (waitForSync must be enabled) */

} ARSTREAM2_H264Filter_Config_t;


/**
 * @brief SPS/PPS NAL units callback function
 *
 * The optional SPS/PPS callback function is called when SPS/PPS are found in the stream.
 *
 * @param spsBuffer Pointer to the SPS NAL unit buffer
 * @param spsSize Size in bytes of the SPS NAL unit
 * @param ppsBuffer Pointer to the PPS NAL unit buffer
 * @param ppsSize Size in bytes of the PPS NAL unit
 * @param userPtr SPS/PPS callback user pointer
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 *
 * @note This callback function is optional.
 *
 * @warning ARSTREAM2_H264Filter functions must not be called within the callback function.
 */
typedef eARSTREAM2_ERROR (*ARSTREAM2_H264Filter_SpsPpsCallback_t)(uint8_t *spsBuffer, int spsSize, uint8_t *ppsBuffer, int ppsSize, void *userPtr);


/**
 * @brief Get access unit buffer callback function
 *
 * The mandatory get AU buffer callback function is called to retreive a buffer to fill with an access unit.
 *
 * @param auBuffer Pointer to the AU buffer pointer
 * @param auBufferSize Pointer to the AU buffer size in bytes
 * @param auBufferUserPtr Pointer to the AU buffer user pointer
 * @param userPtr Get AU buffer callback user pointer
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return ARSTREAM2_ERROR_RESOURCE_UNAVAILABLE if no buffers are available.
 * @return an eARSTREAM2_ERROR error code if another error occurred.
 *
 * @warning This callback function is mandatory.
 * @warning ARSTREAM2_H264Filter functions must not be called within the callback function.
 */
typedef eARSTREAM2_ERROR (*ARSTREAM2_H264Filter_GetAuBufferCallback_t)(uint8_t **auBuffer, int *auBufferSize, void **auBufferUserPtr, void *userPtr);


/**
 * @brief Access unit ready callback function
 *
 * The mandatory AU ready callback function is called to output an access unit.
 *
 * @param auBuffer Pointer to the AU buffer
 * @param auSize AU size in bytes
 * @param auTimestamp AU timestamp in microseconds
 * @param auTimestampShifted AU timestamp shifted to the sender's clock in microseconds
 * @param auSyncType AU synchronization type
 * @param auUserData AU user data SEI buffer
 * @param auUserDataSize AU user data SEI size in bytes
 * @param auBufferUserPtr AU buffer user pointer
 * @param userPtr AU readey callback user pointer
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return ARSTREAM2_ERROR_RESYNC_REQUIRED if a decoding error occurred and re-sync is needed.
 * @return an eARSTREAM2_ERROR error code if another error occurred.
 *
 * @warning This callback function is mandatory.
 * @warning ARSTREAM2_H264Filter functions must not be called within the callback function.
 */
typedef eARSTREAM2_ERROR (*ARSTREAM2_H264Filter_AuReadyCallback_t)(uint8_t *auBuffer, int auSize, uint64_t auTimestamp, uint64_t auTimestampShifted, eARSTREAM2_H264_FILTER_AU_SYNC_TYPE auSyncType, void *auUserData, int auUserDataSize, void *auBufferUserPtr, void *userPtr);


/**
 * @brief Initialize an H264Filter instance.
 *
 * The library allocates the required resources. The user must call ARSTREAM2_H264Filter_Free() to free the resources.
 *
 * @param filterHandle Pointer to the handle used in future calls to the library.
 * @param config The instance configuration.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_Init(ARSTREAM2_H264Filter_Handle *filterHandle, ARSTREAM2_H264Filter_Config_t *config);


/**
 * @brief Free an H264Filter instance.
 *
 * The library frees the allocated resources. On success the filterHandle is set to NULL.
 *
 * @param filterHandle Pointer to the instance handle.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_Free(ARSTREAM2_H264Filter_Handle *filterHandle);


/**
 * @brief Run an H264Filter main thread.
 *
 * The instance must be correctly allocated using ARSTREAM2_H264Filter_Init().
 * @warning This function never returns until ARSTREAM2_H264Filter_Stop() is called. The tread can then be joined.
 *
 * @param filterHandle Instance handle casted as (void*).
 *
 * @return NULL in all cases.
 */
void* ARSTREAM2_H264Filter_RunFilterThread(void *filterHandle);


/**
 * @brief Start an H264Filter instance.
 *
 * The function starts processing the ARSTREAM2_RtpReceiver input.
 * The processing can be stopped using ARSTREAM2_H264Filter_Pause().
 *
 * @param filterHandle Instance handle.
 * @param spsPpsCallback Optional SPS/PPS callback function.
 * @param spsPpsCallbackUserPtr Optional SPS/PPS callback user pointer.
 * @param getAuBufferCallback Mandatory get access unit buffer callback function.
 * @param getAuBufferCallbackUserPtr Optional get access unit buffer callback user pointer.
 * @param auReadyCallback Mandatory access unit ready callback function.
 * @param auReadyCallbackUserPtr Optional access unit ready callback user pointer.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_Start(ARSTREAM2_H264Filter_Handle filterHandle, ARSTREAM2_H264Filter_SpsPpsCallback_t spsPpsCallback, void* spsPpsCallbackUserPtr,
                                            ARSTREAM2_H264Filter_GetAuBufferCallback_t getAuBufferCallback, void* getAuBufferCallbackUserPtr,
                                            ARSTREAM2_H264Filter_AuReadyCallback_t auReadyCallback, void* auReadyCallbackUserPtr);


/**
 * @brief Pause an H264Filter instance.
 *
 * The function stops processing the ARSTREAM2_RtpReceiver input.
 * The callback functions provided to ARSTREAM2_H264Filter_Start() will not be called any more.
 * The filter can be started again by a new call to ARSTREAM2_H264Filter_Start().
 *
 * @param filterHandle Instance handle.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_Pause(ARSTREAM2_H264Filter_Handle filterHandle);


/**
 * @brief Stop an H264Filter instance.
 *
 * The function ends the filter thread before it can be joined.
 * A stopped filter cannot be restarted.
 *
 * @param filterHandle Instance handle.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_Stop(ARSTREAM2_H264Filter_Handle filterHandle);


/**
 * @brief Get the SPS and PPS buffers.
 *
 * The buffers are filled by the function and must be provided by the user. The size of the buffers are given
 * by a first call to the function with both buffer pointers null.
 * When the buffer pointers are not null the size pointers must point to the values of the user-allocated buffer sizes.
 *
 * @param filterHandle Instance handle.
 * @param spsBuffer SPS buffer pointer.
 * @param spsSize pointer to the SPS size.
 * @param ppsBuffer PPS buffer pointer.
 * @param ppsSize pointer to the PPS size.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return ARSTREAM2_ERROR_WAITING_FOR_SYNC if SPS/PPS are not available (no sync).
 * @return an eARSTREAM2_ERROR error code if another error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Filter_GetSpsPps(ARSTREAM2_H264Filter_Handle filterHandle, uint8_t *spsBuffer, int *spsSize, uint8_t *ppsBuffer, int *ppsSize);


/**
 * @brief RTP receiver NAL unit callback function
 *
 * @see ARSTREAM2_RtpReceiver_NaluCallback_t.
 *
 */
uint8_t* ARSTREAM2_H264Filter_RtpReceiverNaluCallback(eARSTREAM2_RTP_RECEIVER_CAUSE cause, uint8_t *naluBuffer, int naluSize, uint64_t auTimestamp,
                                                      uint64_t auTimestampShifted, int isFirstNaluInAu, int isLastNaluInAu,
                                                      int missingPacketsBefore, int *newNaluBufferSize, void *custom);


#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* #ifndef _ARSTREAM2_H264_FILTER_H_ */
