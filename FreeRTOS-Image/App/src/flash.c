/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "stm32h7xx_hal.h"
#include "hal/hal_flash.h"
#include <stdint.h>
#include <string.h>

uint32_t get_flash_sector_by_address(void* addr){
    //assert(IS_FLASH_PROGRAM_ADDRESS_VALID((uint32_t)addr));
    return (((uint32_t)addr - FLASH_BANK1_BASE) / FLASH_SECTOR_SIZE) % (FLASH_BANK_SIZE / FLASH_SECTOR_SIZE);
}

uint32_t minimal_flash_read(uint32_t addr, void* buf, size_t length){
	uint32_t status = HAL_OK;
	memcpy(buf, (uint8_t *) addr, length);
	return status;
}

uint32_t minimal_flash_erase_sector(uint32_t addr) {
	uint32_t status = HAL_OK;
	
    HAL_FLASH_Unlock();

	// ST does not clear some flags, and not always, when secure area is set. Clear everything
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_RDSERR_BANK1);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_RDSERR_BANK2);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
	uint32_t PAGEError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.Sector = get_flash_sector_by_address((void*)addr);
	EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_2;
	EraseInitStruct.NbSectors = 1;
	status = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError); 
    HAL_FLASH_Lock();

	return status;
}

uint32_t minimal_flash_write(uint32_t addr, const void *buf, size_t length) {
	uint32_t status = HAL_OK;

    HAL_FLASH_Unlock();

	for(size_t i = 0; i*HAL_FLASH_WORD_SIZE < length; ++i) {

		// St does not clear some flags, and not always, when secure area is set. Clear everything
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_RDSERR_BANK1);
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_RDSERR_BANK2);
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
		// Problem with ST's drivers in the case the board is locked, we need to wait after an erase operation
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY_BANK1) != 0 || __HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY_BANK2) != 0)
 		{
			//wait
		}
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,addr, (uint32_t)(((uint32_t const*)buf) + (i*FLASH_NB_32BITWORD_IN_FLASHWORD)));
        if (status != HAL_OK){
			HAL_FLASH_Lock();
			return status;
		}
		addr += HAL_FLASH_WORD_SIZE;
	}
	
	HAL_FLASH_Lock();

	return status;
}