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
 * @file ARUTILS_Http.h
 * @brief libARUtils Http header file.
 * @date 26/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_HTTP_H_
#define _ARUTILS_HTTP_H_

#include <inttypes.h>
#include <libARSAL/ARSAL_Sem.h>

#include "libARUtils/ARUTILS_Error.h"

/**
 * @brief Htpp max file name string size
 */
#define ARUTILS_HTTP_MAX_NAME_SIZE     64

/**
 * @brief Htpp max file name path string size
 */
#define ARUTILS_HTTP_MAX_PATH_SIZE     256

/**
 * @brief Http max url string size
 */
#define ARUTILS_HTTP_MAX_URL_SIZE      512

/**
 * @brief Http secured enum
 * @see ARUTILS_Http_Connection_New
 */
typedef enum
{
    HTTPS_PROTOCOL_FALSE = 0,
    HTTPS_PROTOCOL_TRUE,

} eARUTILS_HTTPS_PROTOCOL;

/**
 * @brief Http Connection structure
 * @see ARUTILS_Http_Connection_New ()
 */
typedef struct ARUTILS_Http_Connection_t ARUTILS_Http_Connection_t;

/**
 * @brief Http File structure
 * @param name The name of the file
 * @param path The the path of the file
 * @see ARUTILS_Http_Post_WithFiles ()
 */
typedef struct _ARUTILS_Http_File_t
{
    char name[ARUTILS_HTTP_MAX_NAME_SIZE];
    char path[ARUTILS_HTTP_MAX_PATH_SIZE];
    
} ARUTILS_Http_File_t;

/**
 * @brief Progress callback of the Http download
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the media file already downloaded
 * @see ARUTILS_Http_Get ()
 */
typedef void (*ARUTILS_Http_ProgressCallback_t)(void* arg, float percent);

/**
 * @brief Create a new Http Connection
 * @warning This function allocates memory
 * @param cancelSem The pointer of the Http get/post cancel semaphore or null
 * @param server The Http server IP address
 * @param port The Http server port
 * @param security The security flag: to indicate HTTP or HTTPS connection
 * @param username The Http server account name
 * @param password The Http server account password
 * @param[out] error The pointer of the error code: if success ARUTILS_OK, otherwise an error number of eARUTILS_ERROR
 * @retval On success, returns an ARUTILS_Http_Connection_t. Otherwise, it returns null.
 * @see ARUTILS_Http_DeleteConnection ()
 */
ARUTILS_Http_Connection_t * ARUTILS_Http_Connection_New(ARSAL_Sem_t *cancelSem, const char *server, int port, eARUTILS_HTTPS_PROTOCOL security, const char *username, const char* password, eARUTILS_ERROR *error);

/**
 * @brief Delete an Http Connection
 * @warning This function frees memory
 * @param connection The address of the pointer on the Http Connection
 * @see ARUTILS_Http_Connection_New ()
 */
void ARUTILS_Http_Connection_Delete(ARUTILS_Http_Connection_t **connection);

/**
 * @brief Cancel an Http Connection command in progress (get, post, etc)
 * @param connection The address of the pointer on the Http Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New ()
 */
eARUTILS_ERROR ARUTILS_Http_Connection_Cancel(ARUTILS_Http_Connection_t *connection);

/**
 * @brief Check if the connection has received a cancel to it's semaphore
 * @param connection The address of the pointer on the Http Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Http_IsCanceled(ARUTILS_Http_Connection_t *connection);

/**
 * @brief Set a Https Connection Server Certificate
 * @param connection The address of the pointer on the Http Connection
 * @param certPath The certificate file path
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New ()
 */
eARUTILS_ERROR ARUTILS_Http_SetSeverCertificate(ARUTILS_Http_Connection_t *connection, const char* certPath);

/**
 * @brief Get an remote Http server file
 * @param connection The address of the pointer on the Http Connection
 * @param namePath The string of the file name path on the remote Http server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New (), ARUTILS_Http_ProgressCallback_t
 */
eARUTILS_ERROR ARUTILS_Http_Get(ARUTILS_Http_Connection_t *connection, const char *namePath, const char *dstFile, ARUTILS_Http_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Http server file
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Http Connection
 * @param namePath The string of the file name path on the remote Http server
 * @param[out] data Returns byte buffer data address if data mode else give null pointer
 * @param[out] dataLen Returns byte buffer data length else give null pointer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New (), ARUTILS_Http_ProgressCallback_t
 */
eARUTILS_ERROR ARUTILS_Http_Get_WithBuffer(ARUTILS_Http_Connection_t *connection, const char *namePath, uint8_t **data, uint32_t *dataLen, ARUTILS_Http_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Post a file to a remote Http server
 * @param connection The address of the pointer on the Http Connection
 * @param namePath The string of the file name path on the remote Http server
 * @param srcFile The string of the local file name path to be post
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New (), ARUTILS_Http_ProgressCallback_t
 */
eARUTILS_ERROR ARUTILS_Http_Post_WithRange(ARUTILS_Http_Connection_t *connection, const char *namePath, const char *srcFile, const char *md5Txt, uint32_t startRange, uint32_t endRange, uint8_t **outData, uint32_t *outDataLen, ARUTILS_Http_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Post a file to a remote Http server
 * @param connection The address of the pointer on the Http Connection
 * @param namePath The string of the file name path on the remote Http server
 * @param fileList The address of the file list
 * @param fileListCount The file count in the file list
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Http_Connection_New (), ARUTILS_Http_ProgressCallback_t
 */
eARUTILS_ERROR ARUTILS_Http_Post_WithFiles(ARUTILS_Http_Connection_t *connection, const char *namePath, ARUTILS_Http_File_t *fileList, int fileListCount, ARUTILS_Http_ProgressCallback_t progressCallback, void* progressArg);

#endif /* _ARUTILS_HTTP_H_ */
