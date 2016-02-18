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
 * @file ARUTILS_Ftp.h
 * @brief libARUtils Ftp header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_FTP_H_
#define _ARUTILS_FTP_H_

#include <inttypes.h>
#include <libARSAL/ARSAL_Sem.h>
#include "libARUtils/ARUTILS_Error.h"

/**
 * @brief Ftp anonymous user name
 */
#define ARUTILS_FTP_ANONYMOUS        "anonymous"

/**
 * @brief Ftp max url string size
 */
#define ARUTILS_FTP_MAX_URL_SIZE      512

/**
 * @brief Ftp max file name path string size
 */
#define ARUTILS_FTP_MAX_PATH_SIZE     256

/**
 * @brief Ftp max list line string size
 */
#define ARUTILS_FTP_MAX_LIST_LINE_SIZE     512

/**
 * @brief Ftp Resume enum
 * @see ARUTILS_Ftp_Get
 */
typedef enum
{
    FTP_RESUME_FALSE = 0,
    FTP_RESUME_TRUE,

} eARUTILS_FTP_RESUME;

/**
 * @brief Progress callback of the Ftp download
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the media file already downloaded
 * @see ARUTILS_Ftp_Get ()
 */
typedef void (*ARUTILS_Ftp_ProgressCallback_t)(void* arg, float percent);

/**
 * @brief File list iterator function
 * @param list The file list
 * @param nextItem The the next file
 * @param prefix The file prefix to match
 * @param isDirectory The file type requested: 1 directory or 0 file
 * @param indexItem The beginning of the line item if address is not null
 * @param itemLen The length of the line item if address is not null
 * @param lineData The buffer that will receive the result
 * @param lineDataLen The size of the buffer that will receive the result
 * @retval On success, returns the file name of the found item, Otherwise, it returns null
 * @see ARUTILS_Ftp_List ()
 */
const char * ARUTILS_Ftp_List_GetNextItem(const char *list, const char **nextItem, const char *prefix, int isDirectory, const char **indexItem, int *itemLen, char *lineData, int lineDataLen);

/**
 * @brief File size accessor function from a file of a File list
 * @param line The line of the File list where to search file size
 * @param lineSize The size of the given line
 * @param size The addresse of the pointer to the file size to return
 * @retval On success, returns the addresse found size string, Otherwise, it returns null
 * @see ARUTILS_Ftp_List_GetNextItem ()
 */
const char * ARUTILS_Ftp_List_GetItemSize(const char *line, int lineSize, double *size);

/**
 * @brief Public Ftp Connection struct
 * @see ARUTILS_WifiFtp_Connection_New, ARUTILS_WifiFtp_DeleteConnection ()
 */
typedef struct ARUTILS_WifiFtp_Connection_t ARUTILS_WifiFtp_Connection_t;

/**
 * @brief Create a new Ftp Connection
 * @warning This function allocates memory
 * @param cancelSem The pointer of the Ftp get/put cancel semaphore or null
 * @param server The Ftp server IP address
 * @param port The Ftp server port
 * @param username The Ftp server account name
 * @param password The Ftp server account password
 * @param[out] error The pointer of the error code: if success ARUTILS_OK, otherwise an error number of eARUTILS_ERROR
 * @retval On success, returns an ARUTILS_WifiFtp_Connection_t. Otherwise, it returns null.
 * @see ARUTILS_WifiFtp_DeleteConnection ()
 */
ARUTILS_WifiFtp_Connection_t * ARUTILS_WifiFtp_Connection_New(ARSAL_Sem_t *cancelSem, const char *server, int port, const char *username, const char* password, eARUTILS_ERROR *error);

/**
 * @brief Delete an Ftp Connection
 * @warning This function frees memory
 * @param connection The address of the pointer on the Ftp Connection
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
void ARUTILS_WifiFtp_Connection_Delete(ARUTILS_WifiFtp_Connection_t **connection);

/**
 * @brief Disconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection (), ARUTILS_WifiFtp_Connection_Delete ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Connection_Disconnect(ARUTILS_WifiFtp_Connection_t *connection);

/**
 * @brief Reconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection (), ARUTILS_WifiFtp_Connection_Delete ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Connection_Reconnect(ARUTILS_WifiFtp_Connection_t *connection);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Connection_Cancel(ARUTILS_WifiFtp_Connection_t *connection);

/**
 * @brief Check if the connection has received a cancel to it's semaphore
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_WifiFtp_IsCanceled(ARUTILS_WifiFtp_Connection_t *connection);

/**
 * @brief Reset an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Connection_Reset(ARUTILS_WifiFtp_Connection_t *connection);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_List(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Rename an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param oldNamePath The string of the old file name path on the remote Ftp server
 * @param newNamePath The string of the new file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Rename(ARUTILS_WifiFtp_Connection_t *connection, const char *oldNamePath, const char *newNamePath);

/**
 * @brief Get the size of an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Size(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath, double *size);

/**
 * @brief Delete an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Delete(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath);

/**
 * @brief Delete an remote Ftp server directory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_WifiFtp_RemoveDir(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath);

/**
 * @brief Get an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Get(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Get_WithBuffer(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Put an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtp_Put(ARUTILS_WifiFtp_Connection_t *connection, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);


#endif /* _ARUTILS_FTP_H_ */
