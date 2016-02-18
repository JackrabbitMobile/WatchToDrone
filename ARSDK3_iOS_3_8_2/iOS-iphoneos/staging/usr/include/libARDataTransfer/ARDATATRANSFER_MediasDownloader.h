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
 * @file ARDATATRANSFER_MediasDownloader.h
 * @brief libARDataTransfer MediasDownloader header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#include <libARDiscovery/ARDISCOVERY_Discovery.h>
#include <libARUtils/ARUtils.h>

#ifndef _ARDATATRANSFER_MEDIAS_DOWNLOADER_H_
#define _ARDATATRANSFER_MEDIAS_DOWNLOADER_H_

/**
 * @brief Defines the media name size in characters
 * @see ARDATATRANSFER_Media_t
 */
#define ARDATATRANSFER_MEDIA_NAME_SIZE  128

/**
 * @brief Defines the media path size in characters
 * @see ARDATATRANSFER_Media_t
 */
#define ARDATATRANSFER_MEDIA_PATH_SIZE  256

/**
 * @brief Defines the media date size in characters
 * @see ARDATATRANSFER_Media_t
 */
#define ARDATATRANSFER_MEDIA_DATE_SIZE  32

/**
 * @brief Defines the media UUID size in characters
 * @see ARDATATRANSFER_Media_t
 */
#define ARDATATRANSFER_MEDIA_UUID_SIZE  33

/**
 * @brief Media structure
 * @param product The the product that the media belong to
 * @param name The name of the media
 * @param fileName The file name of the media
 * @param date The date of the media
 * @param uuid The uuid of the media
 * @param size The size of the media
 * @param thumbnail The media thumbnail data
 * @param thumbnailSize The size of the media thumbnail
 * @see ARDATATRANSFER_MediaList_t
 */
typedef struct
{
    eARDISCOVERY_PRODUCT product;
    char name[ARDATATRANSFER_MEDIA_NAME_SIZE];
    char filePath[ARDATATRANSFER_MEDIA_PATH_SIZE];
    char date[ARDATATRANSFER_MEDIA_DATE_SIZE];
    char uuid[ARDATATRANSFER_MEDIA_UUID_SIZE];
    char remotePath[ARUTILS_FTP_MAX_PATH_SIZE];
    char remoteThumb[ARUTILS_FTP_MAX_PATH_SIZE];
    double size;
    uint8_t *thumbnail;
    uint32_t thumbnailSize;
    
} ARDATATRANSFER_Media_t;

/**
 * @brief Available media callback called for each media found
 * @param arg The pointer of the user custom argument
 * @param media The availble media found
 * @param index The index of the media in the internal list
 * @see ARDATATRANSFER_MediasDownloader_GetAvailableMedias ()
 */
typedef void (*ARDATATRANSFER_MediasDownloader_AvailableMediaCallback_t) (void* arg, ARDATATRANSFER_Media_t *media, int index);

/**
 * @brief Progress callback of the Media download
 * @param arg The pointer of the user custom argument
 * @param media The media in progress
 * @param percent The percent size of the media file already downloaded
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
typedef void (*ARDATATRANSFER_MediasDownloader_MediaDownloadProgressCallback_t) (void* arg, ARDATATRANSFER_Media_t *media, float percent);

/**
 * @brief Completion callback of the Media download
 * @param arg The pointer of the user custom argument
 * @param media The media to be downloaded
 * @param error The error status to indicate the media downloaded status
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
typedef void (*ARDATATRANSFER_MediasDownloader_MediaDownloadCompletionCallback_t) (void* arg, ARDATATRANSFER_Media_t *media, eARDATATRANSFER_ERROR error);

/**
 * @brief Completion callback of the Media delete
 * @param arg The pointer of the user custom argument
 * @param media The media to be deleted
 * @param error The error status to indicate the media deleted status
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
typedef void (*ARDATATRANSFER_MediasDownloader_DeleteMediaCallback_t) (void* arg, ARDATATRANSFER_Media_t *media, eARDATATRANSFER_ERROR error);

/**
 * @brief Create a new ARDataTransfer MediasDownloader
 * @warning This function allocates memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @param deviceIP The IP address of the Device
 * @param devicePort The FTP port of the Device
 * @param remoteDirectory The FTP sub directory where to download media
 * @param localDirectory The path of the local directory where to store medias
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_Delete ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_New (ARDATATRANSFER_Manager_t *manager, ARUTILS_Manager_t *ftpListManager, ARUTILS_Manager_t *ftpQueueManager, const char *remoteDirectory, const char *localDirectory);

/**
 * @brief Delete an ARDataTransfer MediasDownloader
 * @warning This function frees memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_New ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_Delete (ARDATATRANSFER_Manager_t *manager);

/**
 * @brief Get the medias count available form the Device
 * @warning This function allocates memory
 * @param manager The pointer of the ARDataTransfer Manager
 * @param [out] result The On success, set ARDATATRANSFER_OK. Otherwise, it set an error number of eARDATATRANSFER_ERROR
 * @param withThumbnail The flag to return thumbnail, 0 no thumbnail is returned, 1 thumbnails are returned
 * @retval On success, the number of media found else 0.
 * @see ARDATATRANSFER_MediasDownloader_New ()
 */
int ARDATATRANSFER_MediasDownloader_GetAvailableMediasSync (ARDATATRANSFER_Manager_t *manager, int withThumbnail, eARDATATRANSFER_ERROR *result);

/**
 * @brief Get the media form the the medias list at the given index
 * @warning This function allocates memory
 * @param manager The pointer of the ARDataTransfer Manager
 * @param [out] result The On success, set ARDATATRANSFER_OK. Otherwise, it set an error number of eARDATATRANSFER_ERROR
 * @retval On success, the adresse of the media found else null.
 * @see ARDATATRANSFER_MediasDownloader_GetAvailableMediasSync ()
 */
 ARDATATRANSFER_Media_t * ARDATATRANSFER_MediasDownloader_GetAvailableMediaAtIndex(ARDATATRANSFER_Manager_t *manager, int index, eARDATATRANSFER_ERROR *result);

/**
 * @brief Get the medias list available form the Device
 * @warning This function allocates memory
 * @param manager The pointer of the ARDataTransfer Manager
 * @param availableMediaCallback The available media callback
 * @param availableMediaArg The pointer of the user custom argument
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_AvailableMediaCallback_t ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_GetAvailableMediasAsync (ARDATATRANSFER_Manager_t *manager, ARDATATRANSFER_MediasDownloader_AvailableMediaCallback_t availableMediaCallback, void *availableMediaArg);

/**
 * @brief Send a cancel to the get media list function
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_GetAvailableMediasSync (), ARDATATRANSFER_MediasDownloader_GetAvailableMediasAsync ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_CancelGetAvailableMedias(ARDATATRANSFER_Manager_t *manager);

/**
 * @brief Send a reset to the get media list function
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_GetAvailableMediasSync (), ARDATATRANSFER_MediasDownloader_GetAvailableMediasAsync ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_ResetGetAvailableMedias(ARDATATRANSFER_Manager_t *manager);

/**
 * @brief Delete the media form the Device
 * @param manager The pointer of the ARDataTransfer Manager
 * @param media The media to delete
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_GetAvailableMedias ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_DeleteMedia(ARDATATRANSFER_Manager_t *manager, ARDATATRANSFER_Media_t *media, ARDATATRANSFER_MediasDownloader_DeleteMediaCallback_t deleteMediaCallBack, void *deleteMediaArg);

/**
 * @brief Add a media to the download process queue
 * @param manager The pointer of the ARDataTransfer Manager
 * @param media The media to add
 * @param progressCallback The progress callback for this media download
 * @param progressArg The progress callback user argument for this media download
 * @param completionCallback The completion callback for this media download
 * @param completionArg The completion callback user argument for this media download
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_ThreadRun ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_AddMediaToQueue (ARDATATRANSFER_Manager_t *manager, ARDATATRANSFER_Media_t *media,  ARDATATRANSFER_MediasDownloader_MediaDownloadProgressCallback_t progressCallback, void *progressArg, ARDATATRANSFER_MediasDownloader_MediaDownloadCompletionCallback_t completionCallback, void *completionArg);

/**
 * @brief Process of the media download queue
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval returns NULL
 * @see ARDATATRANSFER_MediasDownloader_Init ()
 */
void* ARDATATRANSFER_MediasDownloader_QueueThreadRun (void *managerArg);

/**
 * @brief Send a cancel to the medias downloader process queue
 * @param manager The pointer of the ARDataTransfer Manager
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see ARDATATRANSFER_MediasDownloader_QueueThreadRun ()
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_CancelQueueThread (ARDATATRANSFER_Manager_t *manager);

/**
 * @brief Get the media thumbnail from the device FTP server
 * @warning This function allocates memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @param media The media for which the thumbnail is requested
 * @retval On success, returns ARDATATRANSFER_OK. Otherwise, it returns an error number of eARDATATRANSFER_ERROR.
 * @see
 */
eARDATATRANSFER_ERROR ARDATATRANSFER_MediasDownloader_GetThumbnail(ARDATATRANSFER_Manager_t *manager, ARDATATRANSFER_Media_t *media);

#endif /* _ARDATATRANSFER_MEDIAS_DOWNLOADER_H_ */


