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
#include "cezmqxxmlsubscriber.h"
#include "unittesthelper.h"

void xmlSubCB(const char * /*topic*/, const char * /*payload*/)
{
    printf("\nxmlSubCB");
}

void xmlSubErrCB(const char * /*topic*/, CEZMQXErrorCode /*errCode*/)
{
    printf("\nxmlSubErrCB");
}

class CEZMQXXMLSubTest : public testing::Test
{
    protected:
        ezmqxConfigHandle_t configHandle;
        ezmqxEPHandle_t endpointHandle;
        ezmqxTopicHandle_t topicHandle;
        ezmqxXMLSubHandle_t subHandle;
        char** idArr;
        size_t arrsize;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateConfig(&configHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, localhost, 0, ""));
            const char* amlPath[1] = {"sample_data_model.aml"};
            ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
            ezmqxCreateEndPoint2("localhost", 4000, &endpointHandle);
            ezmqxCreateTopic1("/topic", idArr[0], 0, endpointHandle, &topicHandle);
        }

        virtual void TearDown()
        {
            if(subHandle)
            {
                ASSERT_EQ(CEZMQX_OK, ezmqxDestroyXMLSubscriber(subHandle));
            }
            ASSERT_EQ(CEZMQX_OK, ezmqxDestroyEndPoint(endpointHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxDestroyTopic(topicHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
        }
};

TEST_F(CEZMQXXMLSubTest, getXMLSubscriber)
{
    ASSERT_EQ(CEZMQX_TNS_NOT_AVAILABLE, ezmqxGetXMLSubscriber("/topic", 1, xmlSubCB, xmlSubErrCB, &subHandle));
    subHandle = NULL;
}

TEST_F(CEZMQXXMLSubTest, getXMLSubscriber1)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle));
    ASSERT_EQ(0, ezmqxXMLSubIsSecured(subHandle));
}

TEST_F(CEZMQXXMLSubTest, getXMLSubscriber2)
{
    void **topicHandleList = (void **)malloc(sizeof(void *)* 2);
    if(NULL == topicHandleList)
    {
        return;
    }
    topicHandleList[0] = topicHandle;
    topicHandleList[1] = topicHandle;
    CEZMQXErrorCode result = ezmqxGetXMLSubscriber2(topicHandleList, 2, xmlSubCB, xmlSubErrCB, &subHandle);
    free(topicHandleList);
    ASSERT_EQ(CEZMQX_OK, result);
}

TEST_F(CEZMQXXMLSubTest, getSecuredXMLSubscriber)
{
    ezmqxEPHandle_t endpoint1;
    ezmqxCreateEndPoint2("192.167.7.6", 5565, &endpoint1);
    ezmqxTopicHandle_t topicHandle1;
    ezmqxCreateTopic1("topic1", idArr[0], 1, endpoint1, &topicHandle1);
    ASSERT_EQ(CEZMQX_OK, ezmqxGetSecuredXMLSubscriber(topicHandle1, serverPublicKey, clientPublicKey,
                                                                                                clientSecretKey, xmlSubCB, xmlSubErrCB, &subHandle));
    ASSERT_EQ(1, ezmqxXMLSubIsSecured(subHandle));
}

TEST_F(CEZMQXXMLSubTest, getSecuredXMLSubscriber2)
{
    ezmqxTopicKeyMap **list = (ezmqxTopicKeyMap **)malloc(sizeof(ezmqxTopicKeyMap)*2);

    ezmqxEPHandle_t endpoint1;
    ezmqxCreateEndPoint2("192.167.7.6", 5565, &endpoint1);
    ezmqxTopicHandle_t topicHandle1;
    ezmqxCreateTopic1("topic1", idArr[0], 1, endpoint1, &topicHandle1);
    ezmqxTopicKeyMap *first = (ezmqxTopicKeyMap *)malloc(sizeof(ezmqxTopicKeyMap));
    first->serverPublicKey = serverPublicKey;
    first->topicHandle = topicHandle1;
    list[0] = first;

    ezmqxEPHandle_t endpoint2;
    ezmqxCreateEndPoint2("192.167.7.7", 5566, &endpoint2);
    ezmqxTopicHandle_t topicHandle2;
    ezmqxCreateTopic1("topic2", idArr[0], 1, endpoint2, &topicHandle2);
    ezmqxTopicKeyMap *second = (ezmqxTopicKeyMap *)malloc(sizeof(ezmqxTopicKeyMap));
    second->serverPublicKey = "tXJx&1^QE2g7WCXbF.$$TVP.wCtxwNhR8?iLiEDE";
    second->topicHandle = topicHandle2;
    list[1] = second;

    ASSERT_EQ(CEZMQX_OK, ezmqxGetSecuredXMLSubscriber2(list, 2, clientPublicKey, clientSecretKey,
                                                                                            xmlSubCB, xmlSubErrCB, &subHandle));
    ASSERT_EQ(1, ezmqxXMLSubIsSecured(subHandle));
    ezmqxDestroyEndPoint(endpoint1);
    ezmqxDestroyEndPoint(endpoint2);
    ezmqxDestroyTopic(topicHandle1);
    ezmqxDestroyTopic(topicHandle2);
    free(list);
}

TEST_F(CEZMQXXMLSubTest, destroyXMLSubscriber)
{
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxDestroyXMLSubscriber(NULL));
    subHandle = NULL;
}

TEST_F(CEZMQXXMLSubTest, xMLTerminate)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle));
    ASSERT_EQ(CEZMQX_OK, ezmqxXMLSubTerminate(subHandle));
}

TEST_F(CEZMQXXMLSubTest, xMLIsTerminated)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle));
    ASSERT_EQ(0, ezmqxXMLSubIsTerminated(subHandle));
    ASSERT_EQ(CEZMQX_OK, ezmqxXMLSubTerminate(subHandle));
    ASSERT_EQ(1, ezmqxXMLSubIsTerminated(subHandle));
}

TEST_F(CEZMQXXMLSubTest, xMLGetTopics)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle));
    ezmqxTopicHandle_t *topics;
    size_t listSize;
    ASSERT_EQ(CEZMQX_OK, ezmqxXMLSubGetTopics(subHandle , &topics, &listSize));
    ASSERT_EQ(1, (int)listSize);
    ezmqxTopicHandle_t tHandle = topics[0];
    char *topic;
    ASSERT_EQ(CEZMQX_OK, ezmqxGetName(tHandle, &topic));
    if(0 != strcmp("/topic", topic))
    {
        EXPECT_EQ(CEZMQX_OK, CEZMQX_INVALID_PARAM);
    }
}

