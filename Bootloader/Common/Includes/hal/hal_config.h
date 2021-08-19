/*
 *    Copyright 2021 CSEM SA
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
*/

#ifndef __HAL_CONFIG_H__
#define __HAL_CONFIG_H__

// Application & Bootloader specific configuration file
// Shortcut to include all drivers in case the HAL_INCLUDE_ALL variable is defined
// Included by every driver

#ifdef HAL_INCLUDE_ALL

#define HAL_INCLUDE_FLASH
#define HAL_INCLUDE_GPIO
#define HAL_INCLUDE_RCC
#define HAL_INCLUDE_FPU
#define HAL_INCLUDE_SCB
#define HAL_INCLUDE_UART
#define HAL_INCLUDE_TIM
#define HAL_INCLUDE_SECURITY
#define HAL_INCLUDE_CRYP
#define HAL_INCLUDE_HASH
#define HAL_INCLUDE_UTIL

#else /* HAL_INCLUDE_ALL */

#include "hal/hal_app.h"

#endif /* HAL_INCLUDE_ALL */

#endif /* __HAL_CONFIG_H__ */