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
 * @file ARDATATRANSFER_Uploader.h
 * @brief libARDataTransfer Uploader header file.
 * @date 21/05/2014
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARDATATRANSFER_UPLOADER_H_
#define _ARDATATRANSFER_UPLOADER_H_

#include <libARUtils/ARUTILS_Manager.h>

/**
 * @brief Ftp Resume enum
 * @see ARDATATRANSFER_Uploader_New
 */
typedef enum
{
    ARDATATRANSFER_UPLOADER_RESUME_FALSE = 0,
    ARDATATRANSFER_UPLOADER_RESUME_TRUE,
    
} eARDATATRANSFER_UPLOADER_RESUME;

/**
 * @brief Progress callback of the Uploader
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the media file already downloaded
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
typedef void (*ARDATATRANSFER_Uploader_ProgressCallback_t) (void* arg, float percent);

/**
 * @brief Completion callback of the Uploader
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the media file already downloaded
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
typedef void (*ARDATATRANSFER_Uploader_CompletionCallback_t) (void* arg, eARDATATRANSFER_ERROR error);

/**
 * @brief Create a new ARDataTransfer Uploader
 * @warning This function allocates memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @param ftpManager The Ftp Manager
 * @param remotePath The remote path server to upload
 * @param localPath The local path system to upload
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_Uploader_Delete ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_Uploader_New (ARDATATRANSFER_Manager_t *manager, ARUTILS_Manager_t *ftpManager, const char *remotePath, const char *localPath, ARDATATRANSFER_Uploader_ProgressCallback_t progressCallback, void *progressArg, ARDATATRANSFER_Uploader_CompletionCallback_t completionCallback, void *completionArg, eARDATATRANSFER_UPLOADER_RESUME resume);

/**
 * @brief Delete an ARDataTransfer Uploader
 * @warning This function frees memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_Uploader_New ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_Uploader_Delete (ARDATATRANSFER_Manager_t *manager);

/**
 * @brief Rename a Remote File
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @param oldNamePath The old remote name path
 * @param newNamePath The new remote name path
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_Uploader_New ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_Uploader_Rename (ARDATATRANSFER_Manager_t *manager, const char *oldNamePath, const char *newNamePath);

/**
 * @brief Process of the device upload
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval returns NULL
 * @see ARDATATRANSFER_Uploader_New ()
 */
void* ARDATATRANSFER_Uploader_ThreadRun (void *managerArg);

/**
 * @brief Send a cancel to the device data downloader thread
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_Uploader_ThreadRun ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_Uploader_CancelThread (ARDATATRANSFER_Manager_t *manager);

#endif /* _ARDATATRANSFER_UPLOADER_H_ */

