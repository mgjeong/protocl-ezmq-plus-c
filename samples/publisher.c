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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <signal.h>

#include "camlinterface.h"
#include "camlrepresentation.h"
#include "camlerrorcodes.h"

#include "cezmqxconfig.h"
#include "cezmqxamlpublisher.h"
#include "cezmqxendpoint.h"
#include "cezmqxtopic.h"

#define UNUSED(x) (void)(x)

ezmqxConfigHandle_t g_configHandle;
ezmqxAMLPubHandle_t g_pubHandle;
int g_isStarted = 0;

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

amlObjectHandle_t createAMLObject()
{
    // create "Model" data
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

    DestroyAMLData(model);
    DestroyAMLData(axis);
    DestroyAMLData(info);
    DestroyAMLData(sample);

    return object;
}

void printError()
{
    printf("\nRe-run the application as shown in below example: \n");
    printf("\n  (1) For running in standalone mode: ");
    printf("\n      ./publisher -t /topic -port 5562\n");
    printf("\n  (2)  For running in docker mode: ");
    printf("\n      ./publisher -t /topic \n");
}

void publishData(int numberOfEvents)
{
    printf("\nWill publish %d events at a interval of 1 seconds!!!\n", numberOfEvents);
    for (int i=1; i <= numberOfEvents; i++)
    {
        printf("\nPublished data: %d\n", i);

        amlObjectHandle_t amlObject = createAMLObject();
        CEZMQXErrorCode result = ezmqxAMLPublish(g_pubHandle, amlObject);
        if(result != CEZMQX_OK)
        {
            printf("\nPublish failed [Result]: %d\n", result);
        }
        DestroyAMLObject(amlObject);

        sleep(1);
    }
}

void sigint(int signal)
{
    printf("\nInterupt signal:  %d\n", signal);
    if (g_isStarted)
    {
        CEZMQXErrorCode result = ezmqxAMLPubTerminate(g_pubHandle);
        if(result != CEZMQX_OK)
        {
            printf("\nPublihser terminate failed [Result]: %d\n", result);
        }
        result = ezmqxReset(g_configHandle);
        if(result != CEZMQX_OK)
        {
            printf("\nReset config failed [Result]: %d\n", result);
        }
        printf("\nReset config done: [Result]: %d\n", result);

    }
    exit(0);
}

int main(int argc, char* argv[])
{
    CEZMQXErrorCode result;
    char *topic = NULL;
    int port = 0;
    int isStandAlone = 0;

    // get port and topic from command line arguments
    if(argc != 3 && argc != 5)
    {
        printError();
        return -1;
    }
    int n = 1;
    while (n < argc)
    {
         if (0 == strcmp(argv[n],"-t"))
        {
            topic = argv[n + 1];
            printf("\nGiven Topic is : %s", topic);
            n = n + 2;
        }
        else if (0 == strcmp(argv[n],"-port"))
        {
            port = atoi(argv[n + 1]);
            printf("\nGiven Port: %d\n", port);
            n = n + 2;
            isStandAlone = 1;
        }
        else
        {
            printError();
        }
    }

    //this handler is added for ctrl+c signal
    signal(SIGINT, sigint);

    result = ezmqxCreateConfig(&g_configHandle);
    if (result != CEZMQX_OK)
    {
        printf("\nCreate config failed [Result] : %d\n", result);
        return -1;
    }
    if (1 == isStandAlone)
    {
        result = ezmqxStartStandAloneMode(g_configHandle, "localhost", 0, "");
        if(result != CEZMQX_OK)
        {
            printf("\nStart stand alone mode failed [Result]: %d\n", result);
            return -1;
        }
    }
    else
    {
        const char *tnsConfPath = "tnsConf.json";
        result = ezmqxStartDockerMode(g_configHandle, tnsConfPath);
        if(result != CEZMQX_OK)
        {
            printf("\nStart docker mode failed [Result]: %d\n", result);
            return -1;
        }
    }

    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    result = ezmqxAddAmlModel(g_configHandle, amlPath, 1, &idArr, &arrsize);
    if(result != CEZMQX_OK)
    {
        printf("\nAdd AmlModel failed [Result]: %d\n", result);
        return -1;
    }
    for (size_t i = 0; i< arrsize; i++)
    {
        printf("\nId is %s\n", idArr[i]);
    }

    ezmqxGetAMLPublisher(topic, AmlModelId, idArr[0], port, &g_pubHandle);
    g_isStarted = 1;

    if (1 == isStandAlone)
    {
        publishData(15);
    }
    else
    {
        publishData(100000);
    }

    result = ezmqxAMLPubTerminate(g_pubHandle);
    if(result != CEZMQX_OK)
    {
        printf("\nPublihser terminate failed [Result]: %d\n", result);
        return -1;
    }
    result = ezmqxReset(g_configHandle);
    if(result != CEZMQX_OK)
    {
        printf("\nReset config failed [Result]: %d\n", result);
        return -1;
    }
    printf("\nReset config done: [Result]: %d\n", result);
    return 0;
}

