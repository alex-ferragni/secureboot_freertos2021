/**
  ******************************************************************************
  * @file    rcc.h
  * @author  CSEM SA
  * @brief   Header file of RCC module.
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

#ifndef __DRIVERS_RCC_H__
#define __DRIVERS_RCC_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_RCC_BASE                            0x58024400
#define HAL_RCC_AHB2ENR_OFFSET                  0x0DC
#define HAL_RCC_AHB4ENR_OFFSET                  0x0E0
#define HAL_RCC_APB1LENR_OFFSET                 0x0E8

#define HAL_RCC_AHB2ENR_REG                     REG_CONTENT(HAL_RCC_BASE, HAL_RCC_AHB2ENR_OFFSET)
#define HAL_RCC_AHB4ENR_REG                     REG_CONTENT(HAL_RCC_BASE, HAL_RCC_AHB4ENR_OFFSET)
#define HAL_RCC_APB1LENR_REG                 	REG_CONTENT(HAL_RCC_BASE, HAL_RCC_APB1LENR_OFFSET)

#define HAL_RCC_AHB2ENR_CRYPTEN_POS				4
#define HAL_RCC_AHB2ENR_CRYPTEN					(1 << HAL_RCC_AHB2ENR_CRYPTEN_POS)
#define HAL_RCC_AHB2ENR_HASHEN_POS				5
#define HAL_RCC_AHB2ENR_HASHEN					(1 << HAL_RCC_AHB2ENR_HASHEN_POS)

#define HAL_RCC_AHB4ENR_GPIOAEN_POS				0
#define HAL_RCC_AHB4ENR_GPIOAEN					(1 << HAL_RCC_AHB4ENR_GPIOAEN_POS)
#define HAL_RCC_AHB4ENR_GPIOBEN_POS				1
#define HAL_RCC_AHB4ENR_GPIOBEN					(1 << HAL_RCC_AHB4ENR_GPIOBEN_POS)
#define HAL_RCC_AHB4ENR_GPIOCEN_POS				2
#define HAL_RCC_AHB4ENR_GPIOCEN					(1 << HAL_RCC_AHB4ENR_GPIOCEN_POS)
#define HAL_RCC_AHB4ENR_GPIODEN_POS				3
#define HAL_RCC_AHB4ENR_GPIODEN					(1 << HAL_RCC_AHB4ENR_GPIODEN_POS)
#define HAL_RCC_AHB4ENR_GPIOEEN_POS				4
#define HAL_RCC_AHB4ENR_GPIOEEN					(1 << HAL_RCC_AHB4ENR_GPIOEEN_POS)


#define HAL_RCC_APB1LENR_TIM6EN_POS			    4
#define HAL_RCC_APB1LENR_TIM6EN				    (1 << HAL_RCC_APB1LENR_TIM6EN_POS)
#define HAL_RCC_APB1LENR_USART3EN_POS			18
#define HAL_RCC_APB1LENR_USART3EN				(1 << HAL_RCC_APB1LENR_USART3EN_POS)

/* -------------------- INTERFACE -------------------- */

// Speed of the default internal core clock, when PLL are not modified
#define HAL_RCC_DEFAULT_CLK_INT             64000000

#define ENABLE_CLK_ABCDE()                   HAL_RCC_AHB4ENR_REG |= (HAL_RCC_AHB4ENR_GPIOAEN | HAL_RCC_AHB4ENR_GPIOBEN | HAL_RCC_AHB4ENR_GPIOCEN | HAL_RCC_AHB4ENR_GPIODEN | HAL_RCC_AHB4ENR_GPIOEEN)
#define DISABLE_CLK_ABCDE()                  HAL_RCC_AHB4ENR_REG &= (~(HAL_RCC_AHB4ENR_GPIOAEN | HAL_RCC_AHB4ENR_GPIOBEN | HAL_RCC_AHB4ENR_GPIOCEN | HAL_RCC_AHB4ENR_GPIODEN | HAL_RCC_AHB4ENR_GPIOEEN))


#define ENABLE_CLK_TIM()                    BIT_SET(HAL_RCC_APB1LENR_REG, HAL_RCC_APB1LENR_TIM6EN_POS)
#define DISABLE_CLK_TIM()                   BIT_CLEAR(HAL_RCC_APB1LENR_REG, HAL_RCC_APB1LENR_TIM6EN_POS)
#define ENABLE_CLK_USART()                  BIT_SET(HAL_RCC_APB1LENR_REG, HAL_RCC_APB1LENR_USART3EN_POS)
#define DISABLE_CLK_USART()                 BIT_CLEAR(HAL_RCC_APB1LENR_REG, HAL_RCC_APB1LENR_USART3EN_POS)

// Cryp RCC will be enabled only when needed, by MCUboot calling the cryp init function
// But it does not have a deinit function, so deinitialize cryp here
#define HAL_RCC_ENABLE_CRYPT()				BIT_SET(HAL_RCC_AHB2ENR_REG,HAL_RCC_AHB2ENR_CRYPTEN_POS)
#define HAL_RCC_DISABLE_CRYPT()				BIT_CLEAR(HAL_RCC_AHB2ENR_REG,HAL_RCC_AHB2ENR_CRYPTEN_POS)

#define HAL_RCC_INIT()                      ENABLE_CLK_ABCDE()
#define HAL_RCC_DEINIT()                    do { \
												DISABLE_CLK_ABCDE(); \
												HAL_RCC_DISABLE_CRYPT(); \
											} while (0)

#endif /* __DRIVERS_RCC_H__ */