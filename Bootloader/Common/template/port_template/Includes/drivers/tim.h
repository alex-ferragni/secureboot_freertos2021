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

#ifndef __DRIVERS_TIM_H__
#define __DRIVERS_TIM_H__

#include "hal/hal_util.h"

/* -------------------- INTERFACE -------------------- */

// Initialization of the TIM unit, called upon bootloader start
#define HAL_TIM_INIT()                      PASS
// Deinitialization of the TIM unit, called before jumping to image
#define HAL_TIM_DEINIT()                    PASS

// Get the current absolute time, in milliseconds
// Not strictly necessary, if you don't want to implement it (or cannot),
// Simply set it to '0', the bootloader will ignore it
#define HAL_TIM_GET_TIME_MS()				0
// Get the difference between two times, in milliseconds
// (Might not be a straight-forward substraction in case we reach the maximum range)
#define HAL_TIM_ELAPSED_TIME(tim0, tim1)	0

#endif /* __DRIVERS_TIM_H__ */