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

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#include "hal/hal_config.h"

#include "drivers/device.h"
#include <stddef.h>

#ifdef HAL_INCLUDE_FLASH

#include "drivers/flash.h"
#include "hal/hal_util.h"

#ifdef ENABLE_FLASH_CORRUPTION

/* The programming function is exposed and its prototype modified in case we want to corrupt flash */

uint32_t HAL_FLASH_Program(uint32_t* flash_address, uint32_t const* data_address, uint32_t write_size);

#endif /* ENABLE_FLASH_CORRUPTION */

#define HAL_FLASH_NB_32BITWORD_IN_FLASHWORD 	    (HAL_FLASH_WORD_SIZE / sizeof(uint32_t))
#define FLASH_RESET_BIT_STATE						(FLASH_RESET_32BIT_STATE & 1)

#endif /* HAL_INCLUDE_FLASH */

/* External interface, implemented by flash drivers if included, else should be implemented by application */

/**
 * @brief Reads data from flash memory
 *
 * @param addr The flash starting target address
 * @param buf The output data buffer
 * @param length The length (in bytes) of the output buffer; number of bytes that will be read
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t minimal_flash_read(uint32_t addr, void* buf, size_t length);

/**
 * @brief Writes data to flash memory
 *
 * @param addr The flash starting target address
 * @param buf The input data buffer
 * @param length The length (in bytes) of the input buffer; number of bytes that will be written
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t minimal_flash_write(uint32_t addr, const void *buf, size_t length);

/**
 * @brief Erases a flash sector
 *
 * @param addr An address belonging the the target sector
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t minimal_flash_erase_sector(uint32_t addr);

/**
 * @brief Erases one or multiple flash sectors. Erases all sectors that overlap with the input range
 *
 * @param addr The start of the input range
 * @param length The length of the input range
 *
 * @return RET_OK on success, RET_ERROR on error
 */
static inline uint32_t minimal_flash_erase_range(uint32_t addr, uint32_t length);

static inline uint32_t minimal_flash_erase_range(uint32_t addr, uint32_t length){
	uint32_t status;
	uint32_t offset = 0;
	do{
		status = minimal_flash_erase_sector(addr + offset);
		offset += HAL_FLASH_SECTOR_SIZE;
	} while(status == RET_OK && offset < length);
	return status;
}

#endif /* __HAL_FLASH_H__ */