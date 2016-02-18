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
 * @file ARUTILS_FileSystem.h
 * @brief libARUtils FileSystem header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/
 
#ifndef _ARUTILS_FILESYSTEM_H_
#define _ARUTILS_FILESYSTEM_H_

#include "libARUtils/ARUTILS_Error.h" 

/**
 * @brief The maximum directory depth for ftw/ARSAL_Ftw calls
 * @see ARSAL_Ftw ()
 */
#define ARUTILS_FILE_SYSTEM_MAX_FD_FOR_FTW    20

/**
 * @brief Check if local file system file or directory exist
 * @param namePath The string name path
 * @param[out] size The size of the file if success
 * @retval On success, returns ARUTILS_OK, ARUTILS_ERROR_FILE_NOT_FOUND if it not exist. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see
 */
eARUTILS_ERROR ARUTILS_FileSystem_IsExist(const char *namePath);

/**
 * @brief Get local file system file size in bytes
 * @param namePath The string name path
 * @param[out] size The size of the file if success
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see 
 */
eARUTILS_ERROR ARUTILS_FileSystem_GetFileSize(const char *namePath, int64_t *size);

/**
 * @brief Rename local file system file
 * @param oldName The old file name path string
 * @param newName The new file name path string
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see stat ()
 */
eARUTILS_ERROR ARUTILS_FileSystem_Rename(const char *oldName, const char *newName);

/**
 * @brief Remove local file system file
 * @param localPath The file name path string
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see remove
 */
eARUTILS_ERROR ARUTILS_FileSystem_RemoveFile(const char *localPath);

/**
 * @brief Remove recurcively a directory and its content
 * @param localPath The file name path string
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARSAL_Ftw (), rmdir
 */
eARUTILS_ERROR ARUTILS_FileSystem_RemoveDir(const char *localPath);

/**
 * @brief Get free space in bytes of the local file system directory
 * @param localPath The file name path string
 * @param[out] freeSpace The free space of the local directory
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see
 */
eARUTILS_ERROR ARUTILS_FileSystem_GetFreeSpace(const char *localPath, double *freeSpace);

#endif /* _ARUTILS_FILESYSTEM_H_ */
