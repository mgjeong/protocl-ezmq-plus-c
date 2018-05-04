/*******************************************************************************
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *******************************************************************************/

/**
 * @file   cezmqxerrorcodes.h
 *
 * @brief   This file contains error codes of CEZMQ service possibly return to application.
 */

#ifndef __CEZMQ_X_ERRORCODES_H_INCLUDED__
#define __CEZMQ_X_ERRORCODES_H_INCLUDED__

#define VERIFY_NON_NULL(PARAM) if (!PARAM){ return CEZMQX_INVALID_PARAM; }
#define ALLOC_ASSERT(PARAM) if (!PARAM){ abort(); }

/**
* @enum CEZMQXErrorCode
* ezmqx service error codes.
*/
typedef enum
{
    CEZMQX_OK = 0,
    CEZMQX_INVALID_PARAM,
    CEZMQX_NOT_INITIALIZED,
    CEZMQX_TERMINATED,
    CEZMQX_UNKNOWN_STATE,
    CEZMQX_SERVICE_UNAVAILABLE,
    CEZMQX_INVALID_TOPIC,
    CEZMQX_DUPLICATED_TOPIC,
    CEZMQX_UNKNOWN_TOPIC,
    CEZMQX_INVALID_END_POINT,
    CEZMQX_BROKEN_PAYLOAD,
    CEZMQX_REST_ERROR,
    CEZMQX_MAXIMUM_PORT_EXCEED,
    CEZMQX_RELEASE_WRONG_PORT,
    CEZMQX_NO_TOPIC_MATCHED,
    CEZMQX_TNS_NOT_AVAILABLE,
} CEZMQXErrorCode;

#endif //__CEZMQ_X_ERRORCODES_H_INCLUDED__

