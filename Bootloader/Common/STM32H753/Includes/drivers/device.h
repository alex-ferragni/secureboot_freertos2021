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

#ifndef __DRIVERS_DEVICE_H__
#define __DRIVERS_DEVICE_H__

#include <stddef.h>
#include "hal/hal_util.h"

//! A user-defined identifier for different storage mediums
//! (i.e internal flash, external NOR flash, eMMC, etc)
// STM32 only has internal flash
#define FLASH_DEVICE_INTERNAL 0

// MCUboot devices
// Not a driver per-say, but it is useful to separate it from flash drivers
// Each flash "device" (e.g. internal flash, external flash, etc.) must be
// represented by a unique integer, each region must then be associated
// to a region

#define IS_DEVICE_ID_VALID(id)			(id == FLASH_DEVICE_INTERNAL)

#define FLASH_DEVICE_BOOTLOADER 	FLASH_DEVICE_INTERNAL
#define FLASH_DEVICE_PRIMARY 		FLASH_DEVICE_INTERNAL
#define FLASH_DEVICE_SECONDARY 		FLASH_DEVICE_INTERNAL
#define FLASH_DEVICE_SCRATCH 		FLASH_DEVICE_INTERNAL

#endif /* __DRIVERS_DEVICE_H__ */