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

#include "cezmqxamlpublisher.h"

using namespace std;
using namespace EZMQX;

static void freeCharArr(char** str, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(str[i]);
    }
    free(str);
}

static AML::AMLData* convertToCppAmlData(amlDataHandle_t amlDataHandle)
{
    AML::AMLData *amlData = new AML::AMLData();

    char** keys = NULL;
    size_t size = 0;
    AMLData_GetKeys(amlDataHandle, &keys, &size);

    for (size_t i = 0; i < size; i++)
    {
        CAMLValueType valType = AMLVALTYPE_STRING;
        AMLData_GetValueType(amlDataHandle, keys[i], &valType);
        if (AMLVALTYPE_STRING == valType)
        {
            char* valStr = NULL;
            AMLData_GetValueStr(amlDataHandle, keys[i], &valStr);

            amlData->setValue(keys[i], valStr);

            free(valStr);
        }
        else if (AMLVALTYPE_STRINGARRAY == valType)
        {
            char** valStrArr = NULL;
            size_t arrSize = 0;
            AMLData_GetValueStrArr(amlDataHandle, keys[i], &valStrArr, &arrSize);

            vector<string> valStrVec;
            for (size_t j = 0; j < arrSize; j++)
            {
                valStrVec.push_back(valStrArr[j]);
            }
            amlData->setValue(keys[i], valStrVec);

            freeCharArr(valStrArr, arrSize);
        }
        else if (AMLVALTYPE_AMLDATA == valType)
        {
            amlDataHandle_t valAmlDataHandle = NULL;
            AMLData_GetValueAMLData(amlDataHandle, keys[i], &valAmlDataHandle);

            AML::AMLData *valAmlData = convertToCppAmlData(valAmlDataHandle);
            amlData->setValue(keys[i], *valAmlData);

            delete valAmlData;
        }
    }

    freeCharArr(keys, size);

    return amlData;
}

static AML::AMLObject* convertToCppAmlObject(amlObjectHandle_t amlObjHandle)
{
    char *deviceId = NULL, *timeStamp = NULL, *id = NULL;
    AMLObject_GetDeviceId(amlObjHandle, &deviceId);
    AMLObject_GetTimeStamp(amlObjHandle, &timeStamp);
    AMLObject_GetId(amlObjHandle, &id);

    AML::AMLObject* cppAmlObj = new AML::AMLObject(deviceId, timeStamp, id);

    free(deviceId);
    free(timeStamp);
    free(id);

    char** dataNames = NULL;
    size_t size = 0;
    AMLObject_GetDataNames(amlObjHandle, &dataNames, &size);

    for (size_t i = 0; i < size; i++)
    {
        amlDataHandle_t dataHandle = NULL;
        AMLObject_GetData(amlObjHandle, dataNames[i], &dataHandle);

        AML::AMLData *amlData = convertToCppAmlData(dataHandle);

        cppAmlObj->addData(dataNames[i], *amlData);

        delete amlData;
    }

    freeCharArr(dataNames, size);

    return cppAmlObj;
}

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
    AML::AMLObject *amlObj = convertToCppAmlObject(amlObjHandle);
    try
    {
        publisher->publish(*amlObj);
    }
    catch(EZMQX::Exception& e)
    {
        delete amlObj;
        return CEZMQXErrorCode(e.getErrCode());
    }
    // This is added temporarily to fix static analyzer issue, this should be handled in ezmq-plus-cpp publish API.
    catch(std::exception& e)
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
    *topicHandle = new(std::nothrow) Topic(nativeTopic.getName(), nativeTopic.getDatamodel(), *endPoint);
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


