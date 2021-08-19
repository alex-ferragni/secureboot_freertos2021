/**
  ******************************************************************************
  * @file    scb.h
  * @author  CSEM SA
  * @brief   Header file of SCB module.
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

#ifndef __DRIVERS_SCB_H__
#define __DRIVERS_SCB_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_SCB_BASE                            0xE000E000

#define HAL_SCB_VTOR_OFFSET                  	0xD08
#define HAL_SCB_AIRCR_OFFSET                  	0xD0C
#define HAL_SCB_CCR_OFFSET                  	0xD14
#define HAL_SCB_SHCRS_OFFSET                  	0xD24

#define HAL_SCB_VTOR_REG                     	REG_CONTENT(HAL_SCB_BASE, HAL_SCB_VTOR_OFFSET)
#define HAL_SCB_AIRCR_REG                     	REG_CONTENT(HAL_SCB_BASE, HAL_SCB_AIRCR_OFFSET)
#define HAL_SCB_CCR_REG                     	REG_CONTENT(HAL_SCB_BASE, HAL_SCB_CCR_OFFSET)

#define HAL_SCB_AIRCR_RESET_POS					2
#define HAL_SCB_AIRCR_RESET						(1 << HAL_SCB_AIRCR_RESET_POS)
#define HAL_SCB_AIRCR_PRIGROUP_POS				8
#define HAL_SCB_AIRCR_PRIGROUP					(0x7 << HAL_SCB_AIRCR_PRIGROUP_POS)
#define HAL_SCB_AIRCR_VECTKEY_POS				16
#define HAL_SCB_AIRCR_VECTKEY					(0x5FA << HAL_SCB_AIRCR_VECTKEY_POS)

#define HAL_SCB_CCR_BFHFNMIGN_POS				8
#define HAL_SCB_CCR_BFHFNMIGN					(1 << HAL_SCB_CCR_BFHFNMIGN_POS)

/* -------------------- INTERFACE -------------------- */

// Mask faults
// Bad, but comes in handy to detect flash corruption without triggering a fault...
#define SCB_SET_FAULTMASK(value)			__asm__ volatile ("msr faultmask,%0"::"r"(value))

// Request a reset (might take some time) and continue execution
#define SCB_REQUEST_RESET()					HAL_SCB_AIRCR_REG = ((HAL_SCB_AIRCR_REG & HAL_SCB_AIRCR_PRIGROUP) | \
															(HAL_SCB_AIRCR_VECTKEY) | \
															(HAL_SCB_AIRCR_RESET))

// Trigger a system reset, this function should thus never return
// (in particular, code placed after this function should never be executed)
#define TRIGGER_SYSTEM_RESET()				do{ \
												SCB_REQUEST_RESET(); \
												DSB(); \
												ISB(); \
												while (1); \
											} while(0)

// Vector relocation table register. Defines a pointer to the VTOR register,
// usable like in "VTOR_REGISTER = 0xff00;"
#define VTOR_REGISTER						HAL_SCB_VTOR_REG

// Start address for the stack
extern uint32_t _sstack;
// End address for the stack
extern uint32_t _estack;
// Start address for the initialization values of the .data section.
extern uint32_t _sidata;
// Start address for the .data section
extern uint32_t _sdata;
// End address for the .data section
extern uint32_t _edata;
// Start address for the .bss section
extern uint32_t _sbss;
// End address for the .bss section
extern uint32_t _ebss;

// Lowest address of the stack. It is only used to scrub the stack,
// so it is fine if it is actually the end of the stack. All is needed is
// that STACK_START contains the lowest address
#define STACK_START							(&_sstack)
// Highest address of the stack. It is only used to scrub the stack,
// so it is fine if it is actually the start of the stack. All is needed is
// that STACK_END contains the highest address
#define STACK_END							(&_estack)
// Start (= lowest address) of the data segment, in RAM. Location to which 
// data will be copied from flash upon boot.
#define DATA_START							(&_sdata)
// End (= highest address) of the data segment, in RAM. Location until which 
// data will be copied from flash upon boot.
#define DATA_END							(&_edata)
// Start (= lowest address) of the data initialization segment, in flash. 
// Starting address from which data will be copied to RAM upon boot
#define DATA_INIT_START						(&_sidata)
// Start (= lowest address) of the bss segment
#define BSS_START							(&_sbss)
// End (= highest address) of the bss segment
#define BSS_END								(&_ebss)

#endif /* __DRIVERS_SCB_H__ */