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
 * @file ARCONTROLLER_Error.h
 * @brief libARController errors known.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_ERROR_H_
#define _ARCONTROLLER_ERROR_H_

/**
 * @brief libARController errors known.
 */
typedef enum
{
    ARCONTROLLER_OK = 0, /**< No error */
    ARCONTROLLER_ERROR = -1000, /**< Unknown generic error */
    
    ARCONTROLLER_ERROR_ALLOC, /**< Memory allocation error */
    ARCONTROLLER_ERROR_BAD_PARAMETER, /**< Bad parameters */
    ARCONTROLLER_ERROR_MUTEX, /**< Mutex lock or unlock error  */
    ARCONTROLLER_ERROR_INIT_MUTEX, /**< Mutex initialization error */
    ARCONTROLLER_ERROR_STATE, /**< Bad state of the Network Controller to call this function */
    ARCONTROLLER_ERROR_BUFFER_SIZE, /**< Buffer is too small */
    
    ARCONTROLLER_ERROR_INIT = -2000, /**< Error of initialization */
    ARCONTROLLER_ERROR_INIT_THREAD, /**< Thread initialization error */
    ARCONTROLLER_ERROR_INIT_ARNETWORKAL_MANAGER, /**< Error during the getting of the ARNETWORKAL_Manager from the device */
    ARCONTROLLER_ERROR_INIT_ARNETWORK_MANAGER, /**< Error Initialization of the ARNETWORK_Manager */
    ARCONTROLLER_ERROR_INIT_NETWORK_CONFIG, /**< Error during the getting of the ARNetWork Configuration from the device */
    ARCONTROLLER_ERROR_INIT_DEVICE_COPY, /**< Error during the copy of the device */
    ARCONTROLLER_ERROR_INIT_DEVICE_GET_NETWORK_CONFIG, /**< Error during the get of the networkConfiguration from the device */
    ARCONTROLLER_ERROR_INIT_DEVICE_JSON_CALLBACK, /**< Error during the add of json callback to the device */
    ARCONTROLLER_ERROR_INIT_GET_DATE, /**< Error during the get of the current date */
    ARCONTROLLER_ERROR_INIT_GET_TIME, /**< Error during the get of the current time */
    ARCONTROLLER_ERROR_INIT_STREAM, /**< Error during the initialization of a stream */
    ARCONTROLLER_ERROR_INIT_SEMAPHORE, /**< Error during the initialization of a semaphore */
    ARCONTROLLER_ERROR_NOT_SENT, /**< Error data not sent */
    ARCONTROLLER_ERROR_NO_VIDEO, /**< Error the device has no video */
    ARCONTROLLER_ERROR_NO_ELEMENT, /**< No element saved for this command */
    ARCONTROLLER_ERROR_NO_ARGUMENTS, /**< No argument saved for this command */
    ARCONTROLLER_ERROR_CANCELED, /**< start canceled*/
    ARCONTROLLER_ERROR_COMMAND_GENERATING, /**< Error of command generating */
    
    ARCONTROLLER_ERROR_COMMAND_CALLBACK = -3000, /**< Error of command generating */
    ARCONTROLLER_ERROR_COMMAND_CALLBACK_ALREADY_REGISTERED, /**< the command callback is already registered*/
    ARCONTROLLER_ERROR_COMMAND_CALLBACK_NOT_REGISTERED, /**< the command callback is not registred*/
    
    ARCONTROLLER_ERROR_STREAMPOOL = -4000, /**< Generic stream pool error */
    ARCONTROLLER_ERROR_STREAMPOOL_FRAME_NOT_FOUND, /**< no frame found */
    
    ARCONTROLLER_ERROR_STREAMQUEUE = -5000, /**< Generic stream queue error */
    ARCONTROLLER_ERROR_STREAMQUEUE_EMPTY, /**< Error stream queue empty*/
    
    ARCONTROLLER_ERROR_JNI = -6000, /**< Generic JNI error */
    ARCONTROLLER_ERROR_JNI_ENV, /**< Error of JNI environment */
    ARCONTROLLER_ERROR_JNI_INIT, /**< Native part not initialized */
    
    ARCONTROLLER_ERROR_EXTENSION = -7000, /**< Generic extension related error */
    ARCONTROLLER_ERROR_EXTENSION_PRODUCT_NOT_VALID, /**< Product not valid to be an extension  */
    
    ARCONTROLLER_ERROR_STREAM = -8000, /**< Generic stream error */
    ARCONTROLLER_ERROR_STREAM_RESYNC_REQUIRED, /**< Stream re-synchronization required  */

} eARCONTROLLER_ERROR;

/**
 * @brief Gets the error string associated with an eARCONTROLLER_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char *ARCONTROLLER_Error_ToString (eARCONTROLLER_ERROR error);

#endif /* _ARCONTROLLER_ERROR_H_ */
