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
 * @file ARMAVLINK_ListUtils.h
 * @brief ARMavlink library List Utils
 * @date 14/05/2014
 * @author djavan.bertrand@parrot.com
 */
#ifndef _ARMAVLINK_LIST_UTILS_H
#define _ARMAVLINK_LIST_UTILS_H

#include <mavlink/common/mavlink.h>

/**
 * @brief a list of mission item.
 * @warning the list allocates first a small amount of memory. Then, when you add too much points, it will reallocate memory.
 * @see mavlink_mission_item_t
 */
typedef struct mission_item_list_t mission_item_list_t;

/**
 * @brief Create a new mission item list
 * @warning This function allocate memory
 * @post ARMAVLINK_ListUtils_MissionItemListDelete() must be called to delete the list and free the memory allocated.
 * @return Pointer on the new list
 * @see ARMAVLINK_ListUtils_MissionItemListDelete()
 */
mission_item_list_t* ARMAVLINK_ListUtils_MissionItemListNew(void);

/**
 * @brief Get a mission item in the list according to its index
 * @param list : a pointer on the list
 * @param[in] the index of the mission item to return
 * @retval a pointer on the mission item if it exists, otherwise null
 * @see mavlink_mission_item_t in mavlink_msg_mission_item.h
 */
mavlink_mission_item_t* ARMAVLINK_ListUtils_MissionItemListGet(const mission_item_list_t *const list, const uint16_t index);

/**
 * @brief Get the current size of the list (the number of mission item which are actually in the list)
 * @param list : a pointer on the list
 * @retval the size of the list
 */
int ARMAVLINK_ListUtils_MissionItemListGetSize(const mission_item_list_t *const list);

/**
 * @brief delete the list
 * @warning This function free memory
 * @param[inout] list : address of the pointer on the list
 */
void ARMAVLINK_ListUtils_MissionItemListDelete(mission_item_list_t **list);

#endif
