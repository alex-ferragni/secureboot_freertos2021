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

#include "image_util/image_util.h"
#include <stdlib.h>
#include "hal/hal.h"
#include "hal/hal_security.h"
#include "mcuboot_config/mcuboot_logging.h"
#include <string.h>

extern uint32_t _stBootloaderCode;
extern uint32_t _lnBootloader;
extern uint32_t _stImgHeader;
extern uint32_t _stImg2Header;
extern uint32_t _lnImg;
extern uint32_t _stScratch;
extern uint32_t _lnScratch;
extern uint32_t _stUsrFlash;
extern uint32_t _lnUsrFlash;

void boot_from_image(uint32_t* image_content_start){

    // We need to move the vector table to reflect the location of the main application
    VTOR_REGISTER = ((uint32_t)image_content_start & 0xFFFFFF00);

    //don't forget to deinit anything that the application does not expect to be initalized
	MCUBOOT_LOG_INF("Uninitializing hardware, jumping to 0x%08lX...", VTOR_REGISTER);
	MCUBOOT_LOG_INF_NO_PREFIX("============= END OF BOOTLOADER ============");

	HAL_DEINIT();

	EXIT_BOOTLOADER(VTOR_REGISTER);
}

void print_current_location(){
	// Not exactly the current pc, but close enough
	uint32_t pc = (uint32_t)__builtin_return_address(0);
	HAL_UART_PRINTF_NOLN("PC is at 0x%08lX; Located in ", pc);

	// In the bootloader area
	if (pc >= (uint32_t)&_stBootloaderCode && pc < (uint32_t)&_stBootloaderCode + (uint32_t)&_lnBootloader){
		HAL_UART_PRINTF("the bootloader");
	}
	// In the primary slot
	else if (pc >= (uint32_t)&_stImgHeader && pc < (uint32_t)&_stImgHeader + (uint32_t)&_lnImg){
		HAL_UART_PRINTF("the primary slot");
	}
	// In the secondary slot
	else if (pc >= (uint32_t)&_stImg2Header && pc < (uint32_t)&_stImg2Header + (uint32_t)&_lnImg){
		HAL_UART_PRINTF("the secondary slot (but that should not be the case!)");
	}
	// In the scratch area
	else if (pc >= (uint32_t)&_stScratch && pc < (uint32_t)&_stScratch + (uint32_t)&_lnScratch){
		HAL_UART_PRINTF("the scratch area (but that should not be the case!)");
	}
	// In the remaining user flash
	else if (pc >= (uint32_t)&_stUsrFlash && pc < (uint32_t)&_stUsrFlash + (uint32_t)&_lnUsrFlash){
		HAL_UART_PRINTF("the user flash area");
	}
	// Not in flash?
	else{
		HAL_UART_PRINTF("an unknown area");
	}
}

#ifdef ENABLE_FLASH_CORRUPTION

void corrupt_flash(uint32_t target_address){
	if ((target_address >= (uint32_t)&_stImgHeader && target_address + HAL_FLASH_WORD_SIZE <= (uint32_t)&_stImgHeader + (uint32_t)&_lnImg) ||
		(target_address >= (uint32_t)&_stImg2Header && target_address + HAL_FLASH_WORD_SIZE <= (uint32_t)&_stImg2Header + (uint32_t)&_lnImg) ||
		(target_address >= (uint32_t)&_stScratch && target_address + HAL_FLASH_WORD_SIZE <= (uint32_t)&_stScratch + (uint32_t)&_lnScratch)){
		// Only ever allow corruption of an image or of scratch
		// Just in case the bootloader calls it one day (even though it should not)

		// First, start with a fresh sector
		minimal_flash_erase_sector(target_address);

		uint8_t buf[HAL_FLASH_WORD_SIZE];
		memset(buf, 0xAA, HAL_FLASH_WORD_SIZE/4);

		// - == never written to
		// Word == 0xAA------
		minimal_flash_write(target_address, &buf, HAL_FLASH_WORD_SIZE/4);

		memset(buf, 0x55, HAL_FLASH_WORD_SIZE/4);

		// Word == 0xAA----55
		minimal_flash_write(target_address + (3*HAL_FLASH_WORD_SIZE)/4, &buf, HAL_FLASH_WORD_SIZE/4);

		memset(buf, 0x99, HAL_FLASH_WORD_SIZE);


		// Word == 0xAA----55
		//				+
		//		   0x999999--
		//				=
		//		   corruption
		// A flash word should only be written to once, for consistency we write three times
		// Each time we write another part of the word, with conflicting data
		// (Empirically found, systematically triggering an error is harder than it looks)
		minimal_flash_write(target_address, &buf, (3*HAL_FLASH_WORD_SIZE)/4);
	}
}

#endif /* ENABLE_FLASH_CORRUPTION */