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

#include <gtest.h>
#include <gmock.h>

#include "cezmqxconfig.h"
#include "cezmqxamlsubscriber.h"
#include "unittesthelper.h"

void amlSubCB(const char * /*topic*/, amlObjectHandle_t /*amlObject*/)
{
    printf("\namlSubCB");
}

void amlSubErrCB(const char * /*topic*/, CEZMQXErrorCode /*errCode*/)
{
    printf("\namlSubErrCB");
}

class CEZMQXAMLSubTest : public testing::Test
{
    protected:
        ezmqxConfigHandle_t configHandle;
        ezmqxEPHandle_t endpointHandle;
        ezmqxTopicHandle_t topicHandle;
        ezmqxAMLSubHandle_t subHandle;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateConfig(&configHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, localhost, 0, ""));
            const char* amlPath[1] = {"sample_data_model.aml"};
            char** idArr;
            size_t arrsize;
            ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
            ezmqxCreateEndPoint2("localhost", 4000, &endpointHandle);
            ezmqxCreateTopic1("/topic", idArr[0], endpointHandle, &topicHandle);
        }

        virtual void TearDown()
        {
            if(subHandle)
            {
                ASSERT_EQ(CEZMQX_OK, ezmqxDestroyAMLSubscriber(subHandle));
            }
            ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
        }
};

TEST_F(CEZMQXAMLSubTest, getAMLSubscriber)
{
    ASSERT_EQ(CEZMQX_TNS_NOT_AVAILABLE, ezmqxGetAMLSubscriber("/topic", 1, amlSubCB, amlSubErrCB, &subHandle));
    subHandle = NULL;
}

TEST_F(CEZMQXAMLSubTest, getAMLSubscriber1)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle));
}

TEST_F(CEZMQXAMLSubTest, getAMLSubscriber2)
{
    void **topicHandleList = (void **)malloc(sizeof(void *) * 2);
    if(NULL == topicHandleList)
    {
        return;
    }
    topicHandleList[0] = topicHandle;
    topicHandleList[1] = topicHandle;
    CEZMQXErrorCode result = ezmqxGetAMLSubscriber2(topicHandleList, 2, amlSubCB, amlSubErrCB, &subHandle);
    free(topicHandleList);
    ASSERT_EQ(CEZMQX_OK, result);
}

TEST_F(CEZMQXAMLSubTest, destroyAMLSubscriber)
{
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxDestroyAMLSubscriber(NULL));
    subHandle = NULL;
}

TEST_F(CEZMQXAMLSubTest, aMLTerminate)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle));
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLSubTerminate(subHandle));
}

TEST_F(CEZMQXAMLSubTest, aMLIsTerminated)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle));
    ASSERT_EQ(0, ezmqxAMLSubIsTerminated(subHandle));
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLSubTerminate(subHandle));
    ASSERT_EQ(1, ezmqxAMLSubIsTerminated(subHandle));
}

TEST_F(CEZMQXAMLSubTest, aMLGetTopics)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle));
    ezmqxTopicHandle_t *topics;
    size_t listSize;
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLSubGetTopics(subHandle , &topics, &listSize));
    ASSERT_EQ(1, (int)listSize);
    ezmqxTopicHandle_t tHandle = topics[0];
    char *topic;
    ASSERT_EQ(CEZMQX_OK, ezmqxGetName(tHandle, &topic));
    if(0 != strcmp("/topic", topic))
    {
        EXPECT_EQ(CEZMQX_OK, CEZMQX_INVALID_PARAM);
    }
}

