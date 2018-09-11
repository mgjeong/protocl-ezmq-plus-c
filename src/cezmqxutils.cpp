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

#include <string>
#include <cstring>
#include <vector>

#include "AMLException.h"

#include "cezmqxutils.h"

using namespace std;

char* ConvertStringToCharStr(std::string str)
{
    size_t size = str.size();
    char* cstr = new char[size + 1];
    memcpy(cstr, str.c_str(), size);
    cstr[size] = '\0';
    return cstr;
}

char** ConvertListToCharStrArr(list<string>& list)
{
    unsigned long size = list.size();
    char ** cstr = new char*[size];

    std::list<std::string>::iterator iter = list.begin();
    for (unsigned long i = 0; i < size; i++)
    {
        auto it = std::next(list.begin(), i);
        cstr[i] = ConvertStringToCharStr(*it);
    }
    return cstr;
}

static void freeCharArr(char** str, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(str[i]);
    }
    free(str);
}

static AML::AMLData* convertToCppAmlData(amlDataHandle_t amlDataHandle)
{
    AML::AMLData *amlData = new AML::AMLData();

    char** keys = NULL;
    size_t size = 0;
    AMLData_GetKeys(amlDataHandle, &keys, &size);

    for (size_t i = 0; i < size; i++)
    {
        CAMLValueType valType = AMLVALTYPE_STRING;
        AMLData_GetValueType(amlDataHandle, keys[i], &valType);
        if (AMLVALTYPE_STRING == valType)
        {
            char* valStr = NULL;
            AMLData_GetValueStr(amlDataHandle, keys[i], &valStr);

            amlData->setValue(keys[i], valStr);

            free(valStr);
        }
        else if (AMLVALTYPE_STRINGARRAY == valType)
        {
            char** valStrArr = NULL;
            size_t arrSize = 0;
            AMLData_GetValueStrArr(amlDataHandle, keys[i], &valStrArr, &arrSize);

            vector<string> valStrVec;
            for (size_t j = 0; j < arrSize; j++)
            {
                valStrVec.push_back(valStrArr[j]);
            }
            amlData->setValue(keys[i], valStrVec);

            freeCharArr(valStrArr, arrSize);
        }
        else if (AMLVALTYPE_AMLDATA == valType)
        {
            amlDataHandle_t valAmlDataHandle = NULL;
            AMLData_GetValueAMLData(amlDataHandle, keys[i], &valAmlDataHandle);

            AML::AMLData *valAmlData = convertToCppAmlData(valAmlDataHandle);
            amlData->setValue(keys[i], *valAmlData);

            delete valAmlData;
        }
    }

    freeCharArr(keys, size);

    return amlData;
}

AML::AMLObject* convertToCppAmlObject(amlObjectHandle_t amlObjHandle)
{
    char *deviceId = NULL, *timeStamp = NULL, *id = NULL;
    AMLObject_GetDeviceId(amlObjHandle, &deviceId);
    AMLObject_GetTimeStamp(amlObjHandle, &timeStamp);
    AMLObject_GetId(amlObjHandle, &id);

    AML::AMLObject* cppAmlObj = new AML::AMLObject(deviceId, timeStamp, id);

    free(deviceId);
    free(timeStamp);
    free(id);

    char** dataNames = NULL;
    size_t size = 0;
    AMLObject_GetDataNames(amlObjHandle, &dataNames, &size);

    for (size_t i = 0; i < size; i++)
    {
        amlDataHandle_t dataHandle = NULL;
        AMLObject_GetData(amlObjHandle, dataNames[i], &dataHandle);

        AML::AMLData *amlData = convertToCppAmlData(dataHandle);

        cppAmlObj->addData(dataNames[i], *amlData);

        delete amlData;
    }

    freeCharArr(dataNames, size);

    return cppAmlObj;
}

static amlDataHandle_t convertToAmlDataHandle(const AML::AMLData* amlData)
{
    amlDataHandle_t amlDataHandle = NULL;
    CreateAMLData(&amlDataHandle);

    vector<string> keys = amlData->getKeys();
    for (string key : keys)
    {
        AML::AMLValueType type = amlData->getValueType(key);
        if (AML::AMLValueType::String == type)
        {
            string valStr = amlData->getValueToStr(key);
            AMLData_SetValueStr(amlDataHandle, key.c_str(), valStr.c_str());
        }
        else if (AML::AMLValueType::StringArray == type)
        {
            vector<string> valStrArr = amlData->getValueToStrArr(key);

            vector<const char*> charVec(valStrArr.size());
            for (unsigned i = 0; i < valStrArr.size(); ++i)
            {
                charVec[i] = valStrArr[i].data();
            }

            AMLData_SetValueStrArr(amlDataHandle, key.c_str(), charVec.data(), charVec.size());
        }
        else if (AML::AMLValueType::AMLData == type)
        {
            AML::AMLData valAMLData = amlData->getValueToAMLData(key);
            amlDataHandle_t valAmlDataHandle = convertToAmlDataHandle(&valAMLData);

            AMLData_SetValueAMLData(amlDataHandle, key.c_str(), valAmlDataHandle);

            DestroyAMLData(valAmlDataHandle);
        }
    }

    return amlDataHandle;
}

amlObjectHandle_t convertToAmlObjHandle(const AML::AMLObject* amlObject)
{
    amlObjectHandle_t amlObjHandle = NULL;
    CreateAMLObjectWithID(amlObject->getDeviceId().c_str(), amlObject->getTimeStamp().c_str(),
                        amlObject->getId().c_str(), &amlObjHandle);

    vector<string> dataNames = amlObject->getDataNames();
    for (string name : dataNames)
    {
        AML::AMLData amlData = amlObject->getData(name);

        amlDataHandle_t amlDataHandle = convertToAmlDataHandle(&amlData);

        AMLObject_AddData(amlObjHandle, name.c_str(), amlDataHandle);

        DestroyAMLData(amlDataHandle);
    }

    return amlObjHandle;
}

