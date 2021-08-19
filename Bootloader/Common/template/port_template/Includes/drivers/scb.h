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

#ifndef __DRIVERS_SCB_H__
#define __DRIVERS_SCB_H__

#include "hal/hal_util.h"

/* -------------------- INTERFACE -------------------- */

// Trigger a system reset, this function should thus never return
// (in particular, code placed after this function should never be executed)
#define TRIGGER_SYSTEM_RESET()				do{ \
												PASS; /*request reset*/ \
												DSB(); \
												ISB(); \
												while (1); \
											} while(0)

// Vector relocation table register. Defines a pointer to the VTOR register,
// usable like in "VTOR_REGISTER = 0xff00;"
#define VTOR_REGISTER						(*((uint32_t*)0))

// Lowest address of the stack. It is only used to scrub the stack,
// so it is fine if it is actually the end of the stack. All is needed is
// that STACK_START contains the lowest address
#define STACK_START							((uint32_t*)0)
// Highest address of the stack. It is only used to scrub the stack,
// so it is fine if it is actually the start of the stack. All is needed is
// that STACK_END contains the highest address
#define STACK_END							((uint32_t*)0)
// Start (= lowest address) of the data segment, in RAM. Location to which 
// data will be copied from flash upon boot.
#define DATA_START							((uint32_t*)0)
// End (= highest address) of the data segment, in RAM. Location until which 
// data will be copied from flash upon boot.
#define DATA_END							((uint32_t*)0)
// Start (= lowest address) of the data initialization segment, in flash. 
// Starting address from which data will be copied to RAM upon boot
#define DATA_INIT_START						((uint32_t*)0)
// Start (= lowest address) of the bss segment
#define BSS_START							((uint32_t*)0)
// End (= highest address) of the bss segment
#define BSS_END								((uint32_t*)0)

#endif /* __DRIVERS_SCB_H__ */