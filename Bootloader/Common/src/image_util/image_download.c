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

extern uint32_t _stImgHeader;
extern uint32_t _stImg2Header;
extern uint32_t _lnImg;

/**
 * @brief Downloads an image at the given address using UART
 * Intended to be used with the image_upload.py script
 *
 * @param img_header address of the recipient flash (i.e., should be the start of an image header)
 *
 * @return RET_OK on success, RET_ERROR on error
 */
static int download_image(uint32_t* img_header);

#ifdef ENABLE_TEST
// Only allow download to primary slot if tests are enabled
int download_image_primary(){
	return download_image(&_stImgHeader);
}
#endif /* ENABLE_TEST */


int download_image_secondary(){
	return download_image(&_stImg2Header);
}

/**
 * @brief Calls a function and return an error if its return value is != RET_OK
 *
 * @param func The function call, usable as in "func;"
 */
#define RETURN_IF_ERROR(func)			do { \
											int error = func; \
											if (error != RET_OK) { return error; } \
										}while(0)

static int download_image(uint32_t* img_header){

	uint32_t buf[HAL_FLASH_NB_32BITWORD_IN_FLASHWORD];
	char ack;

	// Wait for a ":" character before interpreting data
	do {
		RETURN_IF_ERROR(HAL_UART_RECEIVE((uint8_t*)&ack, 1));
	} while (ack != ':');
	
	uint32_t size = 0;

	// Ask the size
	RETURN_IF_ERROR(HAL_UART_RECEIVE((uint8_t*)&size, 4));

	if (size > (uint32_t)&_lnImg){
		return RET_ERROR;
	}

	// Erase flash that will hold the data
	RETURN_IF_ERROR(minimal_flash_erase_range((uint32_t)img_header, size));

	uint32_t processed=0;

	// Proceed flash word by flash word
	while(processed < size && processed < (uint32_t)&_lnImg){
		// Signal that we are ready to receive a flash word
		HAL_UART_PRINTF_NOLN(":");

		// Receive an entire flash word
		RETURN_IF_ERROR(HAL_UART_RECEIVE((uint8_t*)&buf, HAL_FLASH_WORD_SIZE));

		// Write it to flash
		RETURN_IF_ERROR(minimal_flash_write((uint32_t)img_header + processed, &buf, HAL_FLASH_WORD_SIZE));

		processed += HAL_FLASH_WORD_SIZE;
	}

	return RET_OK;
}