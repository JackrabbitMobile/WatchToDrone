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
 * @file ARUTILS_Manager.h
 * @brief libARUtils Manager header file.
 * @date 21/05/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_MANAGER_H_
#define _ARUTILS_MANAGER_H_


#include "libARUtils/ARUTILS_Ftp.h"


/**
 * @brief Ftp MANAGER structure
 * @see ARUTILS_Manager_New
 */
typedef struct ARUTILS_Manager_t ARUTILS_Manager_t;

/**
 * @brief BLE Manager (used to send/read data)
 * @note This is an application-provided object, OS Dependant
 */
//typedef void* ARUTILS_BLEDeviceManager_t;

/**
 * @brief BLE Device reference for the @ref ARUTILS_BLEDevice_t
 * @note This is an application-provided object, OS Dependant
 */
typedef void* ARUTILS_BLEDevice_t;

typedef void* ARUTILS_BLEManager_t;

/**
 * @brief Create a new Ftp Manager
 * @warning This function allocates memory
 * @param[out] error The pointer of the error code: if success ARUTILS_OK, otherwise an error number of eARUTILS_ERROR
 * @retval On success, returns an ARUTILS_Ftp_Connection_t. Otherwise, it returns null.
 * @see ARUTILS_Manager_Delete ()
 */
ARUTILS_Manager_t* ARUTILS_Manager_New(eARUTILS_ERROR *error);

/**
 * @brief Delete an Ftp Manager
 * @warning This function frees memory
 * @param managerAddr The address of the pointer on the Ftp Manager
 * @see ARUTILS_Manager_New ()
 */
void ARUTILS_Manager_Delete(ARUTILS_Manager_t **managerAddr);


/**
 * @brief Create a new WifiFtp Connection
 * @warning This function allocates memory
 * @param manager The Manager
 * @param cancelSem The pointer of the Ftp get/put cancel semaphore or null
 * @param server The Ftp server IP address
 * @param port The Ftp server port
 * @param username The Ftp server account name
 * @param password The Ftp server account password
 * @retval On success, returns an ARUTILS_OK. Otherwise an error number of eARUTILS_ERROR
 * @see ARUTILS_Manager_CloseWifiFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_InitWifiFtp(ARUTILS_Manager_t *manager, const char *server, int port, const char *username, const char* password);

/**
 * @brief Delete an WifiFtp Connection
 * @warning This function frees memory
 * @param manager The address of the pointer on the Ftp Connection
 * @see ARUTILS_Manager_InitWifiFtp ()
 */
void ARUTILS_Manager_CloseWifiFtp(ARUTILS_Manager_t *manager);

/**
 * @brief Create a new BLEFtp Connection
 * @warning This function allocates memory
 * @param manager The Manager
 * @param cancelSem The pointer of the Ftp get/put cancel semaphore or null
 * @param server The Ftp server IP address
 * @param port The Ftp server port
 * @param username The Ftp server account name
 * @param password The Ftp server account password
 * @retval On success, returns an ARUTILS_OK. Otherwise an error number of eARUTILS_ERROR
 * @see ARUTILS_Manager_CloseBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_InitBLEFtp(ARUTILS_Manager_t *manager, ARUTILS_BLEDevice_t device, int port);

/**
 * @brief Delete an WifiFtp Connection
 * @warning This function frees memory
 * @param manager The address of the pointer on the Ftp Connection
 * @see ARUTILS_Manager_InitBLEFtp ()
 */
void ARUTILS_Manager_CloseBLEFtp(ARUTILS_Manager_t *manager);

/**
 * @brief Create a new RFComm FTP Connection
 * @warning This function allocates memory
 * @param manager The Manager
 * @retval On success, returns an ARUTILS_OK. Otherwise an error number of eARUTILS_ERROR
 * @see ARUTILS_Manager_CloseRFCommFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_InitRFCommFtp(ARUTILS_Manager_t *manager);

/**
 * @brief Delete an RFComm Ftp Connection
 * @warning This function frees memory
 * @param manager The address of the pointer on the Ftp Connection
 * @see ARUTILS_Manager_InitWifiFtp ()
 */
void ARUTILS_Manager_CloseRFCommFtp(ARUTILS_Manager_t *manager);


/**
 * @brief Disconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_Disconnect(ARUTILS_Manager_t *manager);

/**
 * @brief Reconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_Reconnect(ARUTILS_Manager_t *manager);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Manager_NewBLEFtp
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_Cancel(ARUTILS_Manager_t *manager);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Manager_NewBLEFtp
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_IsCanceled(ARUTILS_Manager_t *manager);

/**
 * @brief Reset an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Manager_NewBLEFtp
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_Reset(ARUTILS_Manager_t *manager);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_List(ARUTILS_Manager_t *manager, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Execute Ftp Size command to retrieve file size
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the directory path on the remote Ftp server
 * @param fileSize The returned file size
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Size(ARUTILS_Manager_t *manager, const char *namePath, double *fileSize);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Get_WithBuffer(ARUTILS_Manager_t *manager, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Get(ARUTILS_Manager_t *manager, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Put an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Put(ARUTILS_Manager_t *manager, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Delete an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Delete(ARUTILS_Manager_t *manager, const char *namePath);

/**
 * @brief Delete an remote Ftp server directory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the directory name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_RemoveDir(ARUTILS_Manager_t *manager, const char *namePath);

/**
 * @brief Rename an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param oldNamePath The string of the old file name path on the remote Ftp server
 * @param newNamePath The string of the new file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_Manager_Ftp_Rename(ARUTILS_Manager_t *manager, const char *oldNamePath, const char *newNamePath);

/**
 * @brief Disconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_CloseWifiFtp ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Connection_Disconnect_t)(ARUTILS_Manager_t *manager);

/**
 * @brief Reconnect an remote Ftp server connection
 * @param connection The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_CloseWifiFtp ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Connection_Reconnect_t)(ARUTILS_Manager_t *manager);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param manager The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Connection_Cancel_t) (ARUTILS_Manager_t *manager);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param manager The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Connection_IsCanceled_t) (ARUTILS_Manager_t *manager);

/**
 * @brief Reset an Ftp Connection command in progress (get, put, list etc)
 * @param manager The address of the pointer on the Ftp Manager
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Connection_Reset_t) (ARUTILS_Manager_t *manager);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_List_t) (ARUTILS_Manager_t *manager, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Execute Ftp Size command to retrieve file size
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the directory path on the remote Ftp server
 * @param fileSize The returned file size
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_InitWifiFtp (), ARUTILS_Manager_InitBLEFtp ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Size_t)(ARUTILS_Manager_t *manager, const char *namePath, double *fileSize);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Get_WithBuffer_t) (ARUTILS_Manager_t *manager, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Get_t) (ARUTILS_Manager_t *manager, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Put an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Put_t) (ARUTILS_Manager_t *manager, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Delete an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Delete_t) (ARUTILS_Manager_t *manager, const char *namePath);

/**
 * @brief Delete an remote Ftp server directory
 * @param manager The address of the pointer on the Ftp Manager
 * @param namePath The string of the file directory path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_New ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_RemoveDir_t) (ARUTILS_Manager_t *manager, const char *namePath);

/**
 * @brief Rename an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Manager
 * @param oldNamePath The string of the old file name path on the remote Ftp server
 * @param newNamePath The string of the new file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_WifiFtp_NewConnection ()
 */
typedef eARUTILS_ERROR (*ARUTILS_Manager_Ftp_Rename_t)(ARUTILS_Manager_t *manager, const char *oldNamePath, const char *newNamePath);

/**
 * @brief Ftp Manager structure
 * @param ftpConnectionCancel The ARUTILS_Manager_Ftp_Connection_Cancel
 * @param ftpList The ARUTILS_Manager_Ftp_List
 * @param ftpGetWithBuffer The ARUTILS_Manager_Get_WithBuffer
 * @param ftpGet The ARUTILS_Manager_Ftp_Get
 * @param ftpPut The ARUTILS_Manager_Ftp_Put
 * @param ftpDelete The ARUTILS_Manager_Ftp_Delete
 * @see ARUTILS_Manager_New ()
 */
struct ARUTILS_Manager_t
{
    ARUTILS_Manager_Ftp_Connection_Disconnect_t ftpConnectionDisconnect;
    ARUTILS_Manager_Ftp_Connection_Reconnect_t ftpConnectionReconnect;
    ARUTILS_Manager_Ftp_Connection_Cancel_t ftpConnectionCancel;
    ARUTILS_Manager_Ftp_Connection_IsCanceled_t ftpConnectionIsCanceled;
    ARUTILS_Manager_Ftp_Connection_Reset_t ftpConnectionReset;
    ARUTILS_Manager_Ftp_List_t ftpList;
    ARUTILS_Manager_Ftp_Size_t ftpSize;
    ARUTILS_Manager_Get_WithBuffer_t ftpGetWithBuffer;
    ARUTILS_Manager_Ftp_Get_t ftpGet;
    ARUTILS_Manager_Ftp_Put_t ftpPut;
    ARUTILS_Manager_Ftp_Delete_t ftpDelete;
    ARUTILS_Manager_Ftp_Delete_t ftpRemoveDir;
    ARUTILS_Manager_Ftp_Rename_t ftpRename;

    ARSAL_Sem_t cancelSem;
    void *connectionObject;
};

#endif /* _ARUTILS_MANAGER_H_ */

