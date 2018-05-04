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

#include <string.h>

#include "EZMQXEndpoint.h"

#include "cezmqxendpoint.h"
#include "cezmqxutils.h"

using namespace std;
using namespace EZMQX;

CEZMQXErrorCode ezmqxCreateEndPoint(ezmqxEPHandle_t *handle)
{
    VERIFY_NON_NULL(handle);
    *handle  =  new(std::nothrow) Endpoint();
     return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxCreateEndPoint1(const char *address, ezmqxEPHandle_t *handle)
{
    VERIFY_NON_NULL(address);
    VERIFY_NON_NULL(handle);
    *handle =  new(std::nothrow) Endpoint(address);
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxCreateEndPoint2(const char *address, int port, ezmqxEPHandle_t *handle)
{
    VERIFY_NON_NULL(address);
    VERIFY_NON_NULL(handle);
    *handle =  new(std::nothrow) Endpoint(address, port);
     return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxDestroyEndPoint(ezmqxEPHandle_t handle)
{
    VERIFY_NON_NULL(handle);
    Endpoint* endPoint = static_cast<Endpoint *>(handle);
    delete endPoint;
    handle = NULL;
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetAddr(ezmqxEPHandle_t handle, char **address)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(address)
    Endpoint *endPoint = static_cast<Endpoint *>(handle);
    *address = ConvertStringToCharStr(endPoint->getAddr());
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxGetPort(ezmqxEPHandle_t handle, int *port)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(port)
    Endpoint *endPoint = static_cast<Endpoint *>(handle);
    *port = endPoint->getPort();
    return CEZMQX_OK;
}

CEZMQXErrorCode ezmqxToString(ezmqxEPHandle_t handle, char **stringValue)
{
    VERIFY_NON_NULL(handle)
    VERIFY_NON_NULL(stringValue)
    Endpoint *endPoint = static_cast<Endpoint *>(handle);
    *stringValue = ConvertStringToCharStr(endPoint->toString());
    return CEZMQX_OK;
}

