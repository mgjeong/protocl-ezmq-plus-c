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

#ifndef __CEZMQ_X_CONFIG_H__
#define __CEZMQ_X_CONFIG_H__

#include "cezmqxerrorcodes.h"

#define EZMQX_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Config handle
 */
typedef void * ezmqxConfigHandle_t;

/**
 *  Create ezmqx configuration instance.
 *
 * @param handle - [out] Handle will be filled as return value.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateConfig(ezmqxConfigHandle_t *handle);

/**
 * Start as DockerMode that working with Pharos system.
 * In DockerMode, stack automatically using Tns service.
 *
 * @param handle - [in] Config Handle.
 * @param tnsConfPath - [in] path to tns configuration file.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxStartDockerMode(ezmqxConfigHandle_t handle,
    const char *tnsConfPath);

/**
 *  Start as StandAloneMode that working without pharos system.
 *
 * @param handle - [in] Config Handle.
 * @param hostAddr - [in] Ip address of host.
 * @param useTns - [in] Whether to use tns. [1: Use TNS, otherwise 0]
 * @param tnsAddr - [in] TNS address, if useTns is false this value will be ignored.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxStartStandAloneMode(ezmqxConfigHandle_t handle,
    const char *hostAddr, int useTns, const char *tnsAddr);

/**
 *  Add aml model file for publish or subscribe AML data.
 *
 * @param handle - [in] Config handle.
 * @param amlFilePath - [in] List of AML file paths.
 * @param size -  [in] List Size.
 * @param amlFilePath - [out] List of AML ID will be filled..
 * @param listSize - [out] ID List Size.
 *
 * @note ID list and IDs will be allocated, so it should be deleted after use.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAddAmlModel(ezmqxConfigHandle_t handle, const char **amlFilePath,
        const size_t size, char*** idList, size_t* listSize);

/**
 *  Reset whole EZMQX stack.
 *
 * @param handle - [in] Config handle.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxReset(ezmqxConfigHandle_t handle);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_CONFIG_H__

