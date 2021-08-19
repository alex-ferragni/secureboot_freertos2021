/**
  ******************************************************************************
  * @file    tim.h
  * @author  CSEM SA
  * @brief   Header file of TIM module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#ifndef __DRIVERS_TIM_H__
#define __DRIVERS_TIM_H__

#include "hal/hal_util.h"
#include "hal/hal_rcc.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_TIM6_BASE							0x40001000

#define HAL_TIM_CR1_OFFSET						0x0
#define HAL_TIM_CNT_OFFSET						0x24
#define HAL_TIM_PSC_OFFSET						0x28

#define HAL_TIM6_CR1_REG						REG_CONTENT(HAL_TIM6_BASE, HAL_TIM_CR1_OFFSET)
#define HAL_TIM6_CNT_REG						REG_CONTENT(HAL_TIM6_BASE, HAL_TIM_CNT_OFFSET)
#define HAL_TIM6_PSC_REG						REG_CONTENT(HAL_TIM6_BASE, HAL_TIM_PSC_OFFSET)

#define HAL_TIM_CR1_CEN_POS						0
#define HAL_TIM_CR1_CEN							(1 << HAL_TIM_CR1_CEN_POS)

#define HAL_TIM_CNT_CNT_POS						0
#define HAL_TIM_CNT_CNT							(0xffff << HAL_TIM_CNT_CNT_POS)


// Prescaler
#define GET_TIM_PSC(clk)						((clk / 1000) - 1)

/* -------------------- INTERFACE -------------------- */

// Default clock used by the TIM module
#define HAL_TIM_DEFAULT_CLK					RCC_DEFAULT_CLK
// Get the current absolute time, in milliseconds
// Not strictly necessary, if you don't want to implement it (or cannot),
// Simply set it to '0', the bootloader will ignore it
#define HAL_TIM_GET_TIME_MS()				(HAL_TIM6_CNT_REG & HAL_TIM_CNT_CNT)
// The time elapsed is the difference between two measures
// Be careful, the timer has a small range and can loop
// So measure cannot be bigger than the range (large enough to measure MCUboot's execution time)
#define HAL_TIM_ELAPSED_TIME(tim0, tim1)	((tim1 - tim0) & HAL_TIM_CNT_CNT)

#if GET_TIM_PSC(HAL_RCC_DEFAULT_CLK_INT) > HAL_TIM_CNT_CNT
#error "Cannot configure timer prescaler, value would overflow!"
#endif

#define HAL_TIM_INIT()						do { \
												ENABLE_CLK_TIM(); \
												BIT_SET(HAL_TIM6_CR1_REG, HAL_TIM_CR1_CEN_POS); \
												REG_WRITE(HAL_TIM6_PSC_REG, GET_TIM_PSC(HAL_RCC_DEFAULT_CLK_INT));\
											} while (0)
											
#define HAL_TIM_DEINIT()					DISABLE_CLK_TIM()

#endif /* __DRIVERS_TIM_H__ */