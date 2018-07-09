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
#include "cezmqxamlpublisher.h"
#include "cezmqxamlpublisher.h"

#include "unittesthelper.h"

class CEZMQXAMLPubTest : public testing::Test
{
    protected:
        ezmqxConfigHandle_t configHandle;
        ezmqxAMLPubHandle_t publisherHandle;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateConfig(&configHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, 0, ""));
        }

        virtual void TearDown()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxDestroyAMLPublisher(publisherHandle));
            ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
        }
};

TEST_F(CEZMQXAMLPubTest, getAMLPublisher)
{
    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLPublisher("/topic", AmlModelId, idArr[0], 4000, &publisherHandle));
}

TEST_F(CEZMQXAMLPubTest, publish)
{
    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLPublisher("/topic", AmlModelId, idArr[0], 4000, &publisherHandle));
    amlObjectHandle_t amlObject = getAMLObject();
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLPublish(publisherHandle, amlObject));
}

TEST_F(CEZMQXAMLPubTest, getTopic)
{
    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLPublisher("/topic", AmlModelId, idArr[0], 4000, &publisherHandle));
    ezmqxTopicHandle_t topicHandle;
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLPubGetTopic(publisherHandle, &topicHandle));
    char *topic;
    ASSERT_EQ(CEZMQX_OK, ezmqxGetName(topicHandle, &topic));
    if(0 != strcmp("/topic", topic))
    {
        EXPECT_EQ(CEZMQX_OK, CEZMQX_INVALID_PARAM);
    }
}

TEST_F(CEZMQXAMLPubTest, terminate)
{
    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAMLPublisher("/topic", AmlModelId, idArr[0], 4000, &publisherHandle));
    ASSERT_EQ(0, ezmqxAMLPubIsTerminated(publisherHandle));
    ASSERT_EQ(CEZMQX_OK, ezmqxAMLPubTerminate(publisherHandle));
    ASSERT_EQ(1, ezmqxAMLPubIsTerminated(publisherHandle));
}

