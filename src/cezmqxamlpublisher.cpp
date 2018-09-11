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

#include "camlinterface.h"
#include "AMLInterface.h"
#include "AMLException.h"

#include "cezmqxamlpublisher.h"
#include "cezmqxutils.h"

using namespace std;
using namespace EZMQX;

CEZMQXErrorCode ezmqxGetAMLPublisher(const char *topic, CAmlModelInfo infoType, const char *amlModelld,
        int optionalPort, ezmqxAMLPubHandle_t *handle)
{
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(amlModelld)
    VERIFY_NON_NULL(handle)
    try
    {
        *handle = AmlPublisher::getPublisher(topic, AmlModelInfo(infoType), amlModelld, optionalPort);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetSecuredAMLPublisher(const char *topic, const char *serverSecretKey,
        CAmlModelInfo infoType, const char *amlModelld,  int optionalPort, ezmqxAMLPubHandle_t *handle)
{
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(serverSecretKey)
    VERIFY_NON_NULL(amlModelld)
    VERIFY_NON_NULL(handle)
    try
    {
        *handle = AmlPublisher::getSecuredPublisher(topic, serverSecretKey, AmlModelInfo(infoType), amlModelld, optionalPort);
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
    AML::AMLObject *amlObj = nullptr;
    try
    {
        amlObj = convertToCppAmlObject(amlObjHandle);
        publisher->publish(*amlObj);
    }
    catch(EZMQX::Exception& e)
    {
        delete amlObj;
        return CEZMQXErrorCode(e.getErrCode());
    }
    // This is added temporarily to fix static analyzer issue, this should be handled in ezmq-plus-cpp publish API.
    catch(AML::AMLException& e)
    {
        delete amlObj;
        return CEZMQXErrorCode(InvalidAmlModel);
    }

    delete amlObj;
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
    *topicHandle = new(std::nothrow) Topic(nativeTopic.getName(), nativeTopic.getDatamodel(), nativeTopic.isSecured(),*endPoint);
    delete endPoint;
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

int ezmqxAMLPubIsSecured(ezmqxAMLPubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    AmlPublisher *publisher = static_cast<AmlPublisher *>(handle);
    return ((publisher->isSecured()) ? 1:0);
}

