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
 * @file ARDISCOVERY_Device.h
 * @brief Discovery Device contains the informations of a device discovered
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARDISCOVERY_DEVICE_H_
#define _ARDISCOVERY_DEVICE_H_

#include <json/json.h>
#include <libARNetworkAL/ARNETWORKAL_Manager.h>
#include <libARNetworkAL/ARNETWORKAL_Error.h>
#include <libARNetwork/ARNETWORK_IOBufferParam.h>
#include <libARDiscovery/ARDISCOVERY_Error.h>
#include <libARDiscovery/ARDISCOVERY_Connection.h>
#include <libARDiscovery/ARDISCOVERY_Discovery.h>
#include <libARDiscovery/ARDISCOVERY_NetworkConfiguration.h>

//TODO add!!!
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_Device_ConnectionJsonCallback_t) (json_object *jsonObj, void *customData);

/**
 * @brief DiscoveryDevice contains the informations of a device discovered
 */
typedef struct ARDISCOVERY_Device_t ARDISCOVERY_Device_t;

/**
 * @brief Callback to create a new ARNetworkAL
 */
typedef ARNETWORKAL_Manager_t *(*ARDISCOVERY_DEVICE_NewARNetworkAL_t) (ARDISCOVERY_Device_t *device, eARDISCOVERY_ERROR *error, eARNETWORKAL_ERROR *errorAL);

/**
 * @brief Callback to delete the ARNetworkAL
 */
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_DEVICE_DeleteARNetworkAL_t) (ARDISCOVERY_Device_t *device, ARNETWORKAL_Manager_t **networkAL);

/**
 * @brief Callback to Initialize the NetworkConfiguration to use with the device
 */
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_DEVICE_InitNetworkConfiguration_t) (ARDISCOVERY_Device_t *device, ARDISCOVERY_NetworkConfiguration_t *networkConfiguration);

/**
 * @brief Callback to copy the specificParameters 
 */
typedef void *(*ARDISCOVERY_DEVICE_GetCopyOfSpecificParameters_t) (ARDISCOVERY_Device_t *device, eARDISCOVERY_ERROR *error);

/**
 * @brief Callback to delecte the specificParameters 
 */
typedef eARDISCOVERY_ERROR (*ARDISCOVERY_DEVICE_DelecteSpecificParameters_t) (ARDISCOVERY_Device_t *device);

/**
 * @brief DiscoveryDevice contains the informations of a device discovered
 */
struct ARDISCOVERY_Device_t
{
    char *name;
    eARDISCOVERY_PRODUCT productID;
    ARDISCOVERY_DEVICE_NewARNetworkAL_t newNetworkAL;
    ARDISCOVERY_DEVICE_DeleteARNetworkAL_t deleteNetworkAL;
    ARDISCOVERY_DEVICE_InitNetworkConfiguration_t initNetworkConfiguration;
    void *specificParameters; /**< Parameters specific, allocated by initializing functions and dealloc by the canceling functions. */
    ARDISCOVERY_DEVICE_GetCopyOfSpecificParameters_t getCopyOfSpecificParameters;
    ARDISCOVERY_DEVICE_DelecteSpecificParameters_t deleteSpecificParameters;
};


/**
 * @brief Create a new Discovery Device
 * @warning This function allocate memory
 * @post ARDISCOVERY_Device_Delete() must be called to delete the Discovery Device and free the memory allocated.
 * @param[out] error executing error.
 * @return the new Discovery Device
 * @see ARCONTROLLER_Device_Delete
 */
ARDISCOVERY_Device_t *ARDISCOVERY_Device_New (eARDISCOVERY_ERROR *error);

/**
 * @brief Copy a Discovery Device
 * @warning This function allocate memory
 * @post ARDISCOVERY_Device_Delete() must be called to delete the Discovery Device copied and free the memory allocated.
 * @param[in] deviceToCopy the device to copy.
 * @param[out] error executing error.
 * @return the new Discovery Device
 * @see ARCONTROLLER_Device_Delete
 */
ARDISCOVERY_Device_t *ARDISCOVERY_Device_NewByCopy (ARDISCOVERY_Device_t *deviceToCopy, eARDISCOVERY_ERROR *error);

/**
 * @brief Delete the Discovery Device
 * @warning This function free memory
 * @param device The Discovery Device to delete
 * @see ARDISCOVERY_Device_New
 */
void ARDISCOVERY_Device_Delete (ARDISCOVERY_Device_t **device);

/**
 * @brief Create a new ARNetworkAL  //TODO add commentary !!!!!!!!!!!!
 */
ARNETWORKAL_Manager_t *ARDISCOVERY_Device_NewARNetworkAL (ARDISCOVERY_Device_t *discoveryDevice, eARDISCOVERY_ERROR *error, eARNETWORKAL_ERROR *errorAL);

/**
 * @brief Delete a ARNetworkAL //TODO add commentary !!!!!!!!!!!!
 */
eARDISCOVERY_ERROR ARDISCOVERY_Device_DeleteARNetworkAL (ARDISCOVERY_Device_t *discoveryDevice, ARNETWORKAL_Manager_t **networkALManager);

/**
 * @brief Initialize the NetworkConfiguration to use with the device  //TODO add commentary !!!!!!!!!!!!
 */
eARDISCOVERY_ERROR ARDISCOVERY_Device_InitNetworkConfiguration (ARDISCOVERY_Device_t *discoveryDevice, ARDISCOVERY_NetworkConfiguration_t *networkConfiguration);

/***********************
 * -- Wifi part --
 ***********************/

/**
 * @brief Initialize the Discovery Device with a wifi device.
 * @param device The Discovery Device to Initialize.
 * @param[in] product Parrot's product to initialized
 * @param[in] name Device Name ; must be Null-terminated.
 * @param[in] address Device Address ; must be Null-terminated.
 * @param[in] port Device Port. 
 * @return executing error.
 */
eARDISCOVERY_ERROR ARDISCOVERY_Device_InitWifi (ARDISCOVERY_Device_t *device, eARDISCOVERY_PRODUCT product, const char *name, const char *address, int port);

/**
 * @brief Add connection callbacks to a wifi device.
 * @param device The Discovery Device to add callback.
 * @param[in] sendJsonCallback Callback to add a json part durring the connection. 
 * @param[in] receiveJsonCallback Callback to read a json part durring the connection.
 * @param[in] customData custom data given as parameter to the callbacks.
 * @return executing error.
 */
eARDISCOVERY_ERROR ARDISCOVERY_Device_WifiAddConnectionCallbacks (ARDISCOVERY_Device_t *device, ARDISCOVERY_Device_ConnectionJsonCallback_t sendJsonCallback, ARDISCOVERY_Device_ConnectionJsonCallback_t receiveJsonCallback, void *customData);

/**
 * @brief Get wifi IP address
 * @param device The Discovery Device.
 * @param[out] ipAddress ip address of the device
 * @param[in] length maximum length of the ipAddress output.
 * @return executing error.
 */
eARDISCOVERY_ERROR ARDISCOVERY_DEVICE_WifiGetIpAddress (ARDISCOVERY_Device_t *device, char *ipAddress, int length);

/***********************
 * -- BLE part --
 ***********************/

/**
 * @brief Initialize the Discovery Device with a BLE device.
 * @param device The Discovery Device to Initialize.
 * @param[in] product Parrot's product to initialized
 * @param[in] deviceManager the OS device manager which will be used for network functions
 * @param[in] device the selected OS specific BLE device to connect to
 * @return executing error.
 */
eARDISCOVERY_ERROR ARDISCOVERY_Device_InitBLE (ARDISCOVERY_Device_t *device, eARDISCOVERY_PRODUCT product, ARNETWORKAL_BLEDeviceManager_t bleDeviceManager, ARNETWORKAL_BLEDevice_t bleDevice);

#endif // _ARDISCOVERY_DEVICE_H_
