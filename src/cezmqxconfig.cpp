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

#include <list>
#include <string.h>

#include "EZMQXConfig.h"
#include "EZMQXException.h"

#include "cezmqxconfig.h"
#include "cezmqxutils.h"

CEZMQXErrorCode ezmqxCreateConfig(ezmqxConfigHandle_t *handle)
{
    VERIFY_NON_NULL(handle);
    try
    {
        *handle = EZMQX::Config::getInstance();
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxStartDockerMode(ezmqxConfigHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    EZMQX::Config* configObj = static_cast<EZMQX::Config *>(handle);
    try
    {
        configObj->startDockerMode();
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxStartStandAloneMode(ezmqxConfigHandle_t handle, int useTns, const char *tnsAddr)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(tnsAddr)
    EZMQX::Config* configObj = static_cast<EZMQX::Config *>(handle);
    try
    {
        configObj->startStandAloneMode(useTns, tnsAddr);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxAddAmlModel(ezmqxConfigHandle_t handle, const char **amlFilePath, const size_t size,
        char*** idList, size_t* listSize)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(amlFilePath)
    VERIFY_NON_NULL(idList)
    VERIFY_NON_NULL(listSize)

    //change char ** to cpp list
    std::list<std::string> filePathList;
    for (size_t i = 0; i< size; i++)
    {
        std::string valueStr(amlFilePath[i], strlen(amlFilePath[i]));
        filePathList.push_back(valueStr);
    }
    std::list<std::string> nativeList;
    EZMQX::Config* configObj = static_cast<EZMQX::Config *>(handle);
    try
    {
        nativeList = configObj->addAmlModel(filePathList);
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }

    *idList = ConvertListToCharStrArr(nativeList);
    *listSize = nativeList.size();
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxReset(ezmqxConfigHandle_t handle)
{
    VERIFY_NON_NULL(handle)
    EZMQX::Config* configObj = static_cast<EZMQX::Config *>(handle);
    try
    {
        configObj->reset();
    }
    catch(EZMQX::Exception& e)
    {
        return CEZMQXErrorCode(e.getErrCode());
    }
    return CEZMQX_OK;
}

