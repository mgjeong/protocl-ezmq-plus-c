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

#include <string.h>

#include "EZMQXAmlPublisher.h"
#include "EZMQXException.h"

#include "AMLInterface.h"

#include "cezmqxamlpublisher.h"

using namespace std;
using namespace EZMQX;

CEZMQXErrorCode ezmqxGetAMLPublisher(const char *topic, CAmlModelInfo infoType, const char *amlModeld,
        int optionalPort, ezmqxAMLPubHandle_t *handle)
{
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(amlModeld)
    VERIFY_NON_NULL(handle)
    try
    {
        *handle = AmlPublisher::getPublisher(topic, AmlModelInfo(infoType), amlModeld, optionalPort);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyAMLPublisher(ezmqxAMLPubHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    delete publisher;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxAMLPublish(ezmqxAMLPubHandle_t handle, amlObjectHandle_t amlObjHandle)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(amlObjHandle)
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    AML::AMLObject *amlObj = static_cast<AML::AMLObject*>(amlObjHandle);
    try
    {
        publisher->publish(*amlObj);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxAMLPubGetTopic(ezmqxAMLPubHandle_t handle, ezmqxTopicHandle_t *topicHandle)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(topicHandle)
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    EZMQX::Topic nativeTopic;
    try
    {
        nativeTopic = publisher->getTopic();
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    EZMQX::Endpoint nativeEndPoint = nativeTopic.getEndpoint();
    Endpoint *endPoint = new Endpoint(nativeEndPoint.getAddr(), nativeEndPoint.getPort());
    *topicHandle = new(std::nothrow) Topic(nativeTopic.getName(), nativeTopic.getDatamodel(), *endPoint);
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxAMLPubTerminate(ezmqxAMLPubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    try
    {
        publisher->terminate();
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

int ezmqxAMLPubIsTerminated(ezmqxAMLPubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    return ((publisher->isTerminated()) ? 1:0);
}


