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

#include "AMLInterface.h"

#include "EZMQXException.h"
#include "EZMQXAMLSubscriber.h"

#include "cezmqxamlsubscriber.h"

using namespace std;
using namespace EZMQX;

static amlDataHandle_t convertToAmlDataHandle(const AML::AMLData* amlData)
{
    amlDataHandle_t amlDataHandle = NULL;
    CreateAMLData(&amlDataHandle);

    vector<string> keys = amlData->getKeys();
    for (string key : keys)
    {
        AML::AMLValueType type = amlData->getValueType(key);
        if (AML::AMLValueType::String == type)
        {
            string valStr = amlData->getValueToStr(key);
            AMLData_SetValueStr(amlDataHandle, key.c_str(), valStr.c_str());
        }
        else if (AML::AMLValueType::StringArray == type)
        {
            vector<string> valStrArr = amlData->getValueToStrArr(key);

            vector<const char*> charVec(valStrArr.size());
            for (unsigned i = 0; i < valStrArr.size(); ++i)
            {
                charVec[i] = valStrArr[i].data();
            }

            AMLData_SetValueStrArr(amlDataHandle, key.c_str(), charVec.data(), charVec.size());
        }
        else if (AML::AMLValueType::AMLData == type)
        {
            AML::AMLData valAMLData = amlData->getValueToAMLData(key);
            amlDataHandle_t valAmlDataHandle = convertToAmlDataHandle(&valAMLData);

            AMLData_SetValueAMLData(amlDataHandle, key.c_str(), valAmlDataHandle);

            DestroyAMLData(valAmlDataHandle);
        }
    }

    return amlDataHandle;
}

static amlObjectHandle_t convertToAmlObjHandle(const AML::AMLObject* amlObject)
{
    amlObjectHandle_t amlObjHandle = NULL;
    CreateAMLObjectWithID(amlObject->getDeviceId().c_str(), amlObject->getTimeStamp().c_str(),
                        amlObject->getId().c_str(), &amlObjHandle);

    vector<string> dataNames = amlObject->getDataNames();
    for (string name : dataNames)
    {
        AML::AMLData amlData = amlObject->getData(name);

        amlDataHandle_t amlDataHandle = convertToAmlDataHandle(&amlData);

        AMLObject_AddData(amlObjHandle, name.c_str(), amlDataHandle);

        DestroyAMLData(amlDataHandle);
    }

    return amlObjHandle;
}

CEZMQXErrorCode ezmqxGetAMLSubscriber(const char *topic, int isHierarchical, cAmlSubCB amlSubCb,
        cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(handle)
    EZMQX::AmlSubCb subCb = [amlSubCb](std::string topic, const AML::AMLObject& payload)
    {
        const AML::AMLObject *amlObject = dynamic_cast<const AML::AMLObject*>(&payload);
        amlObjectHandle_t amlObjHandle = convertToAmlObjHandle(amlObject);

        amlSubCb(topic.c_str(), amlObjHandle);

        DestroyAMLObject(amlObjHandle);
    };
    EZMQX::SubErrCb errCb = [subErrCb](std::string topic, EZMQX::ErrorCode errCode)
    {
        subErrCb(topic.c_str(), CEZMQXErrorCode(errCode));
    };

    try
    {
        *handle = AmlSubscriber::getSubscriber(topic, isHierarchical, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetAMLSubscriber1(ezmqxTopicHandle_t topicHandle, cAmlSubCB amlSubCb,
        cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topicHandle)
    VERIFY_NON_NULL(handle)
    EZMQX::AmlSubCb subCb = [amlSubCb](std::string topic, const AML::AMLObject& payload)
    {
        const AML::AMLObject *amlObject = dynamic_cast<const AML::AMLObject*>(&payload);
        amlObjectHandle_t amlObjHandle = convertToAmlObjHandle(amlObject);

        amlSubCb(topic.c_str(), amlObjHandle);

        DestroyAMLObject(amlObjHandle);
    };
    EZMQX::SubErrCb errCb = [subErrCb](std::string topic, EZMQX::ErrorCode errCode)
    {
        subErrCb(topic.c_str(), CEZMQXErrorCode(errCode));
    };
    Topic *topicObj = static_cast<Topic *>(topicHandle);

    try
    {
        *handle = AmlSubscriber::getSubscriber(*topicObj, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetAMLSubscriber2(ezmqxTopicHandle_t *topicHandle, const size_t listSize,
        cAmlSubCB amlSubCb, cSubErrCB subErrCb, ezmqxAMLSubHandle_t *handle)
{
    VERIFY_NON_NULL(topicHandle)
    VERIFY_NON_NULL(handle)
    EZMQX::AmlSubCb subCb = [amlSubCb](std::string topic, const AML::AMLObject& payload)
    {
        const AML::AMLObject *amlObject = dynamic_cast<const AML::AMLObject*>(&payload);
        amlObjectHandle_t amlObjHandle = convertToAmlObjHandle(amlObject);

        amlSubCb(topic.c_str(), amlObjHandle);

        DestroyAMLObject(amlObjHandle);
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
        *handle = AmlSubscriber::getSubscriber(nativeList, subCb, errCb);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyAMLSubscriber(ezmqxAMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    AmlSubscriber *subscriber = static_cast<AmlSubscriber *>(handle);
    delete subscriber;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxAMLSubTerminate(ezmqxAMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    AmlSubscriber *subscriber = static_cast<AmlSubscriber *>(handle);
    subscriber->terminate();
    return CEZMQX_OK;
}

int ezmqxAMLSubIsTerminated(ezmqxAMLSubHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    AmlSubscriber *subscriber = static_cast<AmlSubscriber *>(handle);
    return ((subscriber->isTerminated()) ? 1:0);
}

CEZMQXErrorCode ezmqxAMLSubGetTopics(ezmqxAMLSubHandle_t handle, ezmqxTopicHandle_t **topics, size_t* listSize)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(topics)
    VERIFY_NON_NULL(listSize)
    AmlSubscriber *subscriber = static_cast<AmlSubscriber *>(handle);
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

