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
extern uint32_t _stScratch;
extern uint32_t _lnScratch;

/**
 * @brief Uploads the content of memory via UART
 * Intended to be used with the image_download.py script
 *
 * @param img_header address of the recipient flash (i.e., should be the start of an image header)
 */
int send_flash_content(uint32_t start_addr, uint32_t length);

/**
 * @brief Calls a function and return an error if its return value is != RET_OK
 *
 * @param func The function call, usable as in "func;"
 */
#define RETURN_IF_ERROR(func)			do { \
											int error = func; \
											if (error != RET_OK) { return error; } \
										}while(0)

int send_flash_content(uint32_t start_addr, uint32_t length){
	HAL_UART_PRINTF("Sending content of 0x%08lX, size = %lu",start_addr, length);
	HAL_UART_PRINTF("Waiting for the script...");

	uint32_t sent = 0;
	uint8_t ack = 0;

	// Wait for a ":" character before sending data
	do {
		RETURN_IF_ERROR(HAL_UART_RECEIVE((uint8_t*)&ack, 1));
	} while (ack != ':');

	// Indicate the size of the image
	HAL_UART_PRINTF_NOLN("%08lx",length);

	while (sent < length){
		uint32_t data[HAL_FLASH_NB_32BITWORD_IN_FLASHWORD];

		// Silently hide that memory is corrupted, send 0s
		if (minimal_flash_read(start_addr+sent, &data, HAL_FLASH_WORD_SIZE) != RET_OK){
			memset(&data, 0, HAL_FLASH_WORD_SIZE);
		}

		// Wait for a ":" character before sending more data
		do {
			RETURN_IF_ERROR(HAL_UART_RECEIVE((uint8_t*)&ack, 1));
		} while (ack != ':');
		
		uint8_t* symbol = (uint8_t*) data;

		// Sent an entire flash word
		for (uint32_t i=0; i<HAL_FLASH_WORD_SIZE; ++i){
			HAL_UART_PRINTF_NOLN("%02hhX", symbol[i]);
		}
		
		sent += HAL_FLASH_WORD_SIZE;
	}

	return RET_OK;
}

int send_primary_image(){
	return send_flash_content((uint32_t)&_stImgHeader, (uint32_t)&_lnImg);
}
int send_secondary_image(){
	return send_flash_content((uint32_t)&_stImg2Header, (uint32_t)&_lnImg);
}

void print_memory_content(uint32_t* start_address, uint32_t length){
	uint8_t *content, *end;
	content = (uint8_t*) start_address;
	end = (uint8_t*)start_address + length;
	int i=0;

	while (content < end){
		// Every 8 bytes, print the address
		if (i % 8 == 0){
			HAL_UART_PRINTF_NOLN("0x%08lx: ",(uint32_t) content);
		}
		uint8_t byte;

		// Signal that memory cannot be read by printing invalid characters
		if (minimal_flash_read((uint32_t)content, &byte, 1) != RET_OK){
			HAL_UART_PRINTF_NOLN("XX ");
		}
		// Otherwise print byte
		else{
			HAL_UART_PRINTF_NOLN("%02x ", byte);
		}
		
		// Line break every 8 bytes
		if (i % 8 == 7){
			HAL_UART_PRINT_STRING("\n");
		}
		content++;
		i++;
	}
}