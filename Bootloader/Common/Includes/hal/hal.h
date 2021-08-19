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

#ifndef __HAL_H__
#define __HAL_H__

#include <stdint.h>
#include "hal/hal_flash.h"
#include "hal/hal_gpio.h"
#include "hal/hal_rcc.h"
#include "hal/hal_util.h"
#include "hal/hal_fpu.h"
#include "hal/hal_scb.h"
#include "hal/hal_uart.h"
#include "hal/hal_tim.h"

/**
 * @brief Initializes all included modules
 */
#define HAL_INIT()          HAL_init()

/**
 * @brief Deinitializes all included modules
 */
#define HAL_DEINIT()        HAL_deinit()

/**
 * @brief Initializes all included modules
 */
static inline void HAL_init(void){

#ifdef HAL_INCLUDE_RCC
	HAL_RCC_INIT();
#endif /* HAL_INCLUDE_RCC */
    
#ifdef HAL_INCLUDE_GPIO
	HAL_GPIO_INIT();
#endif /* HAL_INCLUDE_GPIO */
    
#ifdef HAL_INCLUDE_FPU
	HAL_FPU_INIT();
#endif /* HAL_INCLUDE_FPU */
    
#ifdef HAL_INCLUDE_UART
	HAL_UART_INIT();
#endif /* HAL_INCLUDE_UART */
    
#ifdef HAL_INCLUDE_TIM
	HAL_TIM_INIT();
#endif /* HAL_INCLUDE_TIM */

}

/**
 * @brief Deinitializes all included modules
 */
static inline void HAL_deinit(void){
    
#ifdef HAL_INCLUDE_TIM
	HAL_TIM_DEINIT();
#endif /* HAL_INCLUDE_TIM */
    
#ifdef HAL_INCLUDE_UART
	HAL_UART_DEINIT();
#endif /* HAL_INCLUDE_UART */
    
#ifdef HAL_INCLUDE_FPU
	HAL_FPU_DEINIT();
#endif /* HAL_INCLUDE_FPU */
    
#ifdef HAL_INCLUDE_GPIO
	HAL_GPIO_DEINIT();
#endif /* HAL_INCLUDE_GPIO */

#ifdef HAL_INCLUDE_RCC
	HAL_RCC_DEINIT();
#endif /* HAL_INCLUDE_RCC */

}

#endif /* __HAL_H__ */