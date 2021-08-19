/**
  ******************************************************************************
  * @file    util.h
  * @author  CSEM SA
  * @brief   Utility header file.
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

#ifndef __DRIVERS_UTIL_H__
#define __DRIVERS_UTIL_H__

// Instruction Synchronization Barrier; flushes the pipeline in the processor,
// so that all instructions following the ISB are fetched from cache or memory,
// after the instruction has been completed.
__attribute__((always_inline)) static inline void ISB(void)
{
  __asm volatile ("isb 0xF":::"memory");
}

// Data Synchronization Barrier; acts as a special kind of Data Memory Barrier.
// It completes when all explicit memory accesses before this instruction complete.
__attribute__((always_inline)) static inline void DSB(void)
{
  __asm volatile ("dsb 0xF":::"memory");
}

#endif /* __DRIVERS_UTIL_H__ */