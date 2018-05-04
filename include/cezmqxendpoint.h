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

#ifndef __CEZMQ_X_EP_H__
#define __CEZMQ_X_EP_H__

#include "cezmqxerrorcodes.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * End Point handle
 */
typedef void * ezmqxEPHandle_t;

/**
 *  Create end point.
 *
 * @param handle -Handle will be filled as return value.
 *
 * @note EndPoint instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyEndPoint().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateEndPoint(ezmqxEPHandle_t *handle);

/**
 *  Create end point with given address.
 *
 * @param address -IP Address.
 * @param handle -Handle will be filled as return value.
 *
 * @note EndPoint instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyEndPoint().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateEndPoint1(const char *address, ezmqxEPHandle_t *handle);

/**
 *  Create end point with given address and port.
 *
 * @param address -IP Address.
 * @param port -port.
 * @param handle -Handle will be filled as return value.
 *
 * @note EndPoint instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyEndPoint().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateEndPoint2(const char *address, int port, ezmqxEPHandle_t *handle);

/**
 *  Destroy an instance of end point.
 *
 * @param handle -EndPoint Handle that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyEndPoint(ezmqxEPHandle_t handle);

/**
 *  Get address for given end point.
 *
 * @param handle -EndPoint handle.
 * @param address - Address will be filled as return value.
 *
 * @note Address will be allocated, so it should be deleted after use.
 *              To destroy address : free(address);
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetAddr(ezmqxEPHandle_t handle, char **address);

/**
 *  Get port for given end point.
 *
 * @param handle -EndPoint handle.
 * @param port - Port will be filled as return value.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxGetPort(ezmqxEPHandle_t handle, int *port);

/**
 *  Get string representation for given end point.
 *
 * @param handle -EndPoint handle.
 * @param stringValue - stringValue will be filled as return value.
 *
 * @note stringValue will be allocated, so it should be deleted after use.
 *              To destroy stringValue : free(stringValue);
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxToString(ezmqxEPHandle_t handle, char **stringValue);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_EP_H__

