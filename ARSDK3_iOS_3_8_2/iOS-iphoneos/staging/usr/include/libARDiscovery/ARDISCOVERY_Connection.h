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
#ifndef _ARDISCOVERY_CONNECTION_H_
#define _ARDISCOVERY_CONNECTION_H_

#include <libARDiscovery/ARDISCOVERY_Error.h>

/**
 * @brief JSON strings for UDP ports extraction
 */
#define ARDISCOVERY_CONNECTION_JSON_STATUS_KEY                              "status"
#define ARDISCOVERY_CONNECTION_JSON_C2DPORT_KEY                             "c2d_port"
#define ARDISCOVERY_CONNECTION_JSON_D2CPORT_KEY                             "d2c_port"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM_FRAGMENT_SIZE_KEY              "arstream_fragment_size"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM_FRAGMENT_MAXIMUM_NUMBER_KEY    "arstream_fragment_maximum_number"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM_MAX_ACK_INTERVAL_KEY           "arstream_max_ack_interval"
#define ARDISCOVERY_CONNECTION_JSON_CONTROLLER_TYPE_KEY                     "controller_type"
#define ARDISCOVERY_CONNECTION_JSON_CONTROLLER_NAME_KEY                     "controller_name"
#define ARDISCOVERY_CONNECTION_JSON_DEVICE_ID_KEY                           "device_id"
#define ARDISCOVERY_CONNECTION_JSON_C2D_UPDATE_PORT_KEY                     "c2d_update_port"
#define ARDISCOVERY_CONNECTION_JSON_C2D_USER_PORT_KEY                       "c2d_user_port"
#define ARDISCOVERY_CONNECTION_JSON_SKYCONTROLLER_VERSION                   "skycontroller_version"
#define ARDISCOVERY_CONNECTION_JSON_FEATURES_KEY                            "features"

/**
 * ARStream2 specific keys
 */
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_CLIENT_STREAM_PORT_KEY        "arstream2_client_stream_port"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_CLIENT_CONTROL_PORT_KEY       "arstream2_client_control_port"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_SERVER_STREAM_PORT_KEY        "arstream2_server_stream_port"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_SERVER_CONTROL_PORT_KEY       "arstream2_server_control_port"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_MAX_PACKET_SIZE_KEY           "arstream2_max_packet_size"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_MAX_LATENCY_KEY               "arstream2_max_latency"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_MAX_NETWORK_LATENCY_KEY       "arstream2_max_network_latency"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_MAX_BITRATE_KEY               "arstream2_max_bitrate"
#define ARDISCOVERY_CONNECTION_JSON_ARSTREAM2_PARAMETER_SETS_KEY            "arstream2_parameter_sets"


/**
 * @brief Read/Write buffers max size
 */
#define ARDISCOVERY_CONNECTION_TX_BUFFER_SIZE 512
#define ARDISCOVERY_CONNECTION_RX_BUFFER_SIZE 512

/**
 * @brief callback use to send json information of the connection
 * @warning dataTx must not exceed ARDISCOVERY_CONNECTION_TX_BUFFER_SIZE and must ending by a NULL character
 * @param[out] dataTx Transmission buffer ; must be filled with the json information of the connection
 * @param[out] dataTxSize Transmission data size
 * @param[in] customData custom data
 * @return error during callback execution
 */
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_Connection_SendJsonCallback_t) (uint8_t *dataTx, uint32_t *dataTxSize, void *customData);

/**
 * @brief callback use to receive json information of the connection
 * @param[in] dataRx Reception buffer; containing json information of the connection
 * @param[in] dataRxSize Reception data size
 * @param[in] ip ip address of the sender
 * @param[in] customData custom data
 * @return error during callback execution
 */
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_Connection_ReceiveJsonCallback_t) (uint8_t *dataRx, uint32_t dataRxSize, char *ip, void *customData);

/**
 * @brief Structures to allow data sharing across connection process
 */
typedef struct ARDISCOVERY_Connection_ConnectionData_t ARDISCOVERY_Connection_ConnectionData_t;

/**
 * @brief Create and initialize connection data
 * @param[in] callback Connection data management callback
 * @param[in] customData custom data
 * @param[out] error Error code
 * @return new connection data object
 */
ARDISCOVERY_Connection_ConnectionData_t* ARDISCOVERY_Connection_New (ARDISCOVERY_Connection_SendJsonCallback_t sendJsonCallback, ARDISCOVERY_Connection_ReceiveJsonCallback_t receiveJsonCallback, void *customData, eARDISCOVERY_ERROR *error);

/**
 * @brief Delete connection data
 * @param[in] connectionData Connection data
 * @param[out] error Error code
 */
eARDISCOVERY_ERROR ARDISCOVERY_Connection_Delete (ARDISCOVERY_Connection_ConnectionData_t **connectionData);

/**
 * @brief Initialize connection as a Device
 * @warning Must be called in its own thread
 * @post ARDISCOVERY_Connection_Device_StopListening() must be called to close the connection.
 * @param[in] connectionData Connection data
 * @param[in] port port use to the discovery
 * @return error during execution
 * @see ARDISCOVERY_Connection_Device_StopListening()
 */
eARDISCOVERY_ERROR ARDISCOVERY_Connection_DeviceListeningLoop (ARDISCOVERY_Connection_ConnectionData_t *connectionData, int port);

/**
 * @brief Close connection
 * @warning blocking function ; wait the end of the run
 * @param[in] connectionData Connection data
 * @see ARDISCOVERY_Connection_DeviceListeningLoop()
 */
void ARDISCOVERY_Connection_Device_StopListening (ARDISCOVERY_Connection_ConnectionData_t *connectionData);

/**
 * @brief Initialize connection as a Controller
 * @warning blocking function
 * @post ARDISCOVERY_Connection_Device_StopListening() must be called to close the connection.
 * @param[in] connectionData Connection data
 * @param[in] port port use to the discovery
 * @param[in] ip device IP address
 * @return error during execution
 * @see ARDISCOVERY_Connection_Device_StopListening()
 */
eARDISCOVERY_ERROR ARDISCOVERY_Connection_ControllerConnection (ARDISCOVERY_Connection_ConnectionData_t *connectionData, int port, const char *ip);

/**
 * @brief Copy the IP address of the connected controller device
 * @param[in] connectionData Connection data
 * @param[in] buffer String buffer to copy the IP
 */
void ARDISCOVERY_Connection_getControllerIP (ARDISCOVERY_Connection_ConnectionData_t *connectionData, char* buffer);

/**
 * @brief Abort connection
 * @warning blocking function ; wait the end of the run
 * @param[in] connectionData Connection data
 * @see ARDISCOVERY_Connection_ControllerConnection()
 */
static inline  void ARDISCOVERY_Connection_ControllerConnectionAbort (ARDISCOVERY_Connection_ConnectionData_t *connectionData)
{
    return ARDISCOVERY_Connection_Device_StopListening (connectionData);
}

#endif /* _ARDISCOVERY_CONNECTION_H_ */
