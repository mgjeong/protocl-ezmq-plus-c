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

#ifndef __CEZMQ_X_AML_PUB_H__
#define __CEZMQ_X_AML_PUB_H__

#include "camlinterface.h"

#include "cezmqxerrorcodes.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @enum CAmlModelInfo
* AML model info.
*/
typedef enum
{
    AmlModelId = 0,
    AmlFilePath
}CAmlModelInfo;

/**
 * Publisher handle
 */
typedef void * ezmqxAMLPubHandle_t;

EZMQX_EXPORT CEZMQXErrorCode ezmqxGetPublisher(const char *topic, CAmlModelInfo infoType,
        const char *amlModeld,  int optionalPort, ezmqxAMLPubHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyPublisher(ezmqxAMLPubHandle_t handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxPublish(ezmqxAMLPubHandle_t handle, amlObjectHandle_t object);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_AML_PUB_H__

