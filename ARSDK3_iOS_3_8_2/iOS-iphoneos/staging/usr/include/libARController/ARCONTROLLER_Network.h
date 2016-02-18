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
 * @file ARCONTROLLER_Network.h
 * @brief ARCONTROLLER_Network allow to operate an ARNETWORK_Manager for send and receive commands.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_NETWORK_H_
#define _ARCONTROLLER_NETWORK_H_

#include <libARSAL/ARSAL_Thread.h>
#include <libARNetwork/ARNETWORK_Manager.h>
#include <libARDiscovery/ARDISCOVERY_Device.h>
#include <libARController/ARCONTROLLER_Error.h>
#include <libARController/ARCONTROLLER_Stream.h>

/**
 * Enum characterizing the type of data to send
 */
typedef enum
{
    ARCONTROLLER_NETWORK_SENDING_DATA_TYPE_NOT_ACK = 0, /**< data NOT acknowledged */
    ARCONTROLLER_NETWORK_SENDING_DATA_TYPE_ACK, /**< data acknowledged */
    ARCONTROLLER_NETWORK_SENDING_DATA_TYPE_HIGH_PRIORITY, /**< high priority data*/
    ARCONTROLLER_NETWORK_SENDING_DATA_TYPE_STREAM, /**< strem data */
    
    ARCONTROLLER_NETWORK_SENDING_DATA_TYPE_MAX /**< Max of the enumeration */
}eARCONTROLLER_NETWORK_SENDING_DATA_TYPE;

/**
 * Enum characterizing the states of the network controller
 */
typedef enum
{
    ARCONTROLLER_NETWORK_STATE_STOPPED = 0, /**< network controller stopped */
    ARCONTROLLER_NETWORK_STATE_RUNNING, /**< network controller running */
    ARCONTROLLER_NETWORK_STATE_PAUSE, /**< network controller in pause */
    
    ARCONTROLLER_NETWORK_STATE_MAX /**< Max of the enumeration */
}eARCONTROLLER_NETWORK_STATE;

/**
 * @brief Network controller allow to operate an ARNETWORK_Manager for send and receive commands.
 */
typedef struct ARCONTROLLER_Network_t ARCONTROLLER_Network_t;

/**
 * @brief Sending Configuration used to know the timeout policy in case of sending data timeout.
 */
typedef struct
{
    eARNETWORK_MANAGER_CALLBACK_RETURN timeoutPolicy; /**< Timeout policy in case of sending data timeout */
    void *customData; /**< Custom data */
}ARCONTROLLER_NETWORK_SendingConfiguration_t;

/**
 * @brief Callback used on disconnection of the network
 * @param[in] customData Data given at the registering of the callback.
 */
typedef void (*ARCONTROLLER_Network_DisconnectionCallback_t) (void *customData);

/**
 * @brief Create a new Network Controller
 * @warning This function allocate memory
 * @post ARCONTROLLER_Network_Delete() must be called to delete the Network Controller and free the memory allocated.
 * @param[in] discoveryDevice The device to drive ; must be not NULL. This device will be copied and can be deleted after the call of this function.
 * @param[out] error Executing error.
 * @return the new Network Controller
 * @see ARCONTROLLER_Network_Delete()
 */
ARCONTROLLER_Network_t *ARCONTROLLER_Network_New (ARDISCOVERY_Device_t *discoveryDevice, ARCONTROLLER_Network_DisconnectionCallback_t disconnectionCallback, ARDISCOVERY_Device_ConnectionJsonCallback_t sendJsonCallback, ARDISCOVERY_Device_ConnectionJsonCallback_t receiveJsonCallback, void *customData, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Network Controller
 * @warning This function free memory
 * @param networkController The network controller to delete
 * @see ARCONTROLLER_Network_New()
 */
void ARCONTROLLER_Network_Delete (ARCONTROLLER_Network_t **networkController);

/**
 * @brief Pauses the Network Controller
 * @note Network Controller state must be ARCONTROLLER_NETWORK_STATE_RUNNING ; nothing will be done in others cases
 * @param[in] networkController The network Controller ; must be not NULL.
 * @return executing error
 * @see ARCONTROLLER_Network_Resume()
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_Pause (ARCONTROLLER_Network_t *networkController);

/**
 * @brief Resumes the Network Controller
 * @note Network Controller state must be ARCONTROLLER_NETWORK_STATE_PAUSE ; nothing will be done in ARCONTROLLER_NETWORK_STATE_RUNNING case ; in case ARCONTROLLER_NETWORK_STATE_STOPPED an error ARCONTROLLER_ERROR_STATE will be returned.
 * @param[in] networkController The network Controller ; must be not NULL.
 * @return executing error
 * @see ARCONTROLLER_Network_Pause()
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_Resume (ARCONTROLLER_Network_t *networkController);

/**
 * @brief Set the callbacks of the video stream events.
 * @param networkController The network Controller ; must be not NULL.
 * @param[in] decoderConfigCallback decoder configuration callback function.
 * @param[in] receiveFrameCallback Callback when a frame is received.
 * @param[in] timeoutFrameCallback Callback when timeout in frame receiving.
 * @param[in] customData Data to set as argument to the callbacks.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_SetVideoReceiveCallback (ARCONTROLLER_Network_t *networkController, ARCONTROLLER_Stream_DecoderConfigCallback_t decoderConfigCallback, ARCONTROLLER_Stream_DidReceiveFrameCallback_t receiveFrameCallback, ARCONTROLLER_Stream_TimeoutFrameCallback_t timeoutFrameCallback, void *customData);

/**
 * @brief Set video stream compliant with the mp4 format.
 * @note Must be set to decode H264 stream with the iOS hardware decoder.
 * @param networkController The network Controller ; must be not NULL.
 * @param isMP4Compliant 1 if the video stream must be compliant with mp4 format ; otherwide 0.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_SetVideoStreamMP4Compliant (ARCONTROLLER_Network_t *networkController, int isMP4Compliant);

/**
 * @brief Start Video stream.
 * @param networkController The network Controller ; must be not NULL.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_StartVideoStream (ARCONTROLLER_Network_t *networkController);

/**
 * @brief Stop Video stream.
 * @param networkController The network Controller ; must be not NULL.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_StopVideoStream (ARCONTROLLER_Network_t *networkController);

/**
 * @brief Send data through the network.
 * @param networkController The network Controller ; must be not NULL.
 * @param[in] data The data to send.
 * @param[in] dataSize The data size.
 * @param[in] dataType Type of the data to send.
 * @param[in] timeoutPolicy The policy to use when timeout.
 * @param[in] netError executing network error.
 * @return Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Network_SendData (ARCONTROLLER_Network_t *networkController, void *data, int dataSize, eARCONTROLLER_NETWORK_SENDING_DATA_TYPE dataType, eARNETWORK_MANAGER_CALLBACK_RETURN timeoutPolicy, eARNETWORK_ERROR *netError);

#endif /* _ARCONTROLLER_NETWORK_H_ */
