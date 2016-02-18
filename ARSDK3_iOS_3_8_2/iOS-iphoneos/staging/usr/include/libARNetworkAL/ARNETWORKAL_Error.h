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
 * @file ARNETWORKAL_Error.h
 * @brief libARNetworkAL errors known.
 * @date 04/29/2013
 * @author frederic.dhaeyer@parrot.com
 */

#ifndef _ARNETWORKAL_ERROR_H_
#define _ARNETWORKAL_ERROR_H_

/**
 * @brief libARNetworkAL errors known.
 */
typedef enum
{
    ARNETWORKAL_OK = 0,                              /**< No error */
    ARNETWORKAL_ERROR = -1000,                       /**< ARNetworkAL Generic error */
    ARNETWORKAL_ERROR_ALLOC,                         /**< Memory allocation error */
    ARNETWORKAL_ERROR_BAD_PARAMETER,                 /**< Bad parameters */
    ARNETWORKAL_ERROR_FIFO_INIT,                     /**< Fifo creation error (details set in errno) */
    ARNETWORKAL_ERROR_MAIN_THREAD,                   /**< The function cannot be run in main thread */

    ARNETWORKAL_ERROR_MANAGER = -2000,               /**< Manager generic error */
    ARNETWORKAL_ERROR_MANAGER_OPERATION_NOT_SUPPORTED, /**< The current manager does not support this operation */

    ARNETWORKAL_ERROR_NETWORK = -3000,               /**< Network generic error */
    ARNETWORKAL_ERROR_NETWORK_TYPE,                  /**< Network type, not available for the platform error */

    ARNETWORKAL_ERROR_WIFI = -4000,                  /**< Wifi generic error */
    ARNETWORKAL_ERROR_WIFI_SOCKET_CREATION,          /**< Wifi socket error during creation */
    ARNETWORKAL_ERROR_WIFI_SOCKET_PERMISSION_DENIED, /**< Wifi socket permission denied */
    ARNETWORKAL_ERROR_WIFI_SOCKET_GETOPT,            /**< wifi socket error on getopt */
    ARNETWORKAL_ERROR_WIFI_SOCKET_SETOPT,            /**< wifi socket error on setopt */

    ARNETWORKAL_ERROR_BLE_CONNECTION = -5000,        /**< BLE connection generic error */
    ARNETWORKAL_ERROR_BLE_NOT_CONNECTED,             /**< BLE is not connected */
    ARNETWORKAL_ERROR_BLE_DISCONNECTION,             /**< BLE disconnection error */
    ARNETWORKAL_ERROR_BLE_SERVICES_DISCOVERING,      /**< BLE network services discovering error */
    ARNETWORKAL_ERROR_BLE_CHARACTERISTICS_DISCOVERING,  /**< BLE network characteristics discovering error */
    ARNETWORKAL_ERROR_BLE_CHARACTERISTIC_CONFIGURING,   /**< BLE network characteristic configuring error */
    ARNETWORKAL_ERROR_BLE_STACK,                        /**< BLE stack generic error */

} eARNETWORKAL_ERROR;

/**
 * @brief Gets the error string associated with an eARNETWORKAL_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARNETWORKAL_Error_ToString (eARNETWORKAL_ERROR error);

#endif /* _ARNETWORKAL_ERROR_H_ */
