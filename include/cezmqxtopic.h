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

/**
 * Represents mapping between topic and server public key.
 */
typedef struct
{
    ezmqxTopicHandle_t topicHandle;
    const char *serverPublicKey;
} ezmqxTopicKeyMap;

/**
 *  Create topic.
 *
 * @param handle - [out] Handle will be filled as return value.
 *
 * @note Topic instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyTopic().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateTopic(ezmqxTopicHandle_t *handle);

/**
 *  Create topic instance with given topic, schema and endpoint handle.
 *
 * @param topicName - [in ]Topic name.
 * @param dataModel - [in] String formatted ID of data model.
 * @param isSecured - [in] Topic to be secured or not [0: unsecured / 1: secured].
 * @param epHandle - [in] EndPoint handle.
 * @param handle - [out] Handle will be filled as return value.
 *
 * @note Topic instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyTopic().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateTopic1(const char *topicName, const char *dataModel,
        const int isSecured, ezmqxEPHandle_t epHandle, ezmqxTopicHandle_t *handle);

/**
 *  Destroy an instance of topic.
 *
 * @param handle - [in] Topic Handle that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyTopic(ezmqxTopicHandle_t handle);

/**
 *  Get endpoint handle for given topic.
 *
 * @param handle - [in] Topic handle.
 * @param epHandle  - [out] EndPoint handle will be filled as return value.
 *
 * @note EndPoint will be allocated, so it should be deleted after use.
 *              To destroy an endpoint instance, use ezmqxDestroyEndPoint().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetEndpoint(ezmqxTopicHandle_t handle, ezmqxEPHandle_t *epHandle);

/**
 *  Get data model ID.
 *
 * @param handle - [in] Topic handle.
 * @param modeld - [out] Model ID will be filled as return value.
 *
 * @note schema will be allocated, so it should be deleted after use.
 *              To destroy address : free(schema);
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetDatamodel(ezmqxTopicHandle_t handle, char **modeld);

/**
 *  Get topic name.
 *
 * @param handle - [in] Topic handle.
 * @param topicName - [out] Topic name will be filled as return value.
 *
 * @note topic will be allocated, so it should be deleted after use.
 *              To destroy address : free(topic);
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetName(ezmqxTopicHandle_t handle, char **topicName);

/**
 *  Check whether topic is secured or not.
 *
 * @param handle - [in] Topic handle.
 *
 * @return int - 0 : unsecured / 1 : secured
 */
EZMQX_EXPORT int ezmqxIsTopicSecured(ezmqxTopicHandle_t handle);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_TOPIC_H__

