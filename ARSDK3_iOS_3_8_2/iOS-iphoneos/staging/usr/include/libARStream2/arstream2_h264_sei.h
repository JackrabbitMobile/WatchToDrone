/**
 * @file arstream2_h264_sei.h
 * @brief Parrot Streaming Library - H.264 User Data SEI 
 * @date 08/04/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM2_H264_SEI_H_
#define _ARSTREAM2_H264_SEI_H_

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

#include <inttypes.h>
#include <libARStream2/arstream2_error.h>


#define ARSTREAM2_H264_SEI_PARROT_STREAMING_MAX_SLICE_COUNT 128                            /**< Maximum number of slices per frame */


/**
 * "Parrot Streaming" v1 user data SEI UUID.
 * UUID: 13dbccc7-c720-42f5-a0b7-aafaa2b3af97
 */
#define ARSTREAM2_H264_SEI_PARROT_STREAMING_V1_UUID_0 0x13dbccc7   /**< "Parrot Streaming" v1 user data SEI UUID part 1 (bit 0..31) */
#define ARSTREAM2_H264_SEI_PARROT_STREAMING_V1_UUID_1 0xc72042f5   /**< "Parrot Streaming" v1 user data SEI UUID part 2 (bit 32..63) */
#define ARSTREAM2_H264_SEI_PARROT_STREAMING_V1_UUID_2 0xa0b7aafa   /**< "Parrot Streaming" v1 user data SEI UUID part 3 (bit 64..95) */
#define ARSTREAM2_H264_SEI_PARROT_STREAMING_V1_UUID_3 0xa2b3af97   /**< "Parrot Streaming" v1 user data SEI UUID part 4 (bit 96..127) */


/**
 * @brief "Parrot Streaming" v1 user data SEI payload definition.
 */
typedef struct
{
    uint8_t indexInGop;                                 /**< Frame index in GOP */
    uint8_t sliceCount;                                 /**< Frame slice count */
    /* uint16_t sliceMbCount[sliceCount]; */            /**< Slice macroblock count */

} ARSTREAM2_H264Sei_ParrotStreamingV1_t;


/**
 * @brief "Parrot Streaming" v1 user data SEI definition.
 */
typedef struct
{
    uint32_t uuid[4];                                   /**< User data SEI UUID */
    ARSTREAM2_H264Sei_ParrotStreamingV1_t streaming;        /**< "Parrot Streaming" v1 */

} ARSTREAM2_H264Sei_UserDataParrotStreamingV1_t;


/**
 * @brief Serialize a "Parrot Streaming" v1 user data SEI.
 *
 * The function parses a "Parrot Streaming" v1 structure and fills the user data SEI buffer.
 * bufSize must be at least (sizeof(ARSTREAM2_H264Sei_UserDataParrotStreamingV1_t) + streaming->sliceCount * sizeof(uint16_t)).
 *
 * @param streaming Pointer to the "Parrot Streaming" v1 payload structure.
 * @param sliceMbCount Pointer to the sliceMbCount array.
 * @param pBuf Pointer to the user data SEI buffer to fill.
 * @param bufSize Size of the user data SEI buffer.
 * @param size Pointer to the final user data SEI buffer size.
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Sei_SerializeUserDataParrotStreamingV1(const ARSTREAM2_H264Sei_ParrotStreamingV1_t *streaming, const uint16_t *sliceMbCount, void* pBuf, unsigned int bufSize, unsigned int *size);


/**
 * @brief Deserialize a "Parrot Streaming" v1 user data SEI.
 *
 * The function parses a "Parrot Streaming" v1 user data SEI and fills the streaming structure and sliceMbCount array.
 *
 * @param pBuf Pointer to the user data SEI buffer.
 * @param bufSize Size of the user data SEI.
 * @param streaming Pointer to the "Parrot Streaming" v1 payload structure to fill.
 * @param sliceMbCount Pointer to the sliceMbCount array to fill (array size must be ARSTREAM2_H264_SEI_PARROT_STREAMING_MAX_SLICE_COUNT).
 *
 * @return ARSTREAM2_OK if no error occurred.
 * @return an eARSTREAM2_ERROR error code if an error occurred.
 */
eARSTREAM2_ERROR ARSTREAM2_H264Sei_DeserializeUserDataParrotStreamingV1(const void* pBuf, unsigned int bufSize, ARSTREAM2_H264Sei_ParrotStreamingV1_t *streaming, uint16_t *sliceMbCount);


/**
 * @brief Checks if the data provided is a "Parrot Streaming" v1 user data SEI.
 *
 * The function checks if the data provided in the pBuf buffer corresponds to a "Parrot Streaming" v1 user data SEI
 * using the user data SEI UUID.
 *
 * @param pBuf Pointer to the user data SEI buffer.
 * @param bufSize Size of the user data SEI.
 *
 * @return 1 if the data is a "Parrot Streaming" v1 user data SEI.
 * @return 0 if the data is not a "Parrot Streaming" v1 user data SEI.
 * @return -1 if an error occurred.
 */
int ARSTREAM2_H264Sei_IsUserDataParrotStreamingV1(const void* pBuf, unsigned int bufSize);


#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* #ifndef _ARSTREAM2_H264_SEI_H_ */
