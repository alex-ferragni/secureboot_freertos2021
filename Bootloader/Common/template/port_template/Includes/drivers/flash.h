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

#ifndef __DRIVERS_FLASH_H__
#define __DRIVERS_FLASH_H__

#include <stddef.h>
#include "hal/hal_util.h"
#include "flash_map_backend/flash_map_backend.h"

/* -------------------- INTERFACE -------------------- */


// Last valid address of the flash memory (i.e., this address + 1 is invalid)
#define HAL_FLASH_END                           0x081FFFFF

// Size, in bytes, of a flash sector (i.e., minimal erase size)
#define HAL_FLASH_SECTOR_SIZE 					0x20000

// Is a flash address in range?
#define IS_FLASH_PROGRAM_ADDRESS_VALID(ADDRESS) (0 == 0)

// Size in bytes of a flash word (i.e., smallest write size)
#define HAL_FLASH_WORD_SIZE 0

// State of a flash 32-bit word after reset
#define HAL_FLASH_RESET_32BIT_STATE				0xFFFFFFFF

#endif /* __DRIVERS_FLASH_H__ */