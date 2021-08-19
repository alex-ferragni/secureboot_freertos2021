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

#ifndef __HAL_SECURITY_H__
#define __HAL_SECURITY_H__

#include "hal/hal_config.h"

#ifdef HAL_INCLUDE_SECURITY

#include "hal/hal_util.h"
#include "drivers/security.h"

/**
 * @brief Check the current board protection (e.g., RDP & secure area on STM32H753) and increase it
 */
void security_check_and_increase_security(void);

// Only needs to appear in the context of the BIOS
#ifdef ENABLE_BIOS

/**
 * @brief Prints information related to the board protection current status
 */
void security_print_security_status(void);

#endif /* ENABLE_BIOS */

#endif /* HAL_INCLUDE_SECURITY */

#endif /* __HAL_SECURITY_H__ */