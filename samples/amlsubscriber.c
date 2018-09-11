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
#include "cezmqxamlsubscriber.h"
#include "cezmqxtopicdiscovery.h"

#define UNUSED(x) (void)(x)

int g_isStarted = 0;
pthread_cond_t g_cv;
pthread_mutex_t g_mutex;
pthread_condattr_t g_cattr;

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
    printf("\n     ./amlsubscriber -ip 192.168.1.1 -port 5562 -t /topic\n");
    printf("\n  (2) For running in standalone mode [Secured]: ");
    printf("\n     ./amlsubscriber -ip 192.168.1.1 -port 5562 -t /topic -secured 1\n");
    printf("\n  (3) For running in standalone mode [With TNS]: ");
    printf("\n     ./amlsubscriber -t /topic -tns 192.168.10.1\n");
    printf("\n  (4) For running in standalone mode [With TNS + Secured]: ");
    printf("\n     ./amlsubscriber -t /topic -tns 192.168.10.1 -secured 1\n");
    printf("\n  (5)  For running in docker mode: ");
    printf("\n     ./amlsubscriber -t /topic -h 0\n");
    printf("\n  (6)  For running in docker mode [Secured]: ");
    printf("\n     ./amlsubscriber -t /topic -h 0 -secured 1\n");
    printf("\n Note:");
    printf("\n (1) -h stands for hierarchical search for topic from TNS server [0: true 1:false]");
    printf("\n (2) docker mode will work only when sample is running in docker container\n");
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
    ezmqxAMLSubHandle_t subHandle;
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
                result = ezmqxGetAMLSubscriber1(topicHandle, amlSubCB, amlSubErrCB, &subHandle);
            }
            else
            {
                result = ezmqxGetAMLSubscriber(topic, 1, amlSubCB, amlSubErrCB, &subHandle);
            }
        }
        else
        {
            if(NULL == tnsAddr)
            {
                ezmqxTopicHandle_t topicHandle;
                ezmqxCreateTopic1(topic, idArr[0], 1, endpoint, &topicHandle);
                result = ezmqxGetSecuredAMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                        clientSecretKey, amlSubCB, amlSubErrCB, &subHandle);
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
                ezmqxTopicHandle_t topicHandle;
                ezmqxQuery(tdHandle, topic, &topicHandle);
                result = ezmqxGetSecuredAMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                        clientSecretKey, amlSubCB, amlSubErrCB, &subHandle);
            }
        }
    }
    else
    {
        if(0 == isSecured)
        {
            result = ezmqxGetAMLSubscriber(topic, isHierarchical, amlSubCB, amlSubErrCB, &subHandle);
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
            ezmqxTopicHandle_t topicHandle;
            ezmqxQuery(tdHandle, topic, &topicHandle);
            result = ezmqxGetSecuredAMLSubscriber(topicHandle, serverPublicKey, clientPublicKey,
                                                                    clientSecretKey, amlSubCB, amlSubErrCB, &subHandle);
        }
    }
    if(result != CEZMQX_OK)
    {
        printf("\nGet AML Subscriber failed [Result]: %d\n", result);
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
    result = ezmqxAMLSubTerminate(subHandle);
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

