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

#ifndef __DRIVERS_RCC_H__
#define __DRIVERS_RCC_H__

#include "hal/hal_util.h"

/* -------------------- INTERFACE -------------------- */

// Initialization of the GPIO unit, called upon bootloader start
#define HAL_RCC_INIT()                      PASS
// Deinitialization of the GPIO unit, called before jumping to image
#define HAL_RCC_DEINIT()                    PASS

#endif /* __DRIVERS_RCC_H__ */