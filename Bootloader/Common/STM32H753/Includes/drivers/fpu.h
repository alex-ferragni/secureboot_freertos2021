/**
  ******************************************************************************
  * @file    fpu.h
  * @author  CSEM SA
  * @brief   Header file of FPU module.
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

#ifndef __DRIVERS_FPU_H__
#define __DRIVERS_FPU_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_FPU_CPACR_ADDRESS                   0xE000ED88

#define HAL_FPU_CPACR_REG                       REG_CONTENT(HAL_FPU_CPACR_ADDRESS, 0x0)

/* -------------------- INTERFACE -------------------- */

//                                                            0b01 01, privileged mode only
#define ENABLE_FPU()                        HAL_FPU_CPACR_REG |= (0x5 << 20)
#define DISABLE_FPU()                       HAL_FPU_CPACR_REG &= (~(0xF << 20))

// Initialization of the FPU unit, called upon bootloader start
#define HAL_FPU_INIT()                      ENABLE_FPU()
// Deinitialization of the FPU unit, called before jumping to image
#define HAL_FPU_DEINIT()                    DISABLE_FPU()

#endif /* __DRIVERS_FPU_H__ */