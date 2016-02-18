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
 * @file ARUpdater/ARUPDATER_Error.h
 * @brief libARUpdater error header, This file contains all error of ARUPDATER
 * @date 26/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _LIBARUPDATER_ERROR_H_
#define _LIBARUPDATER_ERROR_H_

/**
 * @brief libARUpdater errors known.
 */
typedef enum
{
    ARUPDATER_OK = 0,                                   /**< No error */
    ARUPDATER_ERROR = -1000,                            /**< Unknown generic error */
    ARUPDATER_ERROR_ALLOC,                              /**< Memory allocation error */
    ARUPDATER_ERROR_BAD_PARAMETER,                      /**< Bad parameters error */
    ARUPDATER_ERROR_SYSTEM,                             /**< System error */
    ARUPDATER_ERROR_THREAD_PROCESSING,                  /**< Thread processing error */
    
    ARUPDATER_ERROR_MANAGER = -2000,                    /**< Generic manager error */
    ARUPDATER_ERROR_MANAGER_ALREADY_INITIALIZED,        /**< The uploader or downloader is already initilized in the manager */
    ARUPDATER_ERROR_MANAGER_NOT_INITIALIZED,            /**< The uploader or downloader is not initialized in the manager */
    ARUPDATER_ERROR_MANAGER_BUFFER_TOO_SMALL,           /**< The given buffer is too small */
    
    ARUPDATER_ERROR_PLF = -3000,                        /**< Generic PLF error */
    ARUPDATER_ERROR_PLF_FILE_NOT_FOUND,                 /**< Plf File not found */
    
    ARUPDATER_ERROR_DOWNLOADER = -4000,                    /**< Generic Updater error */
    ARUPDATER_ERROR_DOWNLOADER_ARUTILS_ERROR,              /**< error on a ARUtils operation */
    ARUPDATER_ERROR_DOWNLOADER_DOWNLOAD,                   /**< error downloading a file */
    ARUPDATER_ERROR_DOWNLOADER_PLATFORM_ERROR,             /**< error on a platform name */
    ARUPDATER_ERROR_DOWNLOADER_PHP_APP_OUT_TO_DATE_ERROR,  /**< This app version is out to date */
    ARUPDATER_ERROR_DOWNLOADER_PHP_ERROR,                  /**< error given by the PHP script on server */
    ARUPDATER_ERROR_DOWNLOADER_RENAME_FILE,                /**< error when renaming files */
    ARUPDATER_ERROR_DOWNLOADER_FILE_NOT_FOUND,             /**< Plf file not found in the downloader */
    ARUPDATER_ERROR_DOWNLOADER_MD5_DONT_MATCH,             /**< MD5 checksum does not match with the remote file */
    
    ARUPDATER_ERROR_UPLOADER = -5000,                   /**< Generic Uploader error */
    ARUPDATER_ERROR_UPLOADER_ARUTILS_ERROR,             /**< error on a ARUtils operation in uploader*/
    ARUPDATER_ERROR_UPLOADER_ARDATATRANSFER_ERROR,      /**< error on a ARDataTransfer operation in uploader*/
    ARUPDATER_ERROR_UPLOADER_ARSAL_ERROR,               /**< error on a ARSAL operation in uploader*/
    
} eARUPDATER_ERROR;

/**
 * @brief Gets the error string associated with an eARUPDATER_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARUPDATER_Error_ToString (eARUPDATER_ERROR error);

#endif /* _LIBARUPDATER_ERROR_H_ */
