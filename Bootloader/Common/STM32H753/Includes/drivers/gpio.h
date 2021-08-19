/**
  ******************************************************************************
  * @file    gpio.h
  * @author  CSEM SA
  * @brief   Header file of GPIO module.
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

#ifndef __DRIVERS_GPIO_H__
#define __DRIVERS_GPIO_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_GPIOA_BASE                          0x58020000
#define HAL_GPIOB_BASE                          0x58020400
#define HAL_GPIOC_BASE                          0x58020800
#define HAL_GPIOD_BASE                          0x58020C00
#define HAL_GPIOE_BASE                          0x58021000

#define HAL_GPIO_MODER_OFFSET                   0x00
#define HAL_GPIO_PUPDR_OFFSET                   0x0C
#define HAL_GPIO_IDR_OFFSET                     0x10
#define HAL_GPIO_ODR_OFFSET                     0x14
#define HAL_GPIO_AFRH_LO_OFFSET					0x20
#define HAL_GPIO_AFRH_HI_OFFSET					0x24

#define GPIO_REG_CONTENT(port, register)    REG_CONTENT(HAL_GPIO##port##_BASE, HAL_GPIO_##register##_OFFSET)

#define HAL_GPIOA_MODER_REG                     GPIO_REG_CONTENT(A,MODER)

#define HAL_GPIOB_MODER_REG                     GPIO_REG_CONTENT(B,MODER)
#define HAL_GPIOB_ODR_REG                       GPIO_REG_CONTENT(B,ODR)

#define HAL_GPIOC_MODER_REG                     GPIO_REG_CONTENT(C,MODER)
#define HAL_GPIOC_PUPDR_REG                     GPIO_REG_CONTENT(C,PUPDR)
#define HAL_GPIOC_IDR_REG                       GPIO_REG_CONTENT(C,IDR)
#define HAL_GPIOC_ODR_REG                       GPIO_REG_CONTENT(C,ODR)

#define HAL_GPIOD_MODER_REG                     GPIO_REG_CONTENT(D,MODER)
#define HAL_GPIOD_AFRH_LO_REG                   GPIO_REG_CONTENT(D,AFRH_LO)
#define HAL_GPIOD_AFRH_HI_REG                   GPIO_REG_CONTENT(D,AFRH_HI)

#define HAL_GPIOE_MODER_REG                     GPIO_REG_CONTENT(E,MODER)
#define HAL_GPIOE_ODR_REG                       GPIO_REG_CONTENT(E,ODR)


/* -------------------- INTERFACE -------------------- */

#define SET_GPIO_MOD_OUT(port,pin)          HAL_GPIO##port##_MODER_REG |= (1 << (2*(pin))); \
                                            HAL_GPIO##port##_MODER_REG &= (~(1 << (2*(pin)+1)))
#define SET_GPIO_MOD_ALT(port,pin)          HAL_GPIO##port##_MODER_REG &= (~(1 << (2*(pin)))); \
                                            HAL_GPIO##port##_MODER_REG |= (1 << (2*(pin)+1))
#define SET_GPIO_MOD_ANALOG(port,pin)       HAL_GPIO##port##_MODER_REG |= (3 << (2*(pin)))
#define SET_GPIO_MOD_IN(port,pin)           HAL_GPIO##port##_MODER_REG &= (~(3 << (2*(pin))))
#define SET_GPIO_PULLDOWN(port,pin)         HAL_GPIO##port##_PUPDR_REG &= (~(1 << (2*(pin)))); \
                                            HAL_GPIO##port##_PUPDR_REG |= (1 << (2*(pin)+1))
#define SET_GPIO_NOPULLUPDOWN(port,pin)     HAL_GPIO##port##_PUPDR_REG &= (~(3 << (2*(pin))))


#define SET_GPIO_ALT_FUNC(port, pin, func)	do { \
												if (pin <= 7){ \
													_Pragma("GCC diagnostic push") \
													_Pragma("GCC diagnostic ignored \"-Wshift-count-overflow\"") \
													HAL_GPIOD_AFRH_LO_REG &= ~(0xF << ((pin)*4)); \
													HAL_GPIOD_AFRH_LO_REG |= (func << ((pin)*4)); \
													_Pragma("GCC diagnostic pop") \
												} \
												else{ \
													HAL_GPIOD_AFRH_HI_REG &= ~(0xF << ((pin-8)*4)); \
													HAL_GPIOD_AFRH_HI_REG |= (func << ((pin-8)*4)); \
												}\
											} while (0)

#define TOGGLE_GPIO(port,pin)               HAL_GPIO##port##_ODR_REG ^= (1 << (pin))
#define READ_GPIO(port,pin)                 ((HAL_GPIO##port##_IDR_REG >> (pin)) & 1)

#define TOGGLE_LED_GREEN()                  TOGGLE_GPIO(B,0)
#define TOGGLE_LED_ORANGE()                 TOGGLE_GPIO(E,1)
#define TOGGLE_LED_RED()                    TOGGLE_GPIO(B,14)

// Get a binary input from the user; indicates when the bios should start upon boot
// E.g., user push button or switch. If none available, set to a non-zero value
// And bios will be launched every time in development.
// Never used by the bootloader if the bios is disabled
#define GET_USER_INPUT_STATE()              IS_USER_BUTTON_PRESSED()
#define IS_USER_BUTTON_PRESSED()            (READ_GPIO(C,13) != 0)

// Initialization of the GPIO unit, called upon bootloader start
#define HAL_GPIO_INIT()                     do{\
                                                SET_GPIO_MOD_OUT(B,0);\
                                                SET_GPIO_MOD_OUT(E,1);\
                                                SET_GPIO_MOD_OUT(B,14);\
                                                SET_GPIO_MOD_IN(C, 13);\
                                                SET_GPIO_PULLDOWN(C, 13);\
                                            } while(0)
// Deinitialization of the GPIO unit, called before jumping to image
#define HAL_GPIO_DEINIT()                   do{\
                                                SET_GPIO_MOD_ANALOG(B,0);\
                                                SET_GPIO_MOD_ANALOG(E,1);\
                                                SET_GPIO_MOD_ANALOG(B,14);\
                                                SET_GPIO_MOD_ANALOG(C, 13);\
                                                SET_GPIO_NOPULLUPDOWN(C, 13);\
                                            } while(0)
#endif /* __DRIVERS_GPIO_H__ */