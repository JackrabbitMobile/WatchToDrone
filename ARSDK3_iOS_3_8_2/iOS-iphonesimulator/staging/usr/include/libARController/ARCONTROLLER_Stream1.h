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
 * @file ARCONTROLLER_Stream1.h
 * @brief ARCONTROLLER_Stream1 allow to operate ARStream for receive a stream.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_STREAM1_H_
#define _ARCONTROLLER_STREAM1_H_

#include <json/json.h>
#include <libARSAL/ARSAL_Thread.h>
#include <libARNetwork/ARNETWORK_Manager.h>
#include <libARController/ARCONTROLLER_Error.h>
#include <libARDiscovery/ARDISCOVERY_Error.h>
#include <libARDiscovery/ARDISCOVERY_Device.h>
#include <libARController/ARCONTROLLER_Frame.h>
#include <libARController/ARCONTROLLER_StreamQueue.h>

/**
 * @brief Stream controller allow to operate ARStream for receive a stream.
 */
typedef struct ARCONTROLLER_Stream1_t ARCONTROLLER_Stream1_t;

/**
 * @brief Create a new Stream Controller.
 * @warning This function allocate memory.
 * @post ARCONTROLLER_Stream1_Delete() must be called to delete the Stream Controller and free the memory allocated.
 * @param[in] networkConfiguration The configuration of the network carring the stream.
 * @param[out] error Executing error.
 * @return The new Stream Controller.
 * @see ARCONTROLLER_Stream1_Delete.
 */
ARCONTROLLER_Stream1_t *ARCONTROLLER_Stream1_New (ARDISCOVERY_NetworkConfiguration_t *networkConfiguration, eARCONTROLLER_STREAM_CODEC_TYPE codecType, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Stream Controller.
 * @warning This function free memory.
 * @param stream1Controller The Stream Controller to delete.
 * @see ARCONTROLLER_Stream1_New().
 */
void ARCONTROLLER_Stream1_Delete (ARCONTROLLER_Stream1_t **stream1Controller);

/**
 * @brief Start the Stream Controller.
 * @post ARCONTROLLER_Stream1_Stop() must be called to stop the Stream Controller.
 * @param stream1Controller The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Device_Stop.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream1_Start (ARCONTROLLER_Stream1_t *stream1Controller, ARNETWORK_Manager_t *networkManager);

/**
 * @brief Stop the Stream Controller.
 * @param stream1Controller The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Stream1_Start.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream1_Stop (ARCONTROLLER_Stream1_t *stream1Controller);

/**
 * @brief Set the callbacks of the frames events.
 * @param stream1Controller The stream controller.
 * @param[in] receiveFrameCallback Callback when a frame is received.
 * @param[in] timeoutFrameCallback Callback when timeout in frame receiving.
 * @param[in] customData Data to set as argument to the callbacks.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream1_SetCallbacks (ARCONTROLLER_Stream1_t *stream1Controller, ARCONTROLLER_Stream_DecoderConfigCallback_t decoderConfigCallback, ARCONTROLLER_Stream_DidReceiveFrameCallback_t receiveFrameCallback, ARCONTROLLER_Stream_TimeoutFrameCallback_t timeoutFrameCallback, void *customData);

/**
 * @brief Callback to read a json part durring the connection.
 * @param stream1Controller The stream controller.
 * @param jsonObj connection json.
 * @return Executing error.
 */
eARDISCOVERY_ERROR ARCONTROLLER_Stream1_OnReceiveJson(ARCONTROLLER_Stream1_t *stream1Controller, json_object *jsonObj);

/**
 * @brief Checks if the stream1Controller is running.
 * @param stream1Controller The stream controller.
 * @param error Executing error.
 * @return 1 if the stream1Controller is running else 0.
 */
int ARCONTROLLER_Stream1_IsRunning (ARCONTROLLER_Stream1_t *stream1Controller, eARCONTROLLER_ERROR *error);

/**
 * @brief Set stream compliant with the mp4 format.
 * @note Must be set to decode H264 stream with the iOS hardware decoder.
 * @param stream1Controller The stream controller.
 * @param isMP4Compliant 1 if the stream must be compliant with the mp4 format ; otherwide 0.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream1_SetMP4Compliant (ARCONTROLLER_Stream1_t *stream1Controller, int isMP4Compliant);

#endif /* _ARCONTROLLER_STREAM1_H_ */
