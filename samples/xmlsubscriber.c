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
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>

#include "cezmqxconfig.h"
#include "cezmqxamlpublisher.h"
#include "cezmqxendpoint.h"
#include "cezmqxtopic.h"
#include "cezmqxxmlsubscriber.h"

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

void xmlSubCB(const char * topic, const char * payload)
{
    printf("\nAppication callback [xmlSubCB]: \n");
    fflush(stdout);
    printf("\nTopic: %s", topic);
    fflush(stdout);
    printf("\nPayload:\n %s", payload);
}

void xmlSubErrCB(const char * topic, CEZMQXErrorCode errCode)
{
    printf("\nAppication callback [xmlSubErrCB]");
    fflush(stdout);
    printf("\nTopic: %s", topic);
    fflush(stdout);
    printf("\nError code: %d", errCode);
}

int main()
{
    printf("\nEnter topic ex) /test ");
    char topic[50];
    char *fget = fgets(topic, 50, stdin);
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

    ezmqxXMLSubHandle_t subHandle;
    ezmqxEPHandle_t endpoint;
    ezmqxCreateEndPoint2("localhost", 4000, &endpoint);
    ezmqxTopicHandle_t topicHandle;
    ezmqxCreateTopic1(topic, idArr[0], endpoint, &topicHandle);
    ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle);
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
    ezmqxXMLTerminate(subHandle);
    printf("\nClosing application..\n");
}

