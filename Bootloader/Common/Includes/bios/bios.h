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

#ifndef __BIOS_H__
#define __BIOS_H__

#include <stdint.h>
#include "hal/hal_util.h"

#if defined(ENABLE_BIOS) || defined(ENABLE_TEST)

/**
 * @brief Displays an input option of the bios (/test menu)
 *
 * @param input_char The (lower-case) character that is linked to an option
 * @param help_string A string secribing the resulting action
 */
#define DECLARE_COMMAND_INPUT(input_char, help_string)			HAL_UART_PRINTF("'%c/%c': %s", input_char, input_char-32, help_string);

/**
 * @brief Defines some variables and open a "while not done" loop
 *
 * @param response The name of the char variable which will be defined
 * @param done The name of the boolean (int) variable which will be defined
 */
#define WHILE_NOT_DONE(response, done)				uint8_t response; \
													int done = BIOS_STAY_IN_SUBMENU; \
													while(done == BIOS_STAY_IN_SUBMENU)

/**
 * @brief Print the prompt characters and receives a user response
 *
 * @param response The variable which will hold the signel-character response
 */
#define ASK_FOR_RESPONSE(response)					HAL_UART_PRINT_STRING(" >> "); \
													HAL_UART_RECEIVE(&response, 1); \
													HAL_UART_PRINT_STRING("\n");

/**
 * @brief Opens a case statement on a character
 *
 * @param input_char The (lower-case) character that will trigger an actiion
 */
#define DECLARE_CASE(input_char)					case input_char: \
													case input_char-32

// Either exit a submenu (or exit the bios entirely), or stay in the same menu and re-prompt options
#define BIOS_EXIT_SUBMENU							1
#define BIOS_STAY_IN_SUBMENU						0

#endif /* ENABLE_BIOS || ENABLE_TEST */

#ifdef ENABLE_BIOS

/**
 * @brief Opens the utility bios
 */
void bios_start();

/**
 * @brief Opens the port-specific system menu
 *
 * @return BIOS_EXIT_SUBMENU or BIOS_STAY_IN_SUBMENU, dpending on the action to take
 */
uint32_t bios_system();

// When the bios is enabled, open it
#define BIOS_START()								bios_start()

#else /* ENABLE_BIOS */

// When the bios is not enabled, do nothing
#define BIOS_START()				PASS

#endif /* ENABLE_BIOS */


#ifdef ENABLE_TEST

#include "image_util/image_util.h"

/**
 * @brief Prompts the default test options that should be common to every test menu
 */
#define DECLARE_TEST_DEFAULT_OPTIONS()				DECLARE_COMMAND_INPUT('r', "Reset board"); \
													DECLARE_COMMAND_INPUT('t', "Jump to test image")

/**
 * @brief Defines (and reacts to) the default test options that should be common to every test menu
 */
#define DECLARE_TEST_DEFAULT_CASES()				DECLARE_CASE('r'): \
														TRIGGER_SYSTEM_RESET(); \
														break; \
													DECLARE_CASE('t'): \
														boot_from_image(&_stUsrFlash); \
														break

/**
 * @brief Receives a single character from UART
 * 
 * @param response the variable that will store the char
 */
#define ASK_FOR_TEST_RESPONSE(response)				HAL_UART_RECEIVE(&response, 1);

/**
 * @brief Sends an identifier followed by a char; will be recognized by the test script
 * 
 * @param char the identifier of the current menu
 */
#define TEST_SEND_CHAR(char)					HAL_UART_PRINTF_NOLN("$>%c",char)

/**
 * @brief Opens a minimal test menu containing only default options
 *
 * @param input_char The character that is linked to this menu; the test script will use it to monitor the execution
 */
#define DEFAULT_TEST_MENU(print_char)				do { \
														DECLARE_TEST_DEFAULT_OPTIONS(); \
														WHILE_NOT_DONE(response, done){ \
															TEST_SEND_CHAR(print_char); \
															ASK_FOR_TEST_RESPONSE(response); \
															switch (response){ \
																DECLARE_TEST_DEFAULT_CASES(); \
																default: \
																	break; \
															} \
														} \
													} while(0)

// Expose all hooks:
// After the initializtion of the bootloader
void test_init_menu();
// In the hardfault handler
void test_hardfault_menu();
// When no valid image was found
void test_invalid_image_menu();
// Before jumping to a valid image
void test_valid_image_menu();
// Generic success menu
void test_success_menu();
// Generic failure menu
void test_failure_menu();
// During a swap operation (inside MCUboot!)
void test_mcuboot_swap_menu();

// Fill the hooks that are not part of the test image and shouldbe seen only when tests are enabled
#define TEST_INIT_MENU()			test_init_menu()
#define TEST_VALID_IMAGE_MENU()		test_valid_image_menu()
#define TEST_INVALID_IMAGE_MENU()	test_invalid_image_menu()
#define TEST_MCUBOOT_SWAP_MENU()	test_mcuboot_swap_menu()

#else /* ENABLE_TEST */

// Leave the hooks empty
#define TEST_INIT_MENU()			PASS
#define TEST_VALID_IMAGE_MENU()		PASS
#define TEST_INVALID_IMAGE_MENU()	PASS
#define TEST_MCUBOOT_SWAP_MENU()	PASS

#endif /* ENABLE_TEST */

#endif /* __BIOS_H__ */