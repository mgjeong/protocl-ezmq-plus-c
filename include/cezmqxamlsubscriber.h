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

#ifndef __CEZMQ_X_AML_SUB_H__
#define __CEZMQ_X_AML_SUB_H__

#include "camlinterface.h"
#include "cezmqxerrorcodes.h"
#include "cezmqxtopic.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Subscrbier handle
 */
typedef void * ezmqxAMLSubHandle_t;

/**
 * Callback to get all the subscribed events.
 */
typedef void (*cAmlSubCB)(const char * topic, amlObjectHandle_t data);

/**
 * Callback to get all the subscribed events for a specific topic.
 */
typedef void (*cSubErrCB)(const char * topic, CEZMQXErrorCode errCode);

EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber(const char *topic, cAmlSubCB amlSubCb, cSubErrCB subErrCb,
        ezmqxAMLSubHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber1(ezmqxTopicHandle_t topicHandle, cAmlSubCB amlSubCb,
        cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber2(ezmqxTopicHandle_t *topicHandle, const size_t listSize,
        cAmlSubCB amlSubCb, cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyAMLSubscriber(ezmqxAMLSubHandle_t handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLTerminate(ezmqxAMLSubHandle_t handle);
EZMQX_EXPORT int ezmqxAMLIsTerminated(ezmqxAMLSubHandle_t handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLGetTopics(ezmqxAMLSubHandle_t handle, ezmqxTopicHandle_t **topics,
        size_t* listSize);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_AML_SUB_H__

