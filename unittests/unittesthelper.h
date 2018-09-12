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

#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>

#include "camlinterface.h"

const char *tnsConfPath = "tnsConf.json";
const char *localhost = "localhost";
const char *serverPublicKey = "tXJx&1^QE2g7WCXbF.$$TVP.wCtxwNhR8?iLi&S<";
const char *clientPublicKey = "-QW?Ved(f:<::3d5tJ$[4Er&]6#9yr=vha/caBc(";
const char *clientSecretKey = "ZB1@RS6Kv^zucova$kH(!o>tZCQ.<!Q)6-0aWFmW";

char* getCurrentTime()
{
    char* timeStr = (char*)malloc(sizeof(char) * 10);
    if (NULL == timeStr) return NULL;

    struct timeb tp;
    ftime(&tp);
    strftime(timeStr, 7, "%H%M%S", localtime(&tp.time));

    char milliSec[4];
    snprintf(milliSec, 4, "%d", tp.millitm);

    strncpy(timeStr + 6, milliSec, 3);
    timeStr[9] = '\0';

    return timeStr;
}

amlObjectHandle_t getAMLObject()
{
    amlDataHandle_t model;
    CreateAMLData(&model);

    AMLData_SetValueStr(model, "ctname", "Model_107.113.97.248");
    AMLData_SetValueStr(model, "con", "SR-P7-970");

    // create "Sample" data
    amlDataHandle_t axis;
    CreateAMLData(&axis);
    AMLData_SetValueStr(axis, "x", "20");
    AMLData_SetValueStr(axis, "y", "110");
    AMLData_SetValueStr(axis, "z", "80");

    amlDataHandle_t info;
    CreateAMLData(&info);
    AMLData_SetValueStr(info, "id", "f437da3b");
    AMLData_SetValueAMLData(info, "axis", axis);

    amlDataHandle_t sample;
    CreateAMLData(&sample);
    AMLData_SetValueAMLData(sample, "info", info);
    const char* appendix[3] = {"935", "52303", "1442"};
    AMLData_SetValueStrArr(sample, "appendix", appendix, 3);

    // set datas to object
    amlObjectHandle_t object;
    char* currTime = getCurrentTime();
    CreateAMLObject("Robot0001", currTime, &object);
    free(currTime);
    AMLObject_AddData(object, "Model", model);
    AMLObject_AddData(object, "Sample", sample);

    return object;
}

