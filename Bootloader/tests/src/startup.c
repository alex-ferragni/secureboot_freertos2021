/*
 *    Code Released under MIT License
 *
 *    STM32 port
 *    Copyright (c) 2021 CSEM SA
 *
 *    Interrupt fork
 *    Copyright (c) 2019 Memfault, Inc.
 * 
 *    Original Jekyll Template
 *    Copyright (c) 2015 Jacopo Rabolini
 * 
 *    Permission is hereby granted, free of charge, to any person obtaining a 
 *    copy of this software and associated documentation files (the "Software"), 
 *    to deal in the Software without restriction, including without limitation 
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *    and/or sell copies of the Software, and to permit persons to whom the 
 *    Software is furnished to do so, subject to the following conditions:
 * 
 *    The above copyright notice and this permission notice shall be included 
 *    in all copies or substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 *    THE SOFTWARE.
 */

//! @file
//!
//! A pure C-based Reset Handler and Vector Table for Cortex-M devices

#include <stdint.h>
#include "hal/hal_scb.h"
#include "bios/bios.h"

extern int main(void);

// Prevent inlining to avoid persisting any variables on stack
__attribute__((noinline)) static void prv_cinit(void) {
	// Initialize data and bss
	// Copy the data segment initializers from flash to SRAM
	for (uint32_t *dst = DATA_START, *src = DATA_INIT_START; dst < DATA_END;) {
		*(dst++) = *(src++);
	}
	
	// Zero fill the bss segment.
	for (uint32_t *dst = BSS_START; dst < BSS_END;) {
		*(dst++) = 0;
	}
}

void Reset_Handler(void) {
	prv_cinit();

	// Call the application's entry point.
	main();

	// should be unreachable
	while (1) { }
}

// DefaultIntHandler is used for unpopulated interrupts
static void DefaultIntHandler(void) {
	__asm__("bkpt");
	// Go into an infinite loop.
	while (1);
}

static void NMI_Handler(void) {
	DefaultIntHandler();
}

static void HardFault_Handler(void) {
	test_hardfault_menu();
	DefaultIntHandler();
}

//void DebugMonitor_Exception(void);
//void Irq2_Handler(void);

#define EXTERNAL_INT_BASE 16 // NVIC Interrupt 0 starts here
// A minimal vector table for a Cortex M.
__attribute__((section(".isr_vector"))) void (*const g_pfnVectors[])(void) = {
	[0] = (void(*)(void))STACK_END, // initial stack pointer
	[1] = Reset_Handler,
	[2] = NMI_Handler,
	[3] = HardFault_Handler,
	[4] = DefaultIntHandler,
	[5] = DefaultIntHandler,
	[6] = DefaultIntHandler,
	[7] = DefaultIntHandler,
	[8] = DefaultIntHandler,
	[9] = DefaultIntHandler,
	[10] = DefaultIntHandler,
	[11] = DefaultIntHandler,
	[12] = DefaultIntHandler,
	[13] = DefaultIntHandler,
	[14] = DefaultIntHandler,
	[15] = DefaultIntHandler,
};