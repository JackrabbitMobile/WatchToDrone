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
 * @file ARMAVLINK_Error.h
 * @brief libARMavlink errors known.
 * @date 11/13/2013
 * @author frederic.dhaeyer@parrot.com
 */

#ifndef _ARMAVLINK_ERROR_H_
#define _ARMAVLINK_ERROR_H_

/**
 * @brief libARMavlink errors known.
 */
typedef enum
{
    ARMAVLINK_OK = 0,                                            /**< No error */
    
    ARMAVLINK_ERROR = -1000,                                     /**< Unknown generic error */
    ARMAVLINK_ERROR_ALLOC,                                       /**< Memory allocation error */
    ARMAVLINK_ERROR_BAD_PARAMETER,                               /**< Bad parameters */
    
    ARMAVLINK_ERROR_MANAGER = -2000,                             /**< Unknown ARMAVLINK_Manager error */
    
    ARMAVLINK_ERROR_FILE_GENERATOR = -3000,                      /**< Unknown ARMAVLINK_FileGenerator error */
    
    ARMAVLINK_ERROR_LIST_UTILS = -4000,                          /**< Unknown ARMAVLINK_ListUtils error */
    
    ARMAVLINK_ERROR_MISSION_ITEM_UTILS = -5000,                  /**< Unknown ARMAVLINK_MissionItemUtils error */
    ARMAVLINK_ERROR_MISSION_ITEM_UTILS_NOT_LINKED_COMMAND,       /**< Command not linked with Mavlink commands */
    
    ARMAVLINK_ERROR_FILE_PARSER = -6000,                         /**< Unknown ARMAVLINK_FileParser error */
    ARMAVLINK_ERROR_FILE_PARSER_FILE_NOT_FOUND,                  /**< File to parse not found */
    ARMAVLINK_ERROR_FILE_PARSER_WORD_NOT_EXPTECTED,              /**< A word was not expected during parsing */

} eARMAVLINK_ERROR;

/**
 * @brief Gets the error string associated with an eARMAVLINK_ERROR
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARMAVLINK_Error_ToString (eARMAVLINK_ERROR error);

#endif // _ARMAVLINK_ERROR_H_
