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
 * @file ARNETWORK_Command.h
 * @brief Dictionary controller allow to manage a dictionary.
 * @date 02/03/2015
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARCONTROLLER_DICTIONARY_H_
#define _ARCONTROLLER_DICTIONARY_H_

#include <uthash/uthash.h>
#include <uthash/utlist.h>

#include <libARController/ARCONTROLLER_Error.h>
#include <libARController/ARCONTROLLER_DICTIONARY_Key.h>

#define ARCONTROLLER_DICTIONARY_SINGLE_KEY "ARCONTROLLER_DICTIONARY_SINGLE_KEY"

/**
 * @brief Types of dictionnary commands argument value.
 */
typedef enum  
{
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_U8,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_I8,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_U16,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_I16,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_U32,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_I32,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_U64,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_I64,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_FLOAT,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_DOUBLE,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_STRING,
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_ENUM, /**< enumeration relative to the commands. must be read as I32 type. */
     
    ARCONTROLLER_DICTIONARY_VALUE_TYPE_MAX, /**< Max of the enumeration */
}eARCONTROLLER_DICTIONARY_VALUE_TYPE;

/**
 * @brief Value Type.
 */
typedef union 
{
    uint8_t U8;
    int8_t I8;
    uint16_t U16;
    int16_t I16;
    uint32_t U32;
    int32_t I32;
    uint64_t U64;
    int64_t I64;
    float Float;
    double Double;
    char *String;
}ARCONTROLLER_DICTIONARY_VALUE_t;

/**
 * @brief Dictionary element to storing the commands arguments coming from the device.
 */
typedef struct 
{
    const char *argument; /**< Key associates to the argument.*/
    ARCONTROLLER_DICTIONARY_VALUE_t value; /**< Value associates to the key ; value of the argument.*/
    eARCONTROLLER_DICTIONARY_VALUE_TYPE valueType; /**< Type of the value.*/
    
    UT_hash_handle hh; /**< Makes this structure hashable */
}ARCONTROLLER_DICTIONARY_ARG_t;

/**
 * @brief Dictionary element to storing the commands element.
 */
typedef struct 
{
    char *key; /**< Key associates to the element.*/
    ARCONTROLLER_DICTIONARY_ARG_t *arguments; /**< Arguments of the command coming from the device. */
    
    UT_hash_handle hh; /**< Makes this structure hashable */
}ARCONTROLLER_DICTIONARY_ELEMENT_t;

/**
 * @brief Dictionary element to storing the commands coming from the device.
 */
typedef struct 
{
    eARCONTROLLER_DICTIONARY_KEY command; /**< Key associates to the command */
    ARCONTROLLER_DICTIONARY_ELEMENT_t *elements; /**< Elements of the command. */
    
    UT_hash_handle hh; /**< makes this structure hashable */
}ARCONTROLLER_DICTIONARY_COMMANDS_t;

/**
 * @brief Callback used to inform an update of an element of the dictionary.
 * @param[in] commandKey Command key of the element updated.
 * @param[in] elementDictionary The new element updated.
 * @param[in] customData Data given at the registering of the callback.
 */
typedef void (*ARCONTROLLER_DICTIONARY_CALLBACK_t) (eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);

/**
 * @brief List of callbacks to call when a command is updated.
 */
typedef struct ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t;

/**
 * @brief Dictionary storing the lists of callbacks to call when a command is updated.
 */
typedef struct ARCONTROLLER_Dictionary_t ARCONTROLLER_Dictionary_t;

/**
 * @brief Create a new Dictionary Controller
 * @warning This function allocate memory
 * @post ARCONTROLLER_Dictionary_Delete() must be called to delete the Dictionary Controller and free the memory allocated.
 * @param[in] commandKey Key of the new command.
 * @param[out] error executing error.
 * @return the new dictionary
 * @see ARCONTROLLER_Dictionary_Delete
 */
ARCONTROLLER_Dictionary_t *ARCONTROLLER_Dictionary_New (eARCONTROLLER_DICTIONARY_KEY commandKey, eARCONTROLLER_ERROR *error);

/**
 * @brief Delete the Dictionary Controller
 * @warning This function free memory
 * @param command The Dictionary controller to delete
 * @see ARCONTROLLER_Dictionary_New()
 */
void ARCONTROLLER_Dictionary_Delete (ARCONTROLLER_Dictionary_t **dictionary);

/**
 * @brief Add a callback to use when the command is received
 * @param element The dictionary element in which to add the callback.
 * @param[in] callback The callback to add.
 * @param[in] customData Data given as parameter to the callback.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_AddCallback (ARCONTROLLER_Dictionary_t *element, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);


/**
 * @brief Remove a callback used when the command is received.
 * @param element The dictionary element which remove the callback.
 * @param[in] callback The callback to remove.
 * @param[in] customData Data given at the adding of the callback
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_RemoveCallback (ARCONTROLLER_Dictionary_t *element, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);

/**
 * @brief Add a command element in a dictionary.
 * @param dictionary The dictionary which add the element.
 * @param[in] commandKey Key of the element.
 * @param[in] callback The callback to remove.
 * @param[in] customData Data given as parameter to the callback.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_AddDictionaryElement (ARCONTROLLER_Dictionary_t **dictionary, eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);

/**
 * @brief Remove a command element in a dictionary.
 * @param dictionary The dictionary which remove the element.
 * @param[in] commandKey Key of the element.
 * @param[in] callback The callback to remove.
 * @param[in] customData Data given at the adding of the callback.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_RemoveDictionaryElement (ARCONTROLLER_Dictionary_t *dictionary, eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);

/**
 * @brief Delete a  dictionary.
 * @param dictionary The dictionary to delete.
 */
void ARCONTROLLER_Dictionary_DeleteDictionary (ARCONTROLLER_Dictionary_t **dictionary);

/**
 * @brief Notify all callbacks registered in an element.
 * @param dictionary The dictionary.
 * @param[in] commandKey Key of the element.
 * @param[in] elementDictionary Arguments dictionary to pass as argument to the callbacks.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_Notify (ARCONTROLLER_Dictionary_t *dictionary, eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary);

/**
 * @brief Delete all callback of a list.
 * @param callbackList The list of callbacks.
 */
void ARCONTROLLER_Dictionary_DeleteCallbackList (ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t **callbackList);

/**
 * @brief Add a callback in a list.
 * @param callbackList The list of callbacks.
 * @param[in] callback The callback to add.
 * @param[in] customData Data given as parameter to the callback.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_AddCallbackInList (ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t **callbackList, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);

/**
 * @brief Remove a callback from a list.
 * @param callbackList The list of callbacks.
 * @param[in] callback The callback to remove.
 * @param[in] customData Data given at the adding of the callback.
 * @return error Executing error.
 */
eARCONTROLLER_ERROR ARCONTROLLER_Dictionary_RemoveCallbackFromList (ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t **callbackList, ARCONTROLLER_DICTIONARY_CALLBACK_t callback, void *customData);

/**
 * @brief Notify all callback of a list.
 * @param callbackList The list of callbacks.
 * @param[in] commandKey Key of the element; given as parameter to the callback.
 * @param[in] elementDictionary Arguments dictionary to pass as argument to the callbacks.
 */
void ARCONTROLLER_DICTIONARY_NotifyAllCallbackInList (ARCONTROLLER_DICTIONARY_CALLBAK_LIST_ELEMENT_t **callbackList, eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary);

#endif /* _ARCONTROLLER_DICTIONARY_H_ */
