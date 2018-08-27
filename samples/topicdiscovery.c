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
#include <unistd.h>

#include "cezmqxconfig.h"
#include "cezmqxtopicdiscovery.h"

#define UNUSED(x) (void)(x)

void printError()
{
    printf("\nRe-run the application as shown in below example: \n");
    printf("\n  (1) For running in standalone mode: ");
    printf("\n      ./topicdiscovery -t /topic -tns 192.168.1.1\n");
    printf("\n  (2)  For running in docker mode: ");
    printf("\n      ./topicdiscovery -t /topic \n");
}

void printTopicList(ezmqxTopicHandle_t *topicList, size_t listSize)
{
    for (size_t i = 0; i< listSize; i++)
    {
        printf("\n=================================================");
        ezmqxTopicHandle_t topicHandle = topicList[i];

        char *topic;
        ezmqxGetName(topicHandle, &topic);
        fflush(stdout);
        printf("\nTopic: %s", topic);
        char *dataModel;
        ezmqxGetDatamodel(topicHandle, &dataModel);
        fflush(stdout);
        printf("\nData Model: %s", dataModel);

        ezmqxEPHandle_t epHandle;
        ezmqxGetEndpoint(topicHandle, &epHandle);
        char *toStr;
        ezmqxToString(epHandle, &toStr);
        fflush(stdout);
        printf("\nEndpoint: %s", toStr);
        printf("\n=================================================\n");

        //Free the end point
        ezmqxDestroyEndPoint(epHandle);
    }
}

int main(int argc, char* argv[])
{
    CEZMQXErrorCode result;
    char *tnsAddress= NULL;
    char *topic= NULL;
    int isStandAlone = 0;

    // get TNS address and topic from command line arguments
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
        else if (0 == strcmp(argv[n],"-tns"))
        {
            tnsAddress = argv[n + 1];
            printf("\nGiven TNS address is : %s\n", tnsAddress);
            n = n + 2;
            isStandAlone = 1;
        }
        else
        {
            printError();
        }
    }

    ezmqxConfigHandle_t configHandle;
    result = ezmqxCreateConfig(&configHandle);
    if (result != CEZMQX_OK)
    {
        printf("\nCreate config failed [Result] : %d ", result);
        return -1;
    }
    if (1 == isStandAlone)
    {
        result = ezmqxStartStandAloneMode(configHandle, "localhost", 1, tnsAddress);
        if(result != CEZMQX_OK)
        {
            printf("\nStart stand alone mode failed [Result]: %d", result);
            return -1;
        }
    }
    else
    {
        const char *tnsConfPath = "tnsConf.json";
        result = ezmqxStartDockerMode(configHandle, tnsConfPath);
        if(result != CEZMQX_OK)
        {
            printf("\nStart docker mode failed [Result]: %d", result);
            return -1;
        }
    }

    ezmqxTDiscoveryHandle_t tdHandle;
    result = ezmqxCreateTopicDiscovery(&tdHandle);
    if(result != CEZMQX_OK)
    {
        printf("\nCreate Topic Discovery failed [Result]: %d", result);
        return -1;
    }

    ezmqxTopicHandle_t* topicList;
    size_t listSize = 0;
    result = ezmqxHierarchicalQuery(tdHandle, topic, &topicList, &listSize);
    if(result != CEZMQX_OK)
    {
        printf("\nTopic discovery Query failed [Result]: %d", result);
        return -1;
    }
    printTopicList(topicList, listSize);

    // Wait for 5 minutes before exit [For docker mode].
    if (0 == isStandAlone )
    {
        printf("Waiting for 5 minutes before program exit for docker mode... [press ctrl+c to exit]");
        sleep(300);
    }
    result = ezmqxReset(configHandle);
    printf("\nReset Config done: [Result]: %d", result);
    return 0;
}

