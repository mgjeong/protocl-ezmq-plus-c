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

#ifndef __CEZMQ_X_CONFIG_H__
#define __CEZMQ_X_CONFIG_H__

#include "cezmqxerrorcodes.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Config handle
 */
typedef void * ezmqxConfigHandle_t;

/**
* @enum CModeOption
* CEZMQX mode types.
*/
typedef enum
{
    StandAlone = 0,
    Docker
}CModeOption;

EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateConfig(CModeOption mode, ezmqxConfigHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyConfig(ezmqxConfigHandle_t handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxSetHostInfo(ezmqxConfigHandle_t handle, const char *hostName,
        const char *hostAddr);
EZMQX_EXPORT CEZMQXErrorCode ezmqxSetTnsInfo(ezmqxConfigHandle_t handle, const char * remoteAddr);
EZMQX_EXPORT CEZMQXErrorCode ezmqxAddAmlModel(ezmqxConfigHandle_t handle, const char **amlFilePath,
        const size_t size, char*** idList, size_t* listSize);
EZMQX_EXPORT CEZMQXErrorCode ezmqxReset(ezmqxConfigHandle_t handle, CModeOption mode);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_CONFIG_H__

