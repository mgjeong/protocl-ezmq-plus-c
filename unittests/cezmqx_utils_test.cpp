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

#include "cezmqxutils.h"

#include "unittesthelper.h"

class CEZMQXUtilsTest : public testing::Test
{
    protected:
        virtual void SetUp()
        {
        }
        virtual void TearDown()
        {
        }
};

TEST_F(CEZMQXUtilsTest, ToCppAmlObject)
{
    amlObjectHandle_t handle = getAMLObject();
    AML::AMLObject* almObject = nullptr;
    almObject = convertToCppAmlObject(handle);
    ASSERT_NE(nullptr, almObject);
}

TEST_F(CEZMQXUtilsTest, ToAmlObjHandle)
{
    amlObjectHandle_t handle = getAMLObject();
    AML::AMLObject* almObject = convertToCppAmlObject(handle);
    handle = nullptr;
    handle = convertToAmlObjHandle(almObject);
    ASSERT_NE(nullptr, handle);
}

