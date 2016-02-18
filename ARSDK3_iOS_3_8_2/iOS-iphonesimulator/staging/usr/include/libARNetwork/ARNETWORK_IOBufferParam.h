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
 * @file ARNETWORK_IOBufferParam.h
 * @brief prameters used to set the parameters of a new IOBuffer
 * @date 05/18/2012
 * @author maxime.maitre@parrot.com
 */

#ifndef _ARNETWORK_IOBUFFERPARAM_H_
#define _ARNETWORK_IOBUFFERPARAM_H_

#include <libARNetworkAL/ARNETWORKAL_Frame.h>
#include <libARNetwork/ARNETWORK_Error.h>

/*****************************************
 *
 *             define :
 *
 *****************************************/

/**
 * @brief Infinite value for the IOBufferParams
 */
#define ARNETWORK_IOBUFFERPARAM_INFINITE_NUMBER -1

/**
 * @brief Automatically use the maximum value for dataCopyMaxSize.
 */
#define ARNETWORK_IOBUFFERPARAM_DATACOPYMAXSIZE_USE_MAX -1

/*****************************************
 *
 *             IOBufferParam header:
 *
 *****************************************/

/**
 * @brief used to set the parameters of a new In Out Buffer
 */
typedef struct
{
    int ID; /**< Identifier used to find the IOBuffer in a list - Valid range : 10-127 */
    eARNETWORKAL_FRAME_TYPE dataType; /**< Type of the data stored in the buffer*/
    int sendingWaitTimeMs; /**< Time in millisecond between 2 send when the IOBuffer is used with a ARNetwork_Sender */
    int ackTimeoutMs; /**< Timeout in millisecond before retry to send the data waiting an acknowledgement when the InOutBuffer is used with a ARNetwork_Sender*/
    int numberOfRetry; /**< Maximum number of retry of sending before to consider a failure when the InOutBuffer is used with a ARNetwork_Sender*/

    int32_t numberOfCell; /**< Maximum number of data stored*/
    int32_t dataCopyMaxSize; /**< Maximum Size, in byte, of the data copied in the buffer (the IOBuffer allocates a buffer of numberOfCell X dataCopyMaxSize ).
                               A value of ARNETWORK_IOBUFFERPARAM_DATACOPYMAXSIZE_USE_MAX automatically allocates the maximum size allowed by the underlying media. */
    int isOverwriting; /**< Indicator of overwriting possibility (1 = true | 0 = false)*/

}ARNETWORK_IOBufferParam_t;

/**
 * @brief initialization of the IOBufferParam with default parameters
 * @pre before to use the IOBufferParam the paramaters useful must be set.
 * @param[in,out] IOBufferParam parameters of the new input or output buffer
 * @return error of eARNETWORK_ERROR type
 */
eARNETWORK_ERROR ARNETWORK_IOBufferParam_DefaultInit (ARNETWORK_IOBufferParam_t *IOBufferParam);

/**
 * @brief check the values of the IOBufferParam
 * @param[in] IOBufferParam Parameters of the new input or output buffer
 * @return 1 if the IOBufferParam is usable for create a new ioBuffer else 0
 */
int ARNETWORK_IOBufferParam_Check (const ARNETWORK_IOBufferParam_t *IOBufferParam);

#endif /** _ARNETWORK_IOBUFFERPARAM_H_ */
