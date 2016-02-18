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
 * @file ARMAVLINK_Manager.h
 * @brief ARMavlink manager.
 * @date 11/13/2013
 * @author frederic.dhaeyer@parrot.com
 */

#ifndef _ARMAVLINK_MANAGER_H_
#define _ARMAVLINK_MANAGER_H_

#include <libARMavlink/ARMAVLINK_Error.h>

/**
 * @brief MapMyHouse manager structure allows to manage the MapMyHouse process
 */
typedef struct ARMAVLINK_Manager_t ARMAVLINK_Manager_t;

/**
 * @brief Create a new Manager
 * @warning This function allocate memory
 * @post ARMAVLINK_Manager_Delete() must be called to delete the MapMyHouse manager and free the memory allocated.
 * @param[out] error : pointer on the error output.
 * @return Pointer on the new Manager
 * @see ARMAVLINK_Manager_Delete()
 */
ARMAVLINK_Manager_t* ARMAVLINK_Manager_New(eARMAVLINK_ERROR *error);

/**
 * @brief Delete the Manager
 * @warning This function free memory
 * @param manager : address of the pointer on the Manager
 * @see ARMAVLINK_Manager_New()
 */
void ARMAVLINK_Manager_Delete(ARMAVLINK_Manager_t **manager);

#endif /* _ARMAVLINK_MANAGER_H_ */
