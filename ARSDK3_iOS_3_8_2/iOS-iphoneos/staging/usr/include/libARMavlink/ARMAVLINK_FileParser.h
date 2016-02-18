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
 * @file ARMAVLINK_FileParser.h
 * @brief Mavlink File Parser
 * @date 14/05/2014
 * @author djavan.bertrand@parrot.com
 */
#ifndef _ARMAVLINK_FILEPARSER_h
#define _ARMAVLINK_FILEPARSER_h

#include <libARMavlink/ARMAVLINK_Error.h>
#include <libARMavlink/ARMAVLINK_ListUtils.h>
#include <mavlink/common/mavlink.h>

/**
 * @brief ARMavlink file parser structure allows to manage the process of parsing Mavlink files
 */
typedef struct ARMAVLINK_FileParser_t ARMAVLINK_FileParser_t;


/**
 * @brief Create a new file parser
 * @warning This function allocate memory
 * @post ARMAVLINK_FileParser_Delete() must be called to delete the file parser and free the memory allocated.
 * @param[out] error : pointer on the error output.
 * @return Pointer on the new file parser
 * @see ARMAVLINK_FileParser_Delete()
 */
ARMAVLINK_FileParser_t* ARMAVLINK_FileParser_New(eARMAVLINK_ERROR *error);

/**
 * @brief Delete the File parser
 * @warning This function free memory
 * @param fileParser : address of the pointer on the file parser
 * @see ARMAVLINK_FileParser_New()
 */
void ARMAVLINK_FileParser_Delete(ARMAVLINK_FileParser_t **fileParser);

/**
 * @brief Parse a Mavlink file and store it in a list
 * @param fileParser : pointer on the file parser
 * @param[in] filePath : the path of the file to parse
 * @param[out] missionItemList : a list of all mission items
 * @return ARMAVLINK_OK if parsing went well, the enum description of the error otherwise
 * @see mission_item_list_t
 */
eARMAVLINK_ERROR ARMAVLINK_FileParser_Parse(ARMAVLINK_FileParser_t *fileParser, const char *const filePath, mission_item_list_t *missionItemList);


#endif
