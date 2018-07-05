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

#include "EZMQXTopic.h"

#include "cezmqxtopic.h"
#include "cezmqxutils.h"

using namespace std;
using namespace EZMQX;

CEZMQXErrorCode ezmqxCreateTopic(ezmqxTopicHandle_t *handle)
{
    VERIFY_NON_NULL(handle)
    *handle = new(std::nothrow) Topic();
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxCreateTopic1(const char *topicName, const char *dataModel, ezmqxEPHandle_t epHandle,
        ezmqxTopicHandle_t *handle)
{
    VERIFY_NON_NULL(topicName)
    VERIFY_NON_NULL(dataModel)
    VERIFY_NON_NULL(epHandle)
    VERIFY_NON_NULL(handle)
    Endpoint *endPoint = static_cast<Endpoint *>(epHandle);
    *handle = new(std::nothrow) Topic(topicName, dataModel, *endPoint);
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyTopic(ezmqxTopicHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    Topic* topic = static_cast<Topic *>(handle);
    delete topic;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetEndpoint(ezmqxTopicHandle_t handle, ezmqxEPHandle_t *epHandle)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(epHandle)
    Topic *topic = static_cast<Topic *>(handle);
    Endpoint endPoint = topic->getEndpoint();
    Endpoint *epObj = new Endpoint(endPoint.getAddr(), endPoint.getPort());
    *epHandle = epObj;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetDatamodel(ezmqxTopicHandle_t handle, char **modeld)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(modeld)
    Topic *topic = static_cast<Topic *>(handle);
    *modeld = ConvertStringToCharStr(topic->getDatamodel());
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetName(ezmqxTopicHandle_t handle, char **topicName)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(topicName)
    Topic *topicObj = static_cast<Topic *>(handle);
    *topicName = ConvertStringToCharStr(topicObj->getName());
    return CEZMQX_OK;
}

