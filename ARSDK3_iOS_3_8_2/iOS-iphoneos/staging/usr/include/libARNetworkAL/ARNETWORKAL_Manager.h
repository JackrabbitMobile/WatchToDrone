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
 * @file ARNETWORKAL_Manager.h
 * @brief network manager allow to send over network.
 * @date 04/29/2013
 * @author frederic.dhaeyer@parrot.com
 */

#ifndef _ARNETWORKAL_MANAGER_H_
#define _ARNETWORKAL_MANAGER_H_

#include <stdio.h>
#include <time.h>
#include <libARNetworkAL/ARNETWORKAL_Error.h>
#include <libARNetworkAL/ARNETWORKAL_Frame.h>

#define ARNETWORKAL_MANAGER_DEFAULT_ID_MAX  256 /**< Default ID Max */
#define ARNETWORKAL_MANAGER_WIFI_ID_MAX ARNETWORKAL_MANAGER_DEFAULT_ID_MAX /**< ID Max for WifiNetwork */
#define ARNETWORKAL_MANAGER_BLE_ID_MAX  (ARNETWORKAL_MANAGER_DEFAULT_ID_MAX / 8) /**< ID Max for BLENetwork */

#define ARNETWORKAL_WIFINETWORK_DISCONNECT_TIMEOUT_MS 5000 /**< timeout in millisecond before to account a disconnection */

#define ARNETWORKAL_BLENETWORK_MEDIA_MTU            20
#define ARNETWORKAL_BLENETWORK_HEADER_SIZE          2
#define ARNETWORKAL_BLENETWORK_MAX_BUFFER_SIZE      (ARNETWORKAL_BLENETWORK_MEDIA_MTU - ARNETWORKAL_BLENETWORK_HEADER_SIZE)

/**
 * @brief ARNETWORKAL_Manager_t - Network abstraction structure.
 * @see ARNETWORKAL_Manager_PushNextFrame_t
 * @see ARNETWORKAL_Manager_PopNextFrame_t
 * @see ARNETWORKAL_Manager_Sending_t
 * @see ARNETWORKAL_Manager_Receiving_t
 */
typedef struct ARNETWORKAL_Manager_t ARNETWORKAL_Manager_t;

/**
 * @brief BLE Manager (used to send/read data)
 * @note This is an application-provided object, OS Dependant
 */
typedef void* ARNETWORKAL_BLEDeviceManager_t;

/**
 * @brief BLE Device reference for the @ref ARNETWORKAL_BLEDeviceManager_t
 * @note This is an application-provided object, OS Dependant
 */
typedef void* ARNETWORKAL_BLEDevice_t;

/**
 * @brief Transfert functions return codes
 */
typedef enum
{
    ARNETWORKAL_MANAGER_RETURN_DEFAULT = 0, /**< Default return value (no error)*/
    ARNETWORKAL_MANAGER_RETURN_BUFFER_FULL, /**< Impossible to push a frame : network buffer is full */
    ARNETWORKAL_MANAGER_RETURN_BUFFER_EMPTY, /**< Impossible to pop a frame, no frame in the buffer */
    ARNETWORKAL_MANAGER_RETURN_BAD_FRAME, /**< Impossible to pop a frame, frame is corrupted */
    ARNETWORKAL_MANAGER_RETURN_NO_DATA_AVAILABLE, /**< Impossible to read data from the network, no data available */
    ARNETWORKAL_MANAGER_RETURN_BAD_PARAMETERS, /**< Parameters given to the callback were not good */
    ARNETWORKAL_MANAGER_RETURN_NETWORK_ERROR, /**< Network error while reading or sending data */
} eARNETWORKAL_MANAGER_RETURN;

/**
 * @brief Queues data to be sent on the network
 * This function will queue an @ref ARNETWORKAL_Frame_t into the internal
 * buffers. Depending on the implementation, the send may also be triggered
 * by this call (like if a call to @ref ARNETWORKAL_Manager_Send_t was made)
 * @param manager The manager which should queue the data
 * @param frame The frame to be sent
 * @return A callback return code
 */
typedef eARNETWORKAL_MANAGER_RETURN (*ARNETWORKAL_Manager_PushFrame_t) (ARNETWORKAL_Manager_t *manager, ARNETWORKAL_Frame_t *frame);

/**
 * @brief Sends all queued datas on the network
 * This function will trigger an actual network send of all the queued data. On
 * implementations where @ref ARNETWORKAL_Manager_PushFrame_t triggers a send,
 * this function is a no-op.
 * @param manager The manager which should send all its queued data
 * @return A callback return code
 */
typedef eARNETWORKAL_MANAGER_RETURN (*ARNETWORKAL_Manager_Send_t) (ARNETWORKAL_Manager_t *manager);

/**
 * @brief Read a bunch of frames from the network
 * This function will read a bunch of data from the network, so these datas can
 * be fetched using @ref ARNETWORKAL_Manager_PopFrame_t. On implementations
 * where @ref ARNETWORKAL_Manager_PopFrame_t does an actual network read, this
 * function only waits for data to be available, but does not do an actual
 * read. In any cases, this function should be used by the receiving thread to
 * wait for new network data.
 * @param manager The manager which should read from the network
 * @return A callback return code
 */
typedef eARNETWORKAL_MANAGER_RETURN (*ARNETWORKAL_Manager_Receive_t) (ARNETWORKAL_Manager_t *manager);

/**
 * @brief Fetches data from the last received network frame
 * This function will try to read an ::ARNETWORKAL_Frame_t from the last
 * received network frame. Depending on the implementation, this function may
 * directly read from the network instead of an internal buffer.
 * @param manager The manager which should fetch the data
 * @param frame A frame pointer in which the fetched frame should be saved
 * @return A callback return code
 */
typedef eARNETWORKAL_MANAGER_RETURN (*ARNETWORKAL_Manager_PopFrame_t) (ARNETWORKAL_Manager_t *manager, ARNETWORKAL_Frame_t *frame);

/**
 * @brief unlock all functions locked.
 * this function is call by ARNetwork to permit to join its threads.
 * @param manager The manager which should unlock all its locking functions.
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
typedef eARNETWORKAL_ERROR (*ARNETWORKAL_Manager_Unlock_t) (ARNETWORKAL_Manager_t *manager);

/**
 * @brief gets the network bandwidth.
 * @param manager The manager.
 * @param[out] pointer which will hold the upload bandwidth, in bytes per second (optionnal, can be NULL)
 * @param[out] pointer which will hold the download bandwidth, in bytes per second (optionnal, can be NULL)
 * @return error see ::eARNETWORKAL_ERROR
 */
typedef eARNETWORKAL_ERROR (*ARNETWORKAL_Manager_GetBandwidth_t) (ARNETWORKAL_Manager_t *manager, uint32_t *uploadBw, uint32_t *downloadBw);

/**
 * @brief Entry point for the network bandwidth measurement thread.
 * This thread needs to be started for the ::ARNETWORKAL_Manager_GetBandwidth_t to get any useful information.
 * @note This thread will be stopped by the ARNETWORKAL_CloseXXXX() function.
 * @param manager The manager, casted as a void *
 * @return always returns (void *)0
 */
typedef void* (*ARNETWORKAL_Manager_BandwidthThread_t) (void *manager);

/**
 * @brief function called on disconnect
 * @param manager The manager
 * @param customData The custom data
 */
typedef void (*ARNETWORKAL_Manager_OnDisconnect_t) (ARNETWORKAL_Manager_t *manager, void* customData);

/**
 * @brief function called on disconnect
 * @param manager The manager
 * @param onDisconnectCallback The function to call on disconnection
 * @param customData The custom data
 */
typedef eARNETWORKAL_ERROR (*ARNETWORKAL_Manager_SetOnDisconnectCallback_t) (ARNETWORKAL_Manager_t *manager, ARNETWORKAL_Manager_OnDisconnect_t onDisconnectCallback, void *customData);

/**
 * @brief Sets a buffer size
 * @param manager The manager
 * @param bufferSize The requested buffer size
 * @return error see ::eARNETWORKAL_ERROR
 */
typedef eARNETWORKAL_ERROR (*ARNETWORKAL_Manager_SetBufferSize_t) (ARNETWORKAL_Manager_t *manager, int bufferSize);

/**
 * @brief Gets a buffer size
 * @param manager The manager
 * @param bufferSize Pointer which will hold the requested size
 * @return error see ::eARNETWORKAL_ERROR
 */
typedef eARNETWORKAL_ERROR (*ARNETWORKAL_Manager_GetBufferSize_t) (ARNETWORKAL_Manager_t *manager, int *bufferSize);

/**
 * @brief ARNETWORKAL_Manager_t - Network abstraction structure.
 * @see ARNETWORKAL_Manager_PushNextFrame_t
 * @see ARNETWORKAL_Manager_PopNextFrame_t
 * @see ARNETWORKAL_Manager_Sending_t
 * @see ARNETWORKAL_Manager_Receiving_t
 */
struct ARNETWORKAL_Manager_t
{
    ARNETWORKAL_Manager_PushFrame_t pushFrame; /**< Manager specific PushFrame function */
    ARNETWORKAL_Manager_PopFrame_t popFrame; /**< Manager specific PopFrame function */
    ARNETWORKAL_Manager_Send_t send; /**< Manager specific send function */
    ARNETWORKAL_Manager_Receive_t receive; /**< Manager specific receive function */
    ARNETWORKAL_Manager_Unlock_t unlock; /**< Manager specific unlock function */
    ARNETWORKAL_Manager_GetBandwidth_t getBandwidth; /**< Manager specific getBandwidth function */
    ARNETWORKAL_Manager_BandwidthThread_t bandwidthThread; /**< Manager specific bandwidth thread EP */
    ARNETWORKAL_Manager_SetOnDisconnectCallback_t setOnDisconnectCallback; /**< function to set the onDisconnect callback */
    ARNETWORKAL_Manager_SetBufferSize_t setSendBufferSize; /**< Manager specific set send buffer size */
    ARNETWORKAL_Manager_SetBufferSize_t setRecvBufferSize; /**< Manager specific set recv buffer size */
    ARNETWORKAL_Manager_GetBufferSize_t getSendBufferSize; /**< Manager specific get send buffer size */
    ARNETWORKAL_Manager_GetBufferSize_t getRecvBufferSize; /**< Manager specific get recv buffer size */
    void *senderObject; /**< Internal reference, do not use */
    void *receiverObject; /**< Internal reference, do not use */
    int maxIds; /**< Maximum supported buffer ID for ARNetwork */
    unsigned int maxBufferSize; /**< Maximum size allowed for a buffer (size available for data). */

    FILE *dumpFile; /**< Output file for dumping traffic */
};

/**
 * @brief Create a new Manager
 * @warning This function allocate memory
 * @post ARNETWORKAL_Manager_Delete() must be called to delete the manager and free the memory allocated.
 * @param[out] error pointer on the error output.
 * @return Pointer on the new Manager
 * @see ARNETWORKAL_Manager_Delete()
 */
ARNETWORKAL_Manager_t* ARNETWORKAL_Manager_New(eARNETWORKAL_ERROR *error);

/**
 * @brief Delete the Manager
 * @warning This function free memory
 * @param manager address of the pointer on the Manager
 * @see ARNETWORKAL_Manager_New()
 */
void ARNETWORKAL_Manager_Delete(ARNETWORKAL_Manager_t **manager);

/**
 * @brief gets the network bandwidth.
 * @param manager The manager.
 * @param[out] pointer which will hold the upload bandwidth, in bytes per second (optionnal, can be NULL)
 * @param[out] pointer which will hold the download bandwidth, in bytes per second (optionnal, can be NULL)
 * @return error see ::eARNETWORKAL_ERROR
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_GetBandwidth (ARNETWORKAL_Manager_t *manager, uint32_t *uploadBw, uint32_t *downloadBw);

/**
 * @brief Entry point for the bandwidth thread of the manager.
 * This threads needs to be starter in order to get meaningful informations
 * with ARNETWORKAL_Manager_GetBandwidth().
 * @note The thread will be stopped by the ARNETWORKAL_Manager_CloseXxxx() call.
 * @warning Only call this function once on a manager.
 * @param manager The manager, casted as a void *.
 * @return Always returns (void *)0.
 */
void* ARNETWORKAL_Manager_BandwidthThread (void *manager);

/**
 * @brief unlock all functions locked.
 * this function is call by ARNetwork to permit to join its threads.
 * @param manager The manager which should unlock all its locking functions.
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_Unlock (ARNETWORKAL_Manager_t *manager);


/**
 * @brief initialize Wifi network.
 * @param manager pointer on the Manager
 * @param[in] addr IP address of connection at which the data will be sent.
 * @param[in] sendingPort port on which the data will be sent.
 * @param[in] receivingPort port on which the data will be received.
 * @param[in] recvTimeoutSec timeout in seconds set to limit the time of blocking of the Receive function.
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_InitWifiNetwork(ARNETWORKAL_Manager_t *manager, const char *addr, int sendingPort, int receivingPort, int recvTimeoutSec);

/**
 * @brief Cancel the initialization Wifi network.
 * @param manager pointer on the Manager
 * @return error equal to ARNETWORKAL_OK if the initialization if successful Canceled otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_CancelWifiNetwork (ARNETWORKAL_Manager_t *manager);

/**
 * @brief close Wifi network.
 * @param manager pointer on the Manager
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_CloseWifiNetwork(ARNETWORKAL_Manager_t *manager);

/**
 * @brief initialize BLE network.
 * @param manager pointer on the Manager
 * @param deviceManager the OS device manager which will be used for network functions
 * @param device the selected OS specific BLE device to connect to
 * @param recvTimeoutSec timeout in seconds set to limit the time of blocking of the Receive function.
 * @param notificationIDs list of the buffer ID to notify. If NULL all buffers of the receiver are notify.
 * @param numberOfNotificationID number of buffer ID to notifiy.
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_InitBLENetwork(ARNETWORKAL_Manager_t *manager, ARNETWORKAL_BLEDeviceManager_t deviceManager, ARNETWORKAL_BLEDevice_t device, int recvTimeoutSec, int *notificationIDs, int numberOfNotificationID);

/**
 * @brief Cancel the initialization BLE network.
 * @param manager pointer on the Manager.
 * @return error equal to ARNETWORKAL_OK if the initialization if successful Canceled otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_CancelBLENetwork (ARNETWORKAL_Manager_t *manager);

/**
 * @brief close BLE network.
 * @param manager pointer on the Manager
 * @return error equal to ARNETWORKAL_OK if the initialization if successful otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_CloseBLENetwork(ARNETWORKAL_Manager_t *manager);

/**
 * @brief set the OnDisconnect Callback
 * @warning Only call by the ARNetworkManager
 * @param manager pointer on the Manager
 * @param onDisconnectCallbak function called on disconnect
 * @param customData custom data sent to the onDisconnectCallback
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_SetOnDisconnectCallback(ARNETWORKAL_Manager_t *manager, ARNETWORKAL_Manager_OnDisconnect_t onDisconnectCallback, void *customData);

/**
 * @brief Sets the size of the send buffer
 * @param manager pointer on the Manager
 * @param bufferSize requested size for the buffer
 * @return ARNETWORKAL_OK if the buffer size was set, otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_SetSendBufferSize(ARNETWORKAL_Manager_t *manager, int bufferSize);

/**
 * @brief Sets the size of the revceive buffer
 * @param manager pointer on the Manager
 * @param bufferSize requested size for the buffer
 * @return ARNETWORKAL_OK if the buffer size was set, otherwise see eARNETWORKAL_ERROR.
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_SetRecvBufferSize(ARNETWORKAL_Manager_t *manager, int bufferSize);

/**
 * @brief Gets the size of the send buffer
 * @param manager pointer on the Manager
 * @param bufferSize Pointer which will hold the size of the buffer
 * @return see ::eARNETWORKAL_ERROR
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_GetSendBufferSize(ARNETWORKAL_Manager_t *manager, int *bufferSize);

/**
 * @brief Gets the size of the receive buffer
 * @param manager pointer on the Manager
 * @param bufferSize Pointer which will hold the size of the buffer
 * @return see ::eARNETWORKAL_ERROR
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_GetRecvBufferSize(ARNETWORKAL_Manager_t *manager, int *bufferSize);

/**
 * @brief Enables dump of data pushed/sent/received/popped.
 * @param manager pointer on the Manager
 * @param logDir directory where to save data dump. It musts exists otherwize dump will not be enabled
 * @param name name to give to the log. A rotation (up to 4 files) will be done if name already exists.
 * @return see ::eARNETWORKAL_ERROR
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_EnableDataDump(ARNETWORKAL_Manager_t *manager, const char *logDir, const char *name);

/**
 * @brief Adds a data to the dumpfile.
 * This is a no-op if ARNETWORKAL_Manager_EnableDataDump was not called (or raised an error).
 * @param manager pointer on the Manager.
 * @param tag 1-byte identifier of data (values under 0x20 are reserved).
 * @param data data buffer.
 * @param size size of the data.
 * @param sizeDump size of the data to actually dump. 0 to dump everything.
 * @param ts timestamp of data. NULL to use current time
 * @return see ::eARNETWORKAL_ERROR
 */
eARNETWORKAL_ERROR ARNETWORKAL_Manager_DumpData(ARNETWORKAL_Manager_t *manager, uint8_t tag, const void *data, size_t size, size_t sizeDump, const struct timespec *ts);

#endif /** _ARNETWORKAL_MANAGER_H_ */
