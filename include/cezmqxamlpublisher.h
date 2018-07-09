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
#include "cezmqxtopic.h"

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

/**
 *  Get/Create publisher with given details.
 *
 * @param topic - [in] Topic on which publisher will be publishing.
 * @param infoType - [in] CAmlModelInfo.
 * @param amlModeld - [in] AML Model Id.
 * @param optionalPort - [in] Port to be bound to publisher .
 * @param handle - [out] Handle will be filled as return value.
 *
 * @note Publisher instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyAMLPublisher().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAMLPublisher(const char *topic, CAmlModelInfo infoType,
        const char *amlModeld,  int optionalPort, ezmqxAMLPubHandle_t *handle);

/**
 *  Destroy an instance of publisher.
 *
 * @param handle - [in] Handle of Publisher that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyAMLPublisher(ezmqxAMLPubHandle_t handle);

/**
 *  Publish event [AML object] on socket for subscribers.
 *
 * @param handle - [in] Publisher handle.
 * @param object - [in] AML object to be published.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLPublish(ezmqxAMLPubHandle_t handle, amlObjectHandle_t object);

/**
 *  Get topic on which publisher is publishing.
 *
 * @param handle - [in] AML publisher handle.
 * @param topicHandle - [out] Topic handle will be filled.
 *
 * @note Topic handle will be allocated, so it should be deleted after use.
 *              To destroy topic handle, use ezmqxDestroyTopic().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLPubGetTopic(ezmqxAMLPubHandle_t handle, ezmqxTopicHandle_t *topicHandle);

/**
 * Terminate AmlPublisher instance.
 *
 * @param handle  - [in] Publisher handle.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAMLPubTerminate(ezmqxAMLPubHandle_t handle);

/**
 * Check whether publisher  instance is terminated or not.
 *
 * @param handle - [in] AML publisher handle.
 *
 * @return 1 if terminated and 0 otherwise.
 */
EZMQX_EXPORT int ezmqxAMLPubIsTerminated(ezmqxAMLPubHandle_t handle);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_AML_PUB_H__

