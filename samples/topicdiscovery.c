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

#include "cezmqxconfig.h"
#include "cezmqxtopicdiscovery.h"

#define UNUSED(x) (void)(x)

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
    //CEZMQXErrorCode result = ezmqxCreateConfig(Docker, &config);
    CEZMQXErrorCode result = ezmqxCreateConfig(StandAlone, &config);
    printf("\nCreate config [Result] : %d ", result);
    ezmqxSetTnsInfo(config, "10.113.66.234");

    ezmqxTDiscoveryHandle_t tdHandle;
    ezmqxCreateTopicDiscovery(&tdHandle);

    ezmqxTopicHandle_t* topicList;
    size_t listSize = 0;
    result = ezmqxQuery(tdHandle, topic, &topicList, &listSize);
    if(CEZMQX_OK != result)
    {
        printf("\nTopic discovery Query [Result]: %d", result);
        return -1;
    }
    for (size_t i = 0; i< listSize; i++)
    {
        ezmqxTopicHandle_t topicHandle = topicList[i];

        char *topic;
        ezmqxGetTopic(topicHandle, &topic);
        fflush(stdout);
        printf("\nTopic is : %s\n", topic);
        char *schema;
        ezmqxGetSchema(topicHandle, &schema);
         fflush(stdout);
        printf("\nSchema is : %s\n", schema);

        ezmqxEPHandle_t epHandle;
        ezmqxGetEndpoint(topicHandle, &epHandle);
        char *toStr;
        ezmqxToString(epHandle, &toStr);
         fflush(stdout);
        printf("\nEndpoint is : %s\n", toStr);
    }
}

