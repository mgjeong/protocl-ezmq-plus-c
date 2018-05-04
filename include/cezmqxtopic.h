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

#ifndef __CEZMQ_X_TOPIC_H__
#define __CEZMQ_X_TOPIC_H__

#include "cezmqxendpoint.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Topic handle
 */
typedef void * ezmqxTopicHandle_t;

EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateTopic(ezmqxTopicHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateTopic1(const char *topic, const char *schema,
        ezmqxEPHandle_t epHandle, ezmqxTopicHandle_t *handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyTopic(ezmqxTopicHandle_t handle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetEndpoint(ezmqxTopicHandle_t handle, ezmqxEPHandle_t *epHandle);
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetSchema(ezmqxTopicHandle_t handle, char **schema);
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetTopic(ezmqxTopicHandle_t handle, char **topic);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_TOPIC_H__

