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
 * @file ARCONTROLLER_Stream2.h
 * @brief ARCONTROLLER_Stream2 allow to operate ARStream 2 for receive a stream.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_STREAM2_H_
#define _ARCONTROLLER_STREAM2_H_

typedef struct ARCONTROLLER_Stream2_t ARCONTROLLER_Stream2_t;

/**
 * @brief Create a new Stream 2 Controller.
 * @warning This function allocate memory.
 * @post ARCONTROLLER_Stream2_Delete() must be called to delete the Stream 2 Controller and free the memory allocated.
 * @param[in] networkConfiguration The configuration of the network carring the stream.
 * @param[out] error Executing error.
 * @return The new Stream 2 Controller.
 * @see ARCONTROLLER_Stream2_Delete.
 */
ARCONTROLLER_Stream2_t *ARCONTROLLER_Stream2_New (ARDISCOVERY_Device_t *discoveryDevice, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Stream 2 Controller.
 * @warning This function free memory.
 * @param stream2Controller The Stream Controller to delete.
 * @see ARCONTROLLER_Stream2_New().
 */
void ARCONTROLLER_Stream2_Delete (ARCONTROLLER_Stream2_t **stream2Controller);

/**
 * @brief Start the Stream Controller.
 * @post ARCONTROLLER_Stream_Stop() must be called to stop the Stream Controller.
 * @param stream2Controller The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Device_Stop.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream2_Start (ARCONTROLLER_Stream2_t *stream2Controller);

/**
 * @brief Stop the Stream Controller.
 * @param stream2Controller The stream controller.
 * @return Executing error.
 * @see ARCONTROLLER_Stream_Start.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream2_Stop (ARCONTROLLER_Stream2_t *stream2Controller);

/**
 * @brief Set the callbacks of the frames events.
 * @param stream2Controller The stream controller.
 * @param[in] decoderConfigCallback decoder configuration callback function.
 * @param[in] receiveFrameCallback Callback when a frame is received.
 * @param[in] timeoutFrameCallback Callback when timeout in frame receiving.
 * @param[in] customData Data to set as argument to the callbacks.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream2_SetCallbacks(ARCONTROLLER_Stream2_t *stream2Controller, ARCONTROLLER_Stream_DecoderConfigCallback_t decoderConfigCallback, ARCONTROLLER_Stream_DidReceiveFrameCallback_t didReceiveFrameCallback, void *customData);

/**
 * @brief Checks if the stream2Controller is running.
 * @param stream2Controller The stream controller.
 * @param error Executing error.
 * @return 1 if the stream2Controller is running else 0.
 */
int ARCONTROLLER_Stream2_IsRunning (ARCONTROLLER_Stream2_t *stream2Controller, eARCONTROLLER_ERROR *error);

/**
 * @brief Callback to add a json part durring the connection.
 * @param stream2Controller The stream controller.
 * @param jsonObj connection json.
 * @return Executing error.
 */
eARDISCOVERY_ERROR ARCONTROLLER_Stream2_OnSendJson (ARCONTROLLER_Stream2_t *stream2Controller, json_object *jsonObj);

/**
 * @brief Callback to read a json part durring the connection.
 * @param stream2Controller The stream controller.
 * @param jsonObj connection json.
 * @return Executing error.
 */
eARDISCOVERY_ERROR ARCONTROLLER_Stream2_OnReceiveJson(ARCONTROLLER_Stream2_t *stream2Controller, json_object *jsonObj);

/**
 * @brief Checks if the connection json contains stream2 parameters.
 * @param jsonObj Json received during the connection.
 * @return different of 0 if the json contains stream2 parameters, otherwise 0.
 */
uint8_t ARCONTROLLER_Stream2_JsonContainsStream2Param(json_object *jsonObj);

/**
 * @brief Checks if the stream 2 manager is initialized.
 * @param @param stream2Controller The stream controller.
 * @return 1 if initialized, otherwise 0.
 */
int ARCONTROLLER_Stream2_IsInitilized (ARCONTROLLER_Stream2_t *stream2Controller);

/**
 * @brief Set stream compliant with the mp4 format.
 * @note Must be set to decode H264 stream with the iOS hardware decoder.
 * @param stream2Controller The stream controller.
 * @param isMP4Compliant 1 if the stream must be compliant with the mp4 format ; otherwide 0.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Stream2_SetMP4Compliant (ARCONTROLLER_Stream2_t *stream2Controller, int isMP4Compliant);

#endif /* _ARCONTROLLER_STREAM2_H_ */
