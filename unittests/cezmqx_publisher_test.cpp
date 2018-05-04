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
    ezmqxAMLPubHandle_t publisherHandle;
    virtual void SetUp()
    {
        ezmqxConfigHandle_t config;
        ezmqxCreateConfig(StandAlone, &config);
        const char* amlPath[1] = {"sample_data_model.aml"};
        char** idArr;
        size_t arrsize;
        ASSERT_EQ(CEZMQX_OK,ezmqxAddAmlModel(config, amlPath, 1, &idArr, &arrsize));
        ASSERT_EQ(CEZMQX_OK, ezmqxGetPublisher("/topic", AmlModelId, idArr[0], 4000, &publisherHandle));
    }

    virtual void TearDown()
    {
        ASSERT_EQ(CEZMQX_OK, ezmqxDestroyPublisher(publisherHandle));
    }
};

TEST_F(CEZMQXAMLPubTest, publish)
{
    amlObjectHandle_t amlObject = getAMLObject();
    ASSERT_EQ(CEZMQX_OK, ezmqxPublish(publisherHandle, amlObject));
}

