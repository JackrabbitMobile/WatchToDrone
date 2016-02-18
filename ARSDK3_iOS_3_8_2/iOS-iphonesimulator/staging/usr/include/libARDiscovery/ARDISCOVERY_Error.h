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
#ifndef _ARDISCOVERY_ERROR_H_
#define _ARDISCOVERY_ERROR_H_

/**
 * @brief libARDiscovery errors known.
 */
typedef enum
{
// Do not change these values, they are sent by the device in the Json of connection.
    ARDISCOVERY_OK = 0, /**< No error */
    ARDISCOVERY_ERROR = -1, /**< Unknown generic error */
// End of values sent by the device in the Json of connection.

    ARDISCOVERY_ERROR_SIMPLE_POLL = -1000, /**< Avahi failed to create simple poll object */
    ARDISCOVERY_ERROR_BUILD_NAME, /**< Avahi failed to create simple poll object */
    ARDISCOVERY_ERROR_CLIENT, /**< Avahi failed to create client */
    ARDISCOVERY_ERROR_CREATE_CONFIG, /**< Failed to create config file */
    ARDISCOVERY_ERROR_DELETE_CONFIG, /**< Failed to delete config file */
    ARDISCOVERY_ERROR_ENTRY_GROUP, /**< Avahi failed to create entry group */
    ARDISCOVERY_ERROR_ADD_SERVICE, /**< Avahi failed to add service */
    ARDISCOVERY_ERROR_GROUP_COMMIT, /**< Avahi failed to commit group */
    ARDISCOVERY_ERROR_BROWSER_ALLOC, /**< Avahi failed to allocate desired number of browsers */
    ARDISCOVERY_ERROR_BROWSER_NEW, /**< Avahi failed to create one browser */

    ARDISCOVERY_ERROR_ALLOC = -2000, /**< Failed to allocate connection resources */
    ARDISCOVERY_ERROR_INIT, /**< Wrong type to connect as */
    ARDISCOVERY_ERROR_SOCKET_CREATION, /**< Socket creation error */
    ARDISCOVERY_ERROR_SOCKET_PERMISSION_DENIED, /**< Socket access permission denied */
    ARDISCOVERY_ERROR_SOCKET_ALREADY_CONNECTED, /**< Socket is already connected */
    ARDISCOVERY_ERROR_ACCEPT, /**< Socket accept failed */
    ARDISCOVERY_ERROR_SEND, /**< Failed to write frame to socket */
    ARDISCOVERY_ERROR_READ, /**< Failed to read frame from socket */
    ARDISCOVERY_ERROR_SELECT, /**< Failed to select sets */
    ARDISCOVERY_ERROR_TIMEOUT, /**< timeout error */
    ARDISCOVERY_ERROR_ABORT, /**< Aborted by the user*/
    ARDISCOVERY_ERROR_PIPE_INIT, /**< Failed to intitialize a pipe*/
    ARDISCOVERY_ERROR_BAD_PARAMETER, /**< Bad parameters */
    ARDISCOVERY_ERROR_BUSY, /**< discovery is busy*/
    ARDISCOVERY_ERROR_SOCKET_UNREACHABLE, /**< host or net is not reachable */
    ARDISCOVERY_ERROR_OUTPUT_LENGTH, /**< the length of the output is to small */
    
    ARDISCOVERY_ERROR_JNI = -3000, /**< JNI error */
    ARDISCOVERY_ERROR_JNI_VM, /**< JNI virtual machine, not initialized */
    ARDISCOVERY_ERROR_JNI_ENV, /**< null JNI environment  */
    ARDISCOVERY_ERROR_JNI_CALLBACK_LISTENER, /**<  null jni callback listener*/

// Do not change these values, they are sent by the device in the Json of connection.
    ARDISCOVERY_ERROR_CONNECTION = -4000, /**< Connection error */
    ARDISCOVERY_ERROR_CONNECTION_BUSY = -3999, /**< Product already connected */
    ARDISCOVERY_ERROR_CONNECTION_NOT_READY = -3998, /**< Product not ready to connect */
    ARDISCOVERY_ERROR_CONNECTION_BAD_ID = -3997, /**< It is not the good Product */
// End of values sent by the device in the Json of connection.

    ARDISCOVERY_ERROR_DEVICE = -5000,               /**< Device generic error */
    ARDISCOVERY_ERROR_DEVICE_OPERATION_NOT_SUPPORTED, /**< The current device does not support this operation */
    
    ARDISCOVERY_ERROR_JSON = -6000, /**< Json generic error */
    ARDISCOVERY_ERROR_JSON_PARSSING, /**< Json parssing error */
    ARDISCOVERY_ERROR_JSON_BUFFER_SIZE, /**< The size of the buffer storing the Json is too small */

} eARDISCOVERY_ERROR;

/**
 * @brief Gets the error string associated with an eARDISCOVERY_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARDISCOVERY_Error_ToString (eARDISCOVERY_ERROR error);

#endif /* _ARDISCOVERY_ERROR_H_ */
