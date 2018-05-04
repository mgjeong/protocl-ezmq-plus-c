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

#include "cezmqxendpoint.h"

class CEZMQXEndPointTest : public testing::Test
{
    protected:
        ezmqxEPHandle_t endPointHandle;
        virtual void SetUp()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint(&endPointHandle));
        }

        virtual void TearDown()
        {
            ASSERT_EQ(CEZMQX_OK, ezmqxDestroyEndPoint(endPointHandle));
        }
};

TEST_F(CEZMQXEndPointTest, createEndPoint)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint1("address", &endPointHandle));
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxCreateEndPoint1("address", NULL));
}

TEST_F(CEZMQXEndPointTest, createEndPoint1)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint2("address", 5562, &endPointHandle));
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxCreateEndPoint2("address", 5562, NULL));
}

TEST_F(CEZMQXEndPointTest, destroyEndPoint)
{
    ASSERT_EQ(CEZMQX_INVALID_PARAM, ezmqxDestroyEndPoint(NULL));
}

TEST_F(CEZMQXEndPointTest, getAddr)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint2("address", 5562, &endPointHandle));
    char *address;
    ASSERT_EQ(CEZMQX_OK, ezmqxGetAddr(endPointHandle, &address));
    if(0 != strcmp("address", address))
    {
        EXPECT_EQ(CEZMQX_OK, CEZMQX_INVALID_PARAM);
    }
}

TEST_F(CEZMQXEndPointTest, getPort)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint2("address", 5562, &endPointHandle));
    int port;
    ASSERT_EQ(CEZMQX_OK,  ezmqxGetPort(endPointHandle, &port));
    EXPECT_EQ(5562, port);
}

TEST_F(CEZMQXEndPointTest, toString)
{
    ASSERT_EQ(CEZMQX_OK, ezmqxCreateEndPoint2("address", 5562, &endPointHandle));
    char *address;
    ASSERT_EQ(CEZMQX_OK, ezmqxToString(endPointHandle, &address));
    if(0 != strcmp("address:5562", address))
    {
        EXPECT_EQ(CEZMQX_OK, CEZMQX_INVALID_PARAM);
    }
}

