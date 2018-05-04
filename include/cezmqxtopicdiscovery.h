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

#ifndef __CEZMQ_X_TOPIC_DISCOVERY_H__
#define __CEZMQ_X_TOPIC_DISCOVERY_H__

#include "cezmqxtopic.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * End Point handle
 */
typedef void * ezmqxTDiscoveryHandle_t;

/**
 *  Create topic discovery instance.
 *
 * @param handle  -Handle will be filled as return value.
 *
 * @note Topic discovery instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyTopicDiscovery().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateTopicDiscovery(ezmqxTDiscoveryHandle_t *handle);

/**
 *  Destroy an instance of topic discovery.
 *
 * @param handle -Topic discovery handle that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyTopicDiscovery(ezmqxTDiscoveryHandle_t handle);

/**
 *  Create a topic query.
 *
 * @param handle -Topic discovery handle.
 * @param topic -Topic for which discovery need to be done.
 * @param topics - List of topics handle will be filled.
 * @param listSize - List Size.
 *
 * @note Topic handle list will be allocated, so it should be deleted after use.
 *              To destroy an instance of topic handle, use ezmqxDestroyTopic().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxQuery(ezmqxTDiscoveryHandle_t handle, const char *topic,
        ezmqxTopicHandle_t **topics, size_t* listSize);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_TOPIC_DISCOVERY_H__

