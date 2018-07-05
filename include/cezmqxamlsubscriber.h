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
 * Subscriber handle
 */
typedef void * ezmqxAMLSubHandle_t;

/**
 * Callback to get events on subscribed topic.
 */
typedef void (*cAmlSubCB)(const char * topic, amlObjectHandle_t data);

/**
 * Error Callback to get errors occured on given subscription topic.
 */
typedef void (*cSubErrCB)(const char * topic, CEZMQXErrorCode errCode);

/**
 *  Get/Create AML subscriber with given topic and callbacks.
 *
 * @param topic - [in] Topic to be subscribed.
 * @param isHierarchical - [in] Whether to use Hierarchical query option when query to tns. [0 : false, 1: true]
 * @param amlSubCb - [in] Callback to get events on given topic.
 * @param subErrCb - [in] Error callback to get errorcodes on subscribed topic.
 * @param handle - [out] Handle will be filled as return value.
 *
 * @note Subscriber instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyAMLSubscriber().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber(const char *topic, int isHierarchical,
        cAmlSubCB amlSubCb, cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle);

/**
 *  Get/Create subscriber with given topic object [handle] and callbacks.
 *
 * @param topicHandle - [in] Topic Handle.
 * @param amlSubCb - [in] Callback to get eventa on given topic.
 * @param subErrCb - [in] Error callback to get errorcodes on subscribed topic.
 * @param handle  - [out] Handle will be filled as return value.
 *
 * @note Subscriber instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyAMLSubscriber().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber1(ezmqxTopicHandle_t topicHandle, cAmlSubCB amlSubCb,
        cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle);

/**
 *  Get/Create subscriber with given topic object [handle] list and callbacks.
 *
 * @param topicHandle - [in] Topic handle list.
 * @param listSize - [in] Size of  list.
 * @param amlSubCb - [in] Callback to get events on given topic.
 * @param subErrCb - [in] Error callback to get errorcodes on subscribed topic.
 * @param handle  - [out] Subscriber Handle will be filled as return value.
 *
 * @note Subscriber instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyAMLSubscriber().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLSubscriber2(ezmqxTopicHandle_t *topicHandle, const size_t listSize,
        cAmlSubCB amlSubCb, cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle);

/**
 *  Destroy an instance of subscriber.
 *
 * @param handle - [in] AML Subscriber Handle that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyAMLSubscriber(ezmqxAMLSubHandle_t handle);

/**
 *  Terminate subscriber instance.
 *
 * @param handle - [in] AML Subscriber handle.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLSubTerminate(ezmqxAMLSubHandle_t handle);

/**
 * Check whether subscriber instance is terminated or not.
 *
 * @param handle - [in] AML Subscriber handle.
 *
 * @return 1 if terminated and 0 otherwise.
 */
EZMQX_EXPORT int ezmqxAMLSubIsTerminated(ezmqxAMLSubHandle_t handle);

/**
 *  Get list of subscribed topics.
 *
 * @param handle - [in] AML subscriber handle.
 * @param topics - [out] List of topics handles will be filled.
 * @param listSize - [out] List Size.
 *
 * @note Topic handle list will be allocated, so it should be deleted after use.
 *              To destroy an instance of topic handle, use ezmqxDestroyTopic().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLSubGetTopics(ezmqxAMLSubHandle_t handle, ezmqxTopicHandle_t **topics,
        size_t* listSize);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_AML_SUB_H__

