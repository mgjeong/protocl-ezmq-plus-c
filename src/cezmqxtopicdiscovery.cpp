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
#include <list>

#include "EZMQXTopicDiscovery.h"
#include "EZMQXException.h"

#include "cezmqxtopicdiscovery.h"

using namespace EZMQX;

CEZMQXErrorCode ezmqxCreateTopicDiscovery(ezmqxTDiscoveryHandle_t *handle)
{
    VERIFY_NON_NULL(handle)
    *handle  =  new(std::nothrow) TopicDiscovery();
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyTopicDiscovery(ezmqxTDiscoveryHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    TopicDiscovery* discoveryObj = static_cast<TopicDiscovery*>(handle);
    delete discoveryObj;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxQuery(ezmqxTDiscoveryHandle_t handle, const char *topic, ezmqxTopicHandle_t **topics,
        size_t* listSize)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(topic)
    VERIFY_NON_NULL(topics)
    VERIFY_NON_NULL(listSize)
    TopicDiscovery *tDiscovery = static_cast<TopicDiscovery *>(handle);
    std::list<EZMQX::Topic> nativeList;
    try
    {
        nativeList = tDiscovery->query(topic);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }

    *listSize = nativeList.size();
    *topics = (void **)malloc(sizeof(void *) * (*listSize));
    ALLOC_ASSERT(*topics)
    for (size_t i = 0; i < (*listSize); i++)
    {
        auto iterator = std::next(nativeList.begin(), i);
        void  *topicHandle = (&(*iterator));
        (*topics)[i] = topicHandle;
    }
    return CEZMQX_OK;
}

