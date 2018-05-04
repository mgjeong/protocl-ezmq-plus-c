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
* @enum CModeOption
* CEZMQX mode types.
*/
typedef enum
{
    StandAlone = 0,
    Docker
}CModeOption;

/**
 *  Create ezmqx configuration with given mode.
 *
 * @param mode - Config mode [StandAlone/Docker].
 * @param handle - Handle will be filled as return value.
 *
 * @note Config instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use ezmqxDestroyConfig().
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxCreateConfig(CModeOption mode, ezmqxConfigHandle_t *handle);

/**
 *  Destroy an instance of config.
 *
 * @param handle - Config Handle that will be destroyed.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxDestroyConfig(ezmqxConfigHandle_t handle);

/**
 *  Set host info [name and address].
 *
 * @param handle  -Config handle.
 * @param hostName - Host name.
 * @param hostAddr - Host address.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxSetHostInfo(ezmqxConfigHandle_t handle, const char *hostName,
        const char *hostAddr);

/**
 *  Set TNS info [server address].
 *
 * @param handle  -Config handle.
 * @param remoteAddr - TNS server address.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxSetTnsInfo(ezmqxConfigHandle_t handle, const char * remoteAddr);

/**
 *  Add model information to configuration.
 *
 * @param handle -Config handle.
 * @param amlFilePath - List of AML file paths.
 * @param size - List Size.
 * @param amlFilePath - List of AML ID will be filled..
 * @param listSize - ID List Size.
 *
 * @note ID list and IDs will be allocated, so it should be deleted after use.
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxAddAmlModel(ezmqxConfigHandle_t handle, const char **amlFilePath,
        const size_t size, char*** idList, size_t* listSize);

/**
 *  Reset the ezmqx configuration.
 *
 * @param handle  -Config handle.
 * @param mode - Config mode [StandAlone/Docker].
 *
 * @return CEZMQXErrorCode - CEZMQX_OK on success, otherwise appropriate error code.
 */
EZMQX_EXPORT CEZMQXErrorCode ezmqxReset(ezmqxConfigHandle_t handle, CModeOption mode);

#ifdef __cplusplus
}
#endif

#endif // __CEZMQ_X_CONFIG_H__

