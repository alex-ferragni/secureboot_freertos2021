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

#include <string.h>
#include "hal/hal.h"
#include "bios_port/bios.h"
#include "hal/hal_security.h"

extern uint32_t _stImgContent;
extern uint32_t _stImg2Header;
extern uint32_t _stUsrFlash;

#if defined(ENABLE_BIOS) || defined(ENABLE_TEST)

extern uint32_t _stImgHeader;
extern uint32_t _lnImg;
extern uint32_t _stScratch;
extern uint32_t _lnScratch;

#endif /* ENABLE_BIOS || ENABLE_TEST */

#ifdef ENABLE_BIOS

#include "bootutil/bootutil_public.h"
#include "image_util/image_util.h"

extern uint32_t _stImg2Content;
extern uint32_t _stBootloaderStorage;
extern uint32_t _lnBootloaderStorage;

/**
 * @brief Opens a bios sub-menu containing print-related data
 *
 * @return BIOS_EXIT_SUBMENU or BIOS_STAY_IN_SUBMENU, dpending on the action to take
 */
uint32_t bios_dump(){
	// Control the current submenu with done (0 = re-prompt, 1 = go back one level)
	// Control the parent menu with result (0 = continue with parent, 1 = completely exit bios)
	uint32_t result = 0;
	WHILE_NOT_DONE(response, done){
		DECLARE_COMMAND_INPUT('b', "Go back to previous menu");
		DECLARE_COMMAND_INPUT('q', "Quit this menu (and continue execution)");
		DECLARE_COMMAND_INPUT('k', "Read secure bootloader area's key");
		DECLARE_COMMAND_INPUT('o', "Print flash option registers status");
		DECLARE_COMMAND_INPUT('p', "Dump start of primary slot content");
		DECLARE_COMMAND_INPUT('s', "Dump start of secondary slot content");
		DECLARE_COMMAND_INPUT('i', "Download complete content of image");
		DECLARE_COMMAND_INPUT('u', "Download complete content of update");
		
		ASK_FOR_RESPONSE(response);

		switch(response){
			DECLARE_CASE('b'):
				// Go back one level
				done = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('q'):
				// Completely close bios
				done = BIOS_EXIT_SUBMENU;
				result = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('k'):
				// Print the content of the secret bootlaoder area
				HAL_UART_PRINTF("Trying to read secret bootloader area...");
				HAL_UART_PRINTF("Dumping its content:");

				print_memory_content(&_stBootloaderStorage, (uint32_t)&_lnBootloaderStorage);
				break;

			DECLARE_CASE('o'):
				// Print the status of the board protection
				security_print_security_status();
				break;

			DECLARE_CASE('p'):
				// Dump the start of the header & content of primary slot
				HAL_UART_PRINTF("Dumping start of primary slot content:");

				print_memory_content(&_stImgHeader, 128);
				HAL_UART_PRINTF("[...]");
				print_memory_content(&_stImgContent, 128);
				HAL_UART_PRINTF("[...]");
				break;

			DECLARE_CASE('s'):
				// Dump the start of the header & content of secondary slot
				HAL_UART_PRINTF("Dumping start of secondary slot content:");

				print_memory_content(&_stImg2Header, 128);
				HAL_UART_PRINTF("[...]");
				print_memory_content(&_stImg2Content, 128);
				HAL_UART_PRINTF("[...]");
				break;

			DECLARE_CASE('i'):
				// Sent the entire content of the primary slot via UART
				send_primary_image();
				break;

			DECLARE_CASE('u'):
				// Sent the entire content of the secondary slot via UART
				send_secondary_image();
				break;

			default:
				// Any other character: ignore and re-prompt menu
				break;
		}
	}
	// Signal to the parent whether they should exit
	return result;
}

void bios_start(){
	HAL_UART_PRINTF("================ MINI BIOS =================");

	print_current_location();

	security_print_security_status();

	WHILE_NOT_DONE(response, done){
		DECLARE_COMMAND_INPUT('d', "Download image in secondary slot");
		DECLARE_COMMAND_INPUT('f', "Force boot from image");
		DECLARE_COMMAND_INPUT('p', "Open the print menu");
		DECLARE_COMMAND_INPUT('q', "Quit this menu (and continue execution)");
		DECLARE_COMMAND_INPUT('r', "Restart the board");
		DECLARE_COMMAND_INPUT('s', "Open the advanced system menu");
		DECLARE_COMMAND_INPUT('u', "Mark image for update");

		ASK_FOR_RESPONSE(response);

		switch(response){
			// Download an image into the secondary slot, via UART
			DECLARE_CASE('d'):
				HAL_UART_PRINTF("Waiting for data...");
				download_image_secondary();
				break;

			DECLARE_CASE('f'):
				// Force-boot in the primary slot, as if it were verified by MCUboot
				// (It's good to keep a backdoor during development, incorrectly setting up the
				// secure area might prevent any interaction with the board configuration, so keep
				// a way to boot even if the verification algorithm is faulty)
				HAL_UART_PRINTF("Bypassing secure boot, jumping at 0x%08lX...", (uint32_t)&_stImgContent);
				boot_from_image(&_stImgContent);
				//should never be reached
				break;

			DECLARE_CASE('p'):
				// Open a sub-menu
				// Close this one if requested
				done = bios_dump();
				break;

			DECLARE_CASE('q'):
				// Exit the bios
				done = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('r'):
				HAL_UART_PRINTF("Resetting...");
				TRIGGER_SYSTEM_RESET();
				//should never be reached
				break;

			DECLARE_CASE('s'):
				// Open the port-specific system sub-menu
				done = bios_system();
				break;

			DECLARE_CASE('u'):
				// Mark the image in secondary slot for update
				boot_set_pending(1);
				HAL_UART_PRINTF("Marked image in secondary slot for update");
				break;

			default:
				break;
		}
	}
	HAL_UART_PRINTF("============= END OF MINI BIOS =============");
}

#endif /* ENABLE_BIOS */

#ifdef ENABLE_TEST

// All test menus are defined here since they will need to be called from different images

void test_init_menu(){
#ifdef ENABLE_BIOS
	DECLARE_COMMAND_INPUT('b', "Start complete bios (for manual use)");
#endif /* ENABLE_BIOS */
	DECLARE_COMMAND_INPUT('c', "Continue with mcuboot");
	DECLARE_TEST_DEFAULT_OPTIONS();
	WHILE_NOT_DONE(response, done){
		TEST_SEND_CHAR('i');
		ASK_FOR_TEST_RESPONSE(response);
		switch (response){
			// Include the complete bios if requested
#ifdef ENABLE_BIOS
			DECLARE_CASE('b'):
				done = BIOS_EXIT_SUBMENU;
				bios_start();
				break;
#endif /* ENABLE_BIOS */

			DECLARE_CASE('c'):
				done = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_TEST_DEFAULT_CASES();

			default:
				break;
		}
	}
}

void test_hardfault_menu(){
	DEFAULT_TEST_MENU('h');
}

void test_valid_image_menu(){
	DEFAULT_TEST_MENU('v');
}

void test_invalid_image_menu(){
	DEFAULT_TEST_MENU('x');
}

void test_success_menu(){
	DEFAULT_TEST_MENU('s');
}

void test_failure_menu(){
	DEFAULT_TEST_MENU('f');
}

void test_mcuboot_swap_menu(){
	DECLARE_TEST_DEFAULT_OPTIONS(); 
	// Add an option to continue the execution normally; continue swapping
	DECLARE_COMMAND_INPUT('c', "Continue with mcuboot");
	WHILE_NOT_DONE(response, done){ 
		TEST_SEND_CHAR('m'); 
		ASK_FOR_TEST_RESPONSE(response); 
		switch (response){ 
			DECLARE_TEST_DEFAULT_CASES(); 
			default: 
				break; 

			DECLARE_CASE('c'):
				done = BIOS_EXIT_SUBMENU;
				break;
		} 
	}
}

#endif /* ENABLE_TEST */

