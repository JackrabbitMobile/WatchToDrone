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
 * @file ARUPDATER_Downloader.h
 * @brief libARUpdater Downloader header file.
 * @date 23/05/2014
 * @author djavan.bertrand@parrot.com
 **/

#ifndef _ARUPDATER_DOWNLOADER_H_
#define _ARUPDATER_DOWNLOADER_H_

#include <libARUpdater/ARUPDATER_Manager.h>
#include <libARSAL/ARSAL_MD5_Manager.h>
#include <libARDiscovery/ARDISCOVERY_Discovery.h>

typedef enum
{
    ARUPDATER_DOWNLOADER_ANDROID_PLATFORM,
    ARUPDATER_DOWNLOADER_IOS_PLATFORM,

}eARUPDATER_Downloader_Platforms;

typedef struct ARUPDATER_DownloadInformation_t
{
    char *downloadUrl;
    char *md5Expected;
    char *plfVersion;
    int remoteSize;
    eARDISCOVERY_PRODUCT product;
    
}ARUPDATER_DownloadInformation_t;

typedef struct ARUPDATER_Manager_BlacklistedFirmware_t
{
    eARDISCOVERY_PRODUCT product;
    char **versions;
    int nbVersionAllocated;
    int nbVersionBlacklisted;
} ARUPDATER_Manager_BlacklistedFirmware_t;


typedef struct ARUPDATER_Downloader_t ARUPDATER_Downloader_t;

/**
 * @brief Whether the plf file should be updated or not
 * @param arg The pointer of the user custom argument
 * @param nbPlfToBeUploaded : number of plf which are out to date
 * @param error The error status to indicate the plf check status
 */
typedef void (*ARUPDATER_Downloader_ShouldDownloadPlfCallback_t) (void* arg, int nbPlfToBeUploaded, eARUPDATER_ERROR error);

/**
 * @brief Fired just before the uploading of a plf file
 * @param arg The pointer of the user custom argument
 * @param product : Description of the product targeted by the plf downloaded
 * @param remotePlfVersion : The version of the plf file that will be downloaded
 */
typedef void (*ARUPDATER_Downloader_WillDownloadPlfCallback_t) (void* arg, eARDISCOVERY_PRODUCT product, const char *const remotePlfVersion);

/**
 * @brief Progress callback of the plf download
 * @param arg The pointer of the user custom argument
 * @param percent The percent size of the plf file already downloaded
 * @see ARUPDATER_Manager_CheckLocaleVersionThreadRun ()
 */
typedef void (*ARUPDATER_Downloader_PlfDownloadProgressCallback_t) (void* arg, float percent);

/**
 * @brief Completion callback of the Media download
 * @param arg The pointer of the user custom argument
 * @param media The media to be downloaded
 * @param error The error status to indicate the plf downloaded status
 * @see ARUPDATER_Manager_CheckLocaleVersionThreadRun ()
 */
typedef void (*ARUPDATER_Downloader_PlfDownloadCompletionCallback_t) (void* arg, eARUPDATER_ERROR error);

/**
 * @brief Create an object to download all plf files
 * @warning this function allocates memory
 * @pre rootFolder should already exist
 * @post ARUPDATER_Downloader_Delete should be called
 * @param manager : pointer on the manager
 * @param[in] rootFolder : root folder
 * @param[in] md5Manager : md5 manager
 * @param[in] appPlatform : enum on the application platform
 * @param[in] appVersion : Version of the app (pattern : X.Y.Z where X, Y, Z are integers)
 * @param[in] shouldDownloadCallback : callback which tells if the plf will be downloaded or not
 * @param[in|out] downloadArg : arg given to the shouldDownloadCallback
 * @param[in] willDownloadPlfCallback : callback which is called just the before the download begin
 * @param[in|out] willDownloadArg : arg given to the willDownloadPlfCallback
 * @param[in] progressCallback : callback which tells the progress of the download
 * @param[in|out] progressArg : arg given to the progressCallback
 * @param[in] completionCallback : callback which tells when the download is completed
 * @param[in|out] completionArg : arg given to the completionCallback
 * @return ARUPDATER_OK if operation went well, a description of the error otherwise
 * @see ARUPDATER_Downloader_Delete()
 */
eARUPDATER_ERROR ARUPDATER_Downloader_New(ARUPDATER_Manager_t* manager, const char *const rootFolder, ARSAL_MD5_Manager_t *md5Manager, eARUPDATER_Downloader_Platforms appPlatform, const char* const appVersion, ARUPDATER_Downloader_ShouldDownloadPlfCallback_t shouldDownloadCallback, void *downloadArg, ARUPDATER_Downloader_WillDownloadPlfCallback_t willDownloadCallback, void *willDownloadArg, ARUPDATER_Downloader_PlfDownloadProgressCallback_t progressCallback, void *progressArg, ARUPDATER_Downloader_PlfDownloadCompletionCallback_t completionCallback, void *completionArg);

/**
 * @brief Delete the Downloader of the Manager
 * @warning This function frees memory
 * @param manager a pointer on the ARUpdater Manager
 * @see ARUPDATER_Downloader_New ()
 */
eARUPDATER_ERROR ARUPDATER_Downloader_Delete(ARUPDATER_Manager_t *manager);

/**
 * @brief Set products list to updates process
 * @param managerArg : thread data of type ARUPDATER_Manager_t*
 * @param productList : addresse of hte list of product enums
 * @param productCount : count of product enums given by the list
 * @return ARUPDATER_OK if operation went well, a description of the error otherwise. Casted into a void*
 */
eARUPDATER_ERROR ARUPDATER_Downloader_SetUpdatesProductList(ARUPDATER_Manager_t *manager, eARDISCOVERY_PRODUCT *productList, int productCount);


/**
 * @brief Check if updates are available asynchrounously
 * @post call ARUPDATER_Downloader_ShouldDownloadPlfCallback_t at the end of the execution
 * @param managerArg : thread data of type ARUPDATER_Manager_t*
 * @return ARUPDATER_OK if operation went well, a description of the error otherwise. Casted into a void*
 */
void* ARUPDATER_Downloader_CheckUpdatesAsync(void *managerArg);

/**
 * @brief Check if updates are available synchrounously
 * @param manager : pointer on the manager
 * @param[out] err : The error status. Can be null.
 * @return The number of plf file which need to be updated
 */
int ARUPDATER_Downloader_CheckUpdatesSync(ARUPDATER_Manager_t *manager, eARUPDATER_ERROR *err);

/**
 * @brief Get update information from server synchrounously
 * @param manager : pointer on the manager
 * @param[out] err : The error status. Can be null.
 * @return An array filled with updates info
 */
//ARUPDATER_DownloadInformation_t** ARUPDATER_Downloader_GetUpdatesInfoSync(ARUPDATER_Manager_t *manager, eARUPDATER_ERROR *err);
int ARUPDATER_Downloader_GetUpdatesInfoSync(ARUPDATER_Manager_t *manager, eARUPDATER_ERROR *err, ARUPDATER_DownloadInformation_t*** informations);

/**
 * @brief Download all plf if needed
 * @warning This function must be called in its own thread.
 * @post ARUPDATER_Downloader_CancelDownloadThread() must be called after.
 * @param managerArg : thread data of type ARUPDATER_Manager_t*
 * @return ARUPDATER_OK if operation went well, a description of the error otherwise
 * @see ARUPDATER_Downloader_CancelDownloadThread()
 */
void* ARUPDATER_Downloader_ThreadRun(void *managerArg);

/**
 * @brief cancel the update
 * @details Used to kill the thread calling ARUPDATER_Downloader_DownloadThreadRun().
 * @param manager : pointer on the manager
 * @see ARUPDATER_Downloader_DownloadThreadRun()
 */
eARUPDATER_ERROR ARUPDATER_Downloader_CancelThread(ARUPDATER_Manager_t *manager);

/**
 * @brief Get if the thread is still running
 * @param manager : pointer on the manager
 * @param[out] error : pointer on an error. This can be null
 * @return 1 if the download thread is running, 0 otherwise
 */
int ARUPDATER_Downloader_ThreadIsRunning(ARUPDATER_Manager_t* manager, eARUPDATER_ERROR *error);

/**
 * @brief get all blacklisted firmware versions
 * @param manager : pointer on the manager
 * @param[in] alsoCheckRemote : 1 if we should check on the internet for balcklisted version
 * @param[out] blacklistedFirmwares : the list of blacklisted versions
 * @return ARUPDATER_OK if operation went well, a description of the error otherwise
 */
eARUPDATER_ERROR ARUPDATER_Downloader_GetBlacklistedFirmwareVersionsSync(ARUPDATER_Manager_t* manager, int alsoCheckRemote, ARUPDATER_Manager_BlacklistedFirmware_t ***blacklistedFirmwares);
#endif
