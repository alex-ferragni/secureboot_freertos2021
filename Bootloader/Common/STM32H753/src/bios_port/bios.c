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

#ifdef ENABLE_BIOS

#include "bios_port/bios.h"
#include "hal/hal_security.h"
#include "hal/hal_uart.h"
#include "st/rss.h"

//security should be on by default if nothing is disabled
//explicitely define DISABLE_SAFEGUARD to disable the safeguard
//#define DISABLE_SAFEGUARD

#ifdef DISABLE_SAFEGUARD
#warning "SAFEGUARD IS OFF, PROCEED WITH CAUTION"
#endif /* DISABLE_SAFEGUARD */

void bios_secure_area_confirm();
void bios_rdp1_confirm();
void bios_rdp0_confirm();

void bios_rdp1_confirm(){
	uint8_t response;
	HAL_UART_PRINTF("WARNING: you are currently trying to set readout protection level to 1");
#ifdef DISABLE_SAFEGUARD
	HAL_UART_PRINTF("(WARNING: safeguard is curently off)");
	HAL_UART_PRINTF("(Proceed with caution)");
#endif /* DISABLE_SAFEGUARD */
	HAL_UART_PRINTF("This might lock you device if you did not plan this right");
	HAL_UART_PRINTF("Are you completely sure?");
	HAL_UART_PRINTF("(Any other response will be considered as a no)");

	DECLARE_COMMAND_INPUT('y', "Confirm and set RDP level to 1");

	ASK_FOR_RESPONSE(response);

	switch(response){
		DECLARE_CASE('y'):
#ifndef DISABLE_SAFEGUARD
			HAL_UART_PRINTF("Safeguard is enabled, so I can't do that");
#else
			HAL_UART_PRINTF("Device should be locked now, you need to do a power down reset");
			optreg_enable_rdp1();
#endif /* DISABLE_SAFEGUARD */
			HAL_UART_PRINTF("(RDP was not set to level 1, maybe it was already set or safeguard is on?)");
			break;
		default:
			break;
	}
}

void bios_rdp0_confirm(){
	uint8_t response;
	HAL_UART_PRINTF("WARNING: you are currently trying to set readout protection level to 0");
	HAL_UART_PRINTF("This will perform a mass erase if RDP is set to 1");
	HAL_UART_PRINTF("This will always try to erase the secure area as well, to avoid unnecessary locks");
	HAL_UART_PRINTF("Are you completely sure?");
	HAL_UART_PRINTF("(Any other response will be considered as a no)");

	DECLARE_COMMAND_INPUT('y', "Confirm and set RDP level to 0");

	ASK_FOR_RESPONSE(response);

	switch(response){
		DECLARE_CASE('y'):
			HAL_UART_PRINTF("Device should be locked now, you need to wait some time (it needs ~5 seconds, wait 10)");
			HAL_UART_PRINTF("Then you need to do a power down reset, disable the secure mode");
			HAL_UART_PRINTF("and you can finally flash the device again");
			optreg_disable_secure_area_rdp1();
			HAL_UART_PRINTF("(RDP was not set to level 0, maybe it was already set?)");
			break;
		default:
			break;
	}
}

void bios_secure_area_confirm(){
	uint8_t response;

	HAL_UART_PRINTF("WARNING: you are currently trying to set the secure area");

#ifdef DISABLE_SAFEGUARD
	HAL_UART_PRINTF("(WARNING: safeguard is curently off)");
	HAL_UART_PRINTF("(Proceed with caution)");
#endif /* DISABLE_SAFEGUARD */

	HAL_UART_PRINTF("This will likely lock the board forever if you missed ANY step");
	HAL_UART_PRINTF("Are you completely sure you did everything right?");
	HAL_UART_PRINTF("(Any other response will be considered as a no)");

	DECLARE_COMMAND_INPUT('y', "Confirm and set secure area. Point of no return if you did a mistake");

	ASK_FOR_RESPONSE(response);

	switch(response){
		DECLARE_CASE('y'):
#ifndef DISABLE_SAFEGUARD
			HAL_UART_PRINTF("Safeguard is enabled, so I can't do that");
#else
			resetAndInitializeSecureAreas();
#endif /* DISABLE_SAFEGUARD */
			HAL_UART_PRINTF("(Secure area was not set, maybe it was already set or the Safeguard is enbled?)");
			break;
		default:
			break;
	}
}

uint32_t bios_system(){

#ifdef DISABLE_SAFEGUARD
	HAL_UART_PRINTF("WARNING: safeguard is curently off");
	HAL_UART_PRINTF("Proceed with caution in the advanced system menu");
#endif /* DISABLE_SAFEGUARD */

	uint32_t result = BIOS_STAY_IN_SUBMENU;
	WHILE_NOT_DONE(response, done){
		DECLARE_COMMAND_INPUT('b', "Go back to previous menu");
		DECLARE_COMMAND_INPUT('q', "Quit this menu (and continue execution)");
		DECLARE_COMMAND_INPUT('s', "Set secure mode");
		DECLARE_COMMAND_INPUT('u', "Unset secure mode");
		DECLARE_COMMAND_INPUT('a', "Set secure area (!!!)");
		DECLARE_COMMAND_INPUT('m', "Set RDP level 0 (!!!)");
		DECLARE_COMMAND_INPUT('r', "Set RDP level 1 (!!!)");
		
		ASK_FOR_RESPONSE(response);
		switch(response){
			DECLARE_CASE('a'):
				bios_secure_area_confirm();
				break;

			DECLARE_CASE('b'):
				done = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('m'):
				bios_rdp0_confirm();
				break;

			DECLARE_CASE('q'):
				done = BIOS_EXIT_SUBMENU;
				result = BIOS_EXIT_SUBMENU;
				break;

			DECLARE_CASE('r'):
				bios_rdp1_confirm();
				break;

			DECLARE_CASE('s'):
				HAL_UART_PRINTF("Setting secure mode...");
				optreg_set_security();
				HAL_UART_PRINTF("Secure mode set!");
				break;

			DECLARE_CASE('u'):
				HAL_UART_PRINTF("Unsetting secure mode...");
				optreg_unset_security();
				HAL_UART_PRINTF("Secure mode unset!");
				break;

			default:
				break;
		}
	}
	return result;
}

#endif /* ENABLE_BIOS */