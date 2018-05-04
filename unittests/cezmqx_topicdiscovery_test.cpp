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

#include "cezmqxtopicdiscovery.h"

class CEZMQXTDiscoveryTest : public testing::Test
{
    protected:
        ezmqxTDiscoveryHandle_t discoveryHandle;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateTopicDiscovery(&discoveryHandle));
        }

        virtual void TearDown()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxDestroyTopicDiscovery(discoveryHandle));
        }
};

TEST_F(CEZMQXTDiscoveryTest, createTopicDiscovery)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateTopicDiscovery(&discoveryHandle));
}

TEST_F(CEZMQXTDiscoveryTest, destroyTopicDiscovery)
{
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxDestroyTopicDiscovery(NULL));
}

TEST_F(CEZMQXTDiscoveryTest, query)
{
    ezmqxTopicHandle_t *topics;
    size_t listSize;
    ASSERT_EQ(CEZMQX_TNS_NOT_AVAILABLE, ezmqxQuery(discoveryHandle, "/topic", &topics, &listSize));
}

