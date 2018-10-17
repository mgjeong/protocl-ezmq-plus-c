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
#include "cezmqxendpoint.h"
#include "cezmqxtopic.h"
#include "cezmqxxmlsubscriber.h"
#include "cezmqxtopicdiscovery.h"

#define UNUSED(x) (void)(x)

int g_isStarted = 0;
pthread_cond_t g_cv;
pthread_mutex_t g_mutex;
pthread_condattr_t g_cattr;

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

char * getTNSAddress(const char * address)
{
    char *prefix = "http://";
    char * firstPart = (char *) malloc(1 + strlen(prefix)+ strlen(address));
    strcpy(firstPart, prefix);
    strcat(firstPart, address);
    char *postfix = ":80/tns-server";
    char * tnsAddress = (char *) malloc(1 + strlen(firstPart)+ strlen(postfix));
    strcpy(tnsAddress, firstPart);
    strcat(tnsAddress, postfix);
    return tnsAddress;
}

void printError()
{
    printf("\nRe-run the application as shown in below example: \n");
    printf("\n  (1) For running in standalone mode: ");
    printf("\n     ./xmlsubscriber -ip 192.168.1.1 -port 5562 -t /topic\n");
    printf("\n  (2) For running in standalone mode [Secured]: ");
    printf("\n     ./xmlsubscriber -ip 192.168.1.1 -port 5562 -t /topic -secured 1\n");
    printf("\n  (3) For running in standalone mode [With TNS]: ");
    printf("\n     ./xmlsubscriber -t /topic -tns 192.168.10.1\n");
    printf("\n  (4) For running in standalone mode [With TNS + Secured]: ");
    printf("\n     ./xmlsubscriber -t /topic -tns 192.168.10.1 -secured 1\n");
    printf("\n  (5)  For running in docker mode: ");
    printf("\n     ./xmlsubscriber -t /topic -h 0\n");
    printf("\n  (6)  For running in docker mode [Secured]: ");
    printf("\n     ./xmlsubscriber -t /topic -h 0 -secured 1\n");
    printf("\n Note:");
    printf("\n (1) -h stands for hierarchical search for topic from TNS server [0: true 1:false]");
    printf("\n (2) -h option will work only with TNS + unsecured mode");
    printf("\n (3) docker mode will work only when sample is running in docker container\n");
}

int main(int argc, char* argv[])
{
    CEZMQXErrorCode result;
    char *ip = NULL;
    int port = 0;
    char *topic = NULL;
    char *tnsAddr = NULL;
    int isStandAlone = 0;
    int isHierarchical = 0;
    int isSecured = 0;

    // get ip, port and topic from command line arguments
    if(argc != 5 && argc != 7 && argc!=9)
    {
        printError();
        return -1;
    }
    int n = 1;
    while (n < argc)
    {
         if (0 == strcmp(argv[n],"-ip"))
        {
            ip = argv[n + 1];
            printf("\nGiven IP is : %s", ip);
            n = n + 2;
            isStandAlone = 1;
        }
        else if (0 == strcmp(argv[n],"-t"))
        {
            topic = argv[n + 1];
            printf("\nGiven Topic is : %s", topic);
            n = n + 2;
        }
        else if (0 == strcmp(argv[n],"-port"))
        {
            port = atoi(argv[n + 1]);
            printf("\nGiven Port: %d", port);
            n = n + 2;
        }
        else if (0 == strcmp(argv[n],"-h"))
        {
            isHierarchical = atoi(argv[n + 1]);
            printf("\nIs hierarchical : %d", isHierarchical);
            n = n + 2;
        }
        else if (0 == strcmp(argv[n],"-tns"))
        {
            tnsAddr = argv[n + 1];
            printf("\nTNS address: %s\n", tnsAddr);
            n = n + 2;
            isStandAlone = 1;
        }
        else if (0 == strcmp(argv[n],"-secured"))
        {
            isSecured = atoi(argv[n + 1]);
            printf("\nIs secured: %d\n", isSecured);
            n = n + 2;
        }
        else
        {
            printError();
        }
    }

    //this handler is added for ctrl+c signal
    signal(SIGINT, sigint);

    ezmqxConfigHandle_t configHandle;
    result = ezmqxCreateConfig(&configHandle);
    if (result != CEZMQX_OK)
    {
        printf("\nCreate config failed [Result] : %d\n", result);
        return -1;
    }
    if (1 == isStandAlone)
    {
        if(NULL == tnsAddr)
        {
            result = ezmqxStartStandAloneMode(configHandle, "localhost", 0, "");
        }
        else
        {
            char *tnsAddress = getTNSAddress(tnsAddr);
            printf("\nComplete TNS address: %s\n", tnsAddress);
            result = ezmqxStartStandAloneMode(configHandle, "", 1, tnsAddress);
        }
        if(result != CEZMQX_OK)
        {
            printf("\nStart stand alone mode failed [Result]: %d\n", result);
            return -1;
        }
    }
    else
    {
        const char *tnsConfPath = "tnsConf.json";
        result = ezmqxStartDockerMode(configHandle, tnsConfPath);
        if(result != CEZMQX_OK)
        {
            printf("\nStart docker mode failed [Result]: %d\n", result);
            return -1;
        }
    }

    const char* amlPath[1] = {"sample_data_model.aml"};
    char** idArr;
    size_t arrsize;
    result = ezmqxAddAmlModel(configHandle, amlPath, 1, &idArr, &arrsize);
    if(result != CEZMQX_OK)
    {
        printf("\nAdd AmlModel failed [Result]: %d\n", result);
        return -1;
    }
    for (size_t i = 0; i< arrsize; i++)
    {
        printf("\nid is %s\n", idArr[i]);
    }

    const char *serverPublicKey = "tXJx&1^QE2g7WCXbF.$$TVP.wCtxwNhR8?iLi&S<";
    const char *clientPublicKey = "-QW?Ved(f:<::3d5tJ$[4Er&]6#9yr=vha/caBc(";
    const char *clientSecretKey = "ZB1@RS6Kv^zucova$kH(!o>tZCQ.<!Q)6-0aWFmW";
    ezmqxXMLSubHandle_t subHandle;
    ezmqxEPHandle_t endpoint;
    ezmqxCreateEndPoint2(ip, port, &endpoint);
    if (1 == isStandAlone)
    {
        if(0 == isSecured)
        {
            if(NULL == tnsAddr)
            {
                ezmqxTopicHandle_t topicHandle;
                ezmqxCreateTopic1(topic, idArr[0], 0, endpoint, &topicHandle);
                result = ezmqxGetXMLSubscriber1(topicHandle, xmlSubCB, xmlSubErrCB, &subHandle);
            }
            else
            {
                result = ezmqxGetXMLSubscriber(topic, isHierarchical, xmlSubCB, xmlSubErrCB, &subHandle);
            }
        }
        else
        {
            if(NULL == tnsAddr)
            {
                ezmqxTopicHandle_t topicHandle;
                ezmqxCreateTopic1(topic, idArr[0], 1, endpoint, &topicHandle);
                result = ezmqxGetSecuredXMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                        clientSecretKey, xmlSubCB, xmlSubErrCB, &subHandle);
            }
            else
            {
                ezmqxTDiscoveryHandle_t tdHandle;
                result = ezmqxCreateTopicDiscovery(&tdHandle);
                if(result != CEZMQX_OK)
                {
                    printf("\nCreate Topic Discovery failed [Result]: %d", result);
                    return -1;
                }
                ezmqxTopicHandle_t topicHandle = NULL;
                ezmqxQuery(tdHandle, topic, &topicHandle);
                if(NULL == topicHandle)
                {
                    printf("\nCouldn't find the topic");
                    return -1;
                }
                result = ezmqxGetSecuredXMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                        clientSecretKey, xmlSubCB, xmlSubErrCB, &subHandle);
            }
        }
    }
    else
    {
        if(0 == isSecured)
        {
            result = ezmqxGetXMLSubscriber(topic, isHierarchical, xmlSubCB, xmlSubErrCB, &subHandle);
        }
        else
        {
            ezmqxTDiscoveryHandle_t tdHandle;
            result = ezmqxCreateTopicDiscovery(&tdHandle);
            if(result != CEZMQX_OK)
            {
                printf("\nCreate Topic Discovery failed [Result]: %d", result);
                return -1;
            }
            ezmqxTopicHandle_t topicHandle = NULL;
            ezmqxQuery(tdHandle, topic, &topicHandle);
            if(NULL == topicHandle)
            {
                printf("\nCouldn't find the topic");
                return -1;
            }
            result = ezmqxGetSecuredXMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                    clientSecretKey, xmlSubCB, xmlSubErrCB, &subHandle);
        }
    }
    if(result != CEZMQX_OK)
    {
        printf("\nGet XML Subscriber failed [Result]: %d\n", result);
        return -1;
    }
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
    result = ezmqxXMLSubTerminate(subHandle);
    if(result != CEZMQX_OK)
    {
        printf("\nTerminate Subscriber failed [Result]: %d\n", result);
        return -1;
    }
    //Reset config
    result = ezmqxReset(configHandle);
    if(result != CEZMQX_OK)
    {
        printf("\nReset config failed [Result]: %d\n", result);
        return -1;
    }
}

