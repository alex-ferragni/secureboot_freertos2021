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

#include "hal/hal.h"
#include "hal/hal_security.h"
#include "bios/bios.h"
#include "image_util/image_util.h"
#include "bootutil/bootutil_public.h"

extern uint32_t _stBootloaderCode;
extern uint32_t _stBootloaderStorage;
extern uint32_t _lnBootloaderStorage;
extern uint32_t _stUsrFlash;
extern uint32_t _stImgHeader;
extern uint32_t _stImg2Header;
extern uint32_t _stScratch;

uint32_t download_slot[HAL_FLASH_NB_32BITWORD_IN_FLASHWORD];

int trigger_fault();
void test_helper_menu();
void write_to_bootloader();
void read_secret_key();

#ifndef ENABLE_TEST
#error "Tests should be enabled!"
#endif /* ENABLE_TEST */

int main(int argc, char *argv[]) {
    //initialize minimum HAL
	HAL_INIT();

	HAL_UART_PRINTF("Test utility image started");

	test_helper_menu();

	//start bios, just in case
	BIOS_START();

    while (1){
    }
}

void test_helper_menu(){
#ifdef ENABLE_BIOS
	DECLARE_COMMAND_INPUT('b', "Start complete bios (for manual use)");
#endif /* ENABLE_BIOS */
	DECLARE_COMMAND_INPUT('c', "Corrupt primary slot");
	DECLARE_COMMAND_INPUT('d', "Corrupt secondary slot");
	DECLARE_COMMAND_INPUT('e', "Corrupt scratch");
	DECLARE_COMMAND_INPUT('h', "Trigger a hardfault");
	DECLARE_COMMAND_INPUT('k', "Try to read secret key");
	DECLARE_COMMAND_INPUT('p', "Install an image in primary slot");
	DECLARE_COMMAND_INPUT('s', "Install an image in secondary slot");
	DECLARE_COMMAND_INPUT('u', "Mark secondary image for update");
	DECLARE_COMMAND_INPUT('w', "Try to write to bootloader");
	DECLARE_COMMAND_INPUT('x', "Erase first sector of primary slot");
	DECLARE_COMMAND_INPUT('y', "Erase first sector of secondary slot");
	DECLARE_TEST_DEFAULT_OPTIONS();

	WHILE_NOT_DONE(response, done){
		TEST_SEND_CHAR('t');
		ASK_FOR_TEST_RESPONSE(response);
		switch (response){

#ifdef ENABLE_BIOS
			DECLARE_CASE('b'):
				bios_start();
				break;
#endif /* ENABLE_BIOS */

			DECLARE_CASE('c'):
				corrupt_flash((uint32_t)&_stImgHeader);
				break;

			DECLARE_CASE('d'):
				corrupt_flash((uint32_t)&_stImg2Header);
				break;

			DECLARE_CASE('e'):
				corrupt_flash((uint32_t)&_stScratch);
				break;

			DECLARE_CASE('h'):
				trigger_fault();
				//failed if arrived here
				break;

			DECLARE_CASE('k'):
				//try to read secret key
				read_secret_key();
				break;

			DECLARE_CASE('p'):
				//receive an image from uart and write it to primary slot
				download_image_primary();
				break;

			DECLARE_CASE('s'):
				//receive an image from uart and write it to secondary slot
				download_image_secondary();
				break;

			DECLARE_CASE('u'):
				boot_set_pending(1);
				break;

			DECLARE_CASE('w'):
				write_to_bootloader();
				//failed if arrived here
				break;

			DECLARE_CASE('x'):
				//erase first primary slot
				minimal_flash_erase_sector((uint32_t)&_stImgHeader);
				break;

			DECLARE_CASE('y'):
				//erase first secondary slot
				minimal_flash_erase_sector((uint32_t)&_stImg2Header);
				break;

			DECLARE_TEST_DEFAULT_CASES();

			default:
				break;
		}
	}
}

void write_to_bootloader(){
	uint32_t buf[HAL_FLASH_NB_32BITWORD_IN_FLASHWORD];

	for (int i=0; i<HAL_FLASH_NB_32BITWORD_IN_FLASHWORD; ++i){
		buf[i] = *(uint32_t*)(&_stBootloaderCode+i);
	}

	//write back same content to bootloader (don't corrupt it...)
	//should still trigger a fault if it is read-only
	uint32_t status = minimal_flash_write((uint32_t)&_stBootloaderCode, (void*)&buf, HAL_FLASH_WORD_SIZE);
	if (status != RET_OK){
		//either we trigger a hardfault, or we simulate one (so we don't expect two results)
		test_hardfault_menu();
	}
}

void read_secret_key(){
	for (uint32_t* data = &_stBootloaderStorage; data<(uint32_t*)(((uint32_t)&_stBootloaderStorage) + ((uint32_t)&_lnBootloaderStorage)); ++data){
		if (*data != 0){
			test_failure_menu();
		}
	}
	test_success_menu();
}

#pragma GCC push_options
#pragma GCC optimize ("O0") 
int trigger_fault(){
	int* test = (int*)0xffffffff;
	*test = 42;
	return 1;
}
#pragma GCC pop_options