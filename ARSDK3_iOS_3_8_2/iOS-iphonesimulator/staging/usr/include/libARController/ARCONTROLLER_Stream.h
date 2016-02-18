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
 * @file ARCONTROLLER_Stream.h
 * @brief ARCONTROLLER_Stream allow to operate ARStream for receive a stream.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_STREAM_H_
#define _ARCONTROLLER_STREAM_H_

#include <json/json.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARNetwork/ARNETWORK_Manager.h>
#include <libARController/ARCONTROLLER_Error.h>
#include <libARDiscovery/ARDISCOVERY_Error.h>
#include <libARDiscovery/ARDISCOVERY_Device.h>
#include <libARController/ARCONTROLLER_Frame.h>
#include <libARController/ARCONTROLLER_StreamQueue.h>

/**
 * @brief Callback when a frame is received.
 * @param[in] customData Data given at the registering of the callback.
 * @return Executing error.
 */
typedef eARCONTROLLER_ERROR (*ARCONTROLLER_Stream_DidReceiveFrameCallback_t) (ARCONTROLLER_Frame_t *frame, void *customData);

/**
 * @brief Type of codec
 */
typedef enum
{
    ARCONTROLLER_STREAM_CODEC_TYPE_DEFAULT = 0, /**< default value */
    ARCONTROLLER_STREAM_CODEC_TYPE_H264, /**< h264 codec */
    ARCONTROLLER_STREAM_CODEC_TYPE_MJPEG, /**< MJPEG codec */
    ARCONTROLLER_STREAM_CODEC_TYPE_MAX, /**< Max of the enumeration */
} eARCONTROLLER_STREAM_CODEC_TYPE;

/**
 * @brief H264 Parameters.
 */
typedef struct 
{
    uint8_t *spsBuffer; /**< Pointer to the SPS NAL unit buffer */
    int spsSize; /**< Size in bytes of the SPS NAL unit */
    uint8_t *ppsBuffer; /**< Pointer to the PPS NAL unit buffer */
    int ppsSize; /**< Size in bytes of the PPS NAL unit */
    int isMP4Compliant; /**< 1 if the stream is compliant with the mp4 format; the NAL units start code are replaced by the NALU size */
}ARCONTROLLER_Stream_CodecH264_t;

/**
 * @brief Codec parameters.
 */
typedef union 
{
    ARCONTROLLER_Stream_CodecH264_t h264parameters; /**< H264 Parameters. */
}ARCONTROLLER_Stream_CodecParameters_t;

/**
 * @brief Codec description.
 */
typedef struct 
{
    eARCONTROLLER_STREAM_CODEC_TYPE type; /**< Codec type. */
    ARCONTROLLER_Stream_CodecParameters_t parameters; /**< Parameters of the codec. */
}ARCONTROLLER_Stream_Codec_t;

/**
 * @brief Callback to configure the decoder.
 * @param[in] codec codec of the stream.
 * @param[in] customData Data given at the registering of the callback.
 * @return Executing error.
 */
typedef eARCONTROLLER_ERROR (*ARCONTROLLER_Stream_DecoderConfigCallback_t) (ARCONTROLLER_Stream_Codec_t codec, void *customData);

/**
 * @brief Callback when timeout in frame receiving
 * @param[in] customData Data given at the registering of the callback.
 */
typedef void (*ARCONTROLLER_Stream_TimeoutFrameCallback_t) (void *customData);

/**
 * @brief Stream controller allow to operate ARStream for receive a stream.
 */
typedef struct ARCONTROLLER_Stream_t ARCONTROLLER_Stream_t;

/**
 * @brief Create a new Stream Controller.
 * @warning This function allocate memory.
 * @post ARCONTROLLER_Stream_Delete() must be called to delete the Stream Controller and free the memory allocated.
 * @param[in] networkConfiguration The configuration of the network carring the stream.
 * @param[out] error Executing error.
 * @return The new Stream Controller.
 * @see ARCONTROLLER_Stream_Delete.
 */
ARCONTROLLER_Stream_t *ARCONTROLLER_Stream_New (ARDISCOVERY_NetworkConfiguration_t *networkConfiguration, ARDISCOVERY_Device_t *discoveryDevice, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Stream Controller.
 * @warning This function free memory.
 * @param streamController The Stream Controller to delete.
 * @see ARCONTROLLER_Stream_New().
 */
void ARCONTROLLER_Stream_Delete (ARCONTROLLER_Stream_t **streamController);

/**
 * @brief Start the Stream Controller.
 * @post ARCONTROLLER_Stream_Stop() must be called to stop the Stream Controller.
 * @param streamController The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Device_Stop.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream_Start (ARCONTROLLER_Stream_t *streamController, ARNETWORK_Manager_t *networkManager);

/**
 * @brief Stop the Stream Controller.
 * @param streamController The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Stream_Start.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream_Stop (ARCONTROLLER_Stream_t *streamController);

/**
 * @brief Set stream compliant with the mp4 format.
 * @note Must be set to decode H264 stream with the iOS hardware decoder.
 * @param streamController The stream controller.
 * @param isMP4Compliant 1 if the stream must be compliant with the mp4 format ; otherwide 0.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream_SetMP4Compliant (ARCONTROLLER_Stream_t *streamController, int isMP4Compliant);

/**
 * @brief Set the callbacks of the frames events.
 * @param streamController The stream controller.
 * @param[in] decoderConfigCallback decoder configuration callback function.
 * @param[in] receiveFrameCallback Callback when a frame is received.
 * @param[in] timeoutFrameCallback Callback when timeout in frame receiving.
 * @param[in] customData Data to set as argument to the callbacks.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream_SetReceiveFrameCallback (ARCONTROLLER_Stream_t *streamController, ARCONTROLLER_Stream_DecoderConfigCallback_t decoderConfigCallback, ARCONTROLLER_Stream_DidReceiveFrameCallback_t receiveFrameCallback, ARCONTROLLER_Stream_TimeoutFrameCallback_t timeoutFrameCallback, void *customData);

/**
 * @brief Callback to add a json part durring the connection.
 * @param streamController The stream controller.
 * @param jsonObj connection json.
 * @return Executing error.
 */
eARDISCOVERY_ERROR ARCONTROLLER_Stream_OnSendJson (ARCONTROLLER_Stream_t *streamController, json_object *jsonObj);

/**
 * @brief Callback to read a json part durring the connection.
 * @param streamController The stream controller.
 * @param jsonObj connection json.
 * @return Executing error.
 */
eARDISCOVERY_ERROR ARCONTROLLER_Stream_OnReceiveJson(ARCONTROLLER_Stream_t *streamController, json_object *jsonObj);

#endif /* _ARCONTROLLER_STREAM_H_ */
