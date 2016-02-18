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
 * @file ARDATATRANSFER_Manager.h
 * @brief libARDataTransfer Manager header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARDATATRANSFER_MANAGER_H_
#define _ARDATATRANSFER_MANAGER_H_

/**
 * @brief Manager structure
 * @see ARDATATRANSFER_Manager_New ()
 */
typedef struct ARDATATRANSFER_Manager_t ARDATATRANSFER_Manager_t;

/**
 * @brief Create a new ARDataTransfer Manager
 * @warning This function allocates memory
 * @param[out] error A pointer on the error output
 * @return Pointer on the new ARDataTransfer Manager
 * @see ARDATATRANSFER_Manager_Delete ()
 */
ARDATATRANSFER_Manager_t* ARDATATRANSFER_Manager_New (eARDATATRANSFER_ERROR *error);

/**
 * @brief Delete an ARDataTransfer Manager
 * @warning This function frees memory
 * @param manager The address of the pointer on the ARDataTransfer Manager
 * @see ARDATATRANSFER_Manager_New ()
 */
void ARDATATRANSFER_Manager_Delete (ARDATATRANSFER_Manager_t **managerPtrAddr);

#endif /* _ARDATATRANSFER_MANAGER_H_ */
