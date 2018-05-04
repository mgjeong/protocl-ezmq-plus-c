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
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include "camlinterface.h"
#include "camlrepresentation.h"

#include "cezmqxconfig.h"
#include "cezmqxamlpublisher.h"
#include "cezmqxendpoint.h"
#include "cezmqxtopic.h"
#include "cezmqxamlsubscriber.h"

#define UNUSED(x) (void)(x)

int g_isStarted = 0;
pthread_cond_t g_cv;
pthread_mutex_t g_mutex;
pthread_condattr_t g_cattr;

void sigint(int signal)
{
    printf("\nInterupt signal:  %d\n", signal);
    if (g_isStarted)
    {
        //signal all condition variables
        pthread_mutex_lock(&g_mutex);
        pthread_cond_broadcast(&g_cv);
        pthread_mutex_unlock(&g_mutex);
    }
    else
    {
        exit(0);
    }
}

void freeCharArr(char** str, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(str[i]);
    }
    free(str);
}

void printAMLData(amlDataHandle_t amlData, int depth)
{
    char* indent = (char*)malloc(sizeof(char)*100);
    strncpy(indent, "", 1);
    for (int i = 0; i < depth; i++) strcat(indent, "    ");

    printf("%s{\n", indent);

    char** keys = NULL;
    size_t size = 0;
    AMLData_GetKeys(amlData, &keys, &size);

    for (size_t i = 0; i < size; i++)
    {
        printf("%s    \"%s\" : ", indent, keys[i]);

        CAMLValueType valType = AMLVALTYPE_STRING;
        AMLData_GetValueType(amlData, keys[i], &valType);
        if (AMLVALTYPE_STRING == valType)
        {
            char* valStr;
            AMLData_GetValueStr(amlData, keys[i], &valStr);
            printf("%s", valStr);

            free(valStr);
        }
        else if (AMLVALTYPE_STRINGARRAY == valType)
        {
            char** valStrArr;
            size_t arrsize;
            AMLData_GetValueStrArr(amlData, keys[i], &valStrArr, &arrsize);
            printf("[");
            for (size_t j = 0; j < arrsize; j++)
            {
                printf("%s", valStrArr[j]);
                if (j != arrsize - 1) printf(", ");
            }
            printf("]");

            freeCharArr(valStrArr, arrsize);
        }
        else if (AMLVALTYPE_AMLDATA == valType)
        {
            amlDataHandle_t valAMLData;
            AMLData_GetValueAMLData(amlData, keys[i], &valAMLData);
            printf("\n");
            printAMLData(valAMLData, depth + 1);
        }

        if (i != size - 1)  printf(",");
        printf("\n");
    }
    printf("%s}", indent);

    free(indent);
    freeCharArr(keys, size);
}

void printAMLObject(amlObjectHandle_t amlObj)
{
    char* deviceId, *timeStamp, *id;
    AMLObject_GetDeviceId(amlObj, &deviceId);
    AMLObject_GetTimeStamp(amlObj, &timeStamp);
    AMLObject_GetId(amlObj, &id);

    printf("{\n");
    printf("    \"device\" : %s,\n", deviceId);
    printf("    \"timeStamp\" : %s,\n", timeStamp);
    printf("    \"id\" : %s,\n", id);

    free(deviceId);
    free(timeStamp);
    free(id);

    char** dataNames;
    size_t size;
    AMLObject_GetDataNames(amlObj, &dataNames, &size);

    for (size_t i = 0; i < size; i++)
    {
        amlDataHandle_t data;
        AMLObject_GetData(amlObj, dataNames[i], &data);

        printf("    \"%s\" : \n", dataNames[i]);
        printAMLData(data, 1);

        if (i != size - 1) printf(",\n");
    }

    printf("\n}\n");

    freeCharArr(dataNames, size);
}

void amlSubCB(const char * topic, amlObjectHandle_t amlObject)
{
    printf("\nAppication callback [amlSubCB]: \n");
    fflush(stdout);
    printf("\nTopic: %s\n\n", topic);
    fflush(stdout);
    printAMLObject(amlObject);
}

void amlSubErrCB(const char * topic, CEZMQXErrorCode errCode)
{
    printf("\nAppication callback [amlSubErrCB]");
    fflush(stdout);
    printf("\nTopic: %s\n", topic);
    fflush(stdout);
    printf("\nError code: %d", errCode);
}

int main()
{
    printf("\nEnter topic ex) /test ");
    char topic[50];
    char  *fget = fgets(topic, 50, stdin);
    UNUSED(fget);
    char *newline = strchr(topic, '\n'); // search for newline character
    if ( newline != NULL )
    {
        *newline = '\0'; // overwrite trailing newline
    }

    ezmqxConfigHandle_t config;
    ezmqxCreateConfig(StandAlone, &config);
    printf("\nCreate config done..");
    //ezmqxSetHostInfo(config, "Lolcahost", "hostAddr");
    //ezmqxSetTnsInfo(config, "remoteAddr");

    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    ezmqxAddAmlModel(config, amlPath, 1, &idArr, &arrsize);
    for (size_t i = 0; i< arrsize; i++)
    {
        printf("\nid is %s\n", idArr[i]);
    }

    ezmqxAMLSubHandle_t subHandle;
    ezmqxEPHandle_t endpoint;
    ezmqxCreateEndPoint2("localhost", 4000, &endpoint);
    ezmqxTopicHandle_t topicHandle;
    ezmqxCreateTopic1(topic, idArr[0], endpoint, &topicHandle);
    ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle);
    printf("\nSuscribed to publisher.. -- Waiting for Events --\n");

    g_isStarted = 1;
    // initialize a condition variable to its default value
    pthread_cond_init(&g_cv, NULL);
    //initialize a condition variable
    pthread_cond_init(&g_cv, &g_cattr);
    //this handler is added to check ctrl+c
    signal(SIGINT, sigint);

    // conditional wait to prevent main loop from exit
    pthread_mutex_lock(&g_mutex);
    pthread_cond_wait(&g_cv, &g_mutex);
    pthread_mutex_unlock(&g_mutex);

    //Terminate subscriber
    ezmqxAMLTerminate(subHandle);
    printf("\nClosing application..\n");
}

