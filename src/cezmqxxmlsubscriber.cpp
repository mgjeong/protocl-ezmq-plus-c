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

#include "EZMQXXMLSubscriber.h"
#include "EZMQXException.h"

#include "cezmqxxmlsubscriber.h"

using namespace std;
using namespace EZMQX;

CEZMQXErrorCode ezmqxGetXMLSubscriber(const char *topic, int isHierarchical, cXmlSubCB xmlSubCb,
        cXmlSubErrCB subErrCb, ezmqxXMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(handle)
    EZMQX::XmlSubCb subCb = [xmlSubCb](std::string topic, const std::string& payload)
    {
        xmlSubCb(topic.c_str(), payload.c_str());
    };
    EZMQX::SubErrCb errCb = [subErrCb](std::string topic, EZMQX::ErrorCode errCode)
    {
        subErrCb(topic.c_str(), CEZMQXErrorCode(errCode));
    };
    try
    {
        *handle = XmlSubscriber::getSubscriber(topic, isHierarchical, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetXMLSubscriber1(ezmqxTopicHandle_t topicHandle, cXmlSubCB xmlSubCb,
        cXmlSubErrCB subErrCb, ezmqxXMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topicHandle)
    VERIFY_NON_NULL(handle)

    EZMQX::XmlSubCb subCb = [xmlSubCb](std::string topic, const std::string& payload)
    {
        xmlSubCb(topic.c_str(), payload.c_str());
    };
    EZMQX::SubErrCb errCb = [subErrCb](std::string topic, EZMQX::ErrorCode errCode)
    {
        subErrCb(topic.c_str(), CEZMQXErrorCode(errCode));
    };

    Topic *topicObj = static_cast<Topic *>(topicHandle);
    try
    {
        *handle = XmlSubscriber::getSubscriber(*topicObj, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetXMLSubscriber2(ezmqxTopicHandle_t *topicHandle, const size_t listSize,
        cXmlSubCB xmlSubCb, cXmlSubErrCB subErrCb, ezmqxXMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topicHandle)
    VERIFY_NON_NULL(handle)
    EZMQX::XmlSubCb subCb = [xmlSubCb](std::string topic, const std::string& payload)
    {
        xmlSubCb(topic.c_str(), payload.c_str());
    };
    EZMQX::SubErrCb errCb = [subErrCb](std::string topic, EZMQX::ErrorCode errCode)
    {
        subErrCb(topic.c_str(), CEZMQXErrorCode(errCode));
    };

    std::list<EZMQX::Topic> nativeList;
    for (size_t i = 0; i< listSize; i++)
    {
        void *topic = (void *)(topicHandle[i]);
        Topic *topicObj = static_cast<Topic *>(topic);
        nativeList.push_back(*topicObj);
    }

    try
    {
        *handle = XmlSubscriber::getSubscriber(nativeList, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyXMLSubscriber(ezmqxXMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    XmlSubscriber *subscriber = static_cast<XmlSubscriber *>(handle);
    delete subscriber;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxXMLSubTerminate(ezmqxXMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    XmlSubscriber *subscriber = static_cast<XmlSubscriber *>(handle);
    subscriber->terminate();
    return CEZMQX_OK;
}

int ezmqxXMLSubIsTerminated(ezmqxXMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    XmlSubscriber *subscriber = static_cast<XmlSubscriber *>(handle);
    return ((subscriber->isTerminated()) ? 1:0);
}

CEZMQXErrorCode ezmqxXMLSubGetTopics(ezmqxXMLSubHandle_t handle, ezmqxTopicHandle_t **topics,
        size_t* listSize)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(topics)
    VERIFY_NON_NULL(listSize)
    XmlSubscriber *subscriber = static_cast<XmlSubscriber *>(handle);
    std::list<EZMQX::Topic> nativeList = subscriber->getTopics();

    *listSize = nativeList.size();
    *topics = (void **)malloc(sizeof(void *) * (*listSize));
    ALLOC_ASSERT(*topics)
    for (size_t i = 0; i < (*listSize); i++)
    {
        auto it = std::next(nativeList.begin(), i);
        void  *topicHandle = (&(*it));
        (*topics)[i] = topicHandle;
    }
    return CEZMQX_OK;
}

