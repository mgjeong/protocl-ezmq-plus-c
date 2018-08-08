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
#include "unittesthelper.h"

class CEZMQXConfigTest : public testing::Test
{
    protected:
        ezmqxConfigHandle_t configHandle;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateConfig(&configHandle));
        }

        virtual void TearDown()
        {
        }
};

TEST_F(CEZMQXConfigTest, createConfig)
{
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxCreateConfig(NULL));
}

TEST_F(CEZMQXConfigTest, startStandAloneMode)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, localhost, 0, ""));
    ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
}

TEST_F(CEZMQXConfigTest, startDockerMode)
{
    ASSERT_EQ(CEZMQX_SERVICE_UNAVAILABLE, ezmqxStartDockerMode(configHandle, tnsConfPath));
    ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
}


TEST_F(CEZMQXConfigTest, addAmlModel)
{
    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, localhost, 0, ""));
    ASSERT_EQ(CEZMQX_OK, ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxAddAmlModel(configHandle, NULL, 1, &idArr, &arrsize));
    ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
}

TEST_F(CEZMQXConfigTest, reset)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxStartStandAloneMode(configHandle, localhost, 0, ""));
    ASSERT_EQ(CEZMQX_OK, ezmqxReset(configHandle));
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxReset(NULL));
}

