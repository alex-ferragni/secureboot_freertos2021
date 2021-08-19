/**
  ******************************************************************************
  * @file    security.c
  * @author  CSEM SA
  * @brief   Source file of SECURITY module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "hal/hal_security.h"
#include "hal/hal_flash.h"

#if defined(ENABLE_BIOS) || defined(ENABLE_TEST)
#include "hal/hal_uart.h"
#endif /* ENABLE_BIOS || ENABLE_TEST */

#include "st/rss.h"

//Ideally, all of those option-register-changing functions should wait until it is complete (or wait until it is ready before doing it)
//info is in a BUSY flag in some control register

void optreg_set_security(void){
	//unlock option registers
	HAL_FLASH_UNLOCK_OPT_REG();

	// Verify Flash Option Bytes are unlocked
	if (BIT_READ(HAL_FLASH_OPTCR_REG, 0) != 0U)
	{
		//some error handling?
		//the rest won't do anything anyway so no need to stop
	}

	BIT_SET(HAL_FLASH_OPTSR_REG, HAL_FLASH_OPTSR_SECURITY_POS);
	//apply & lock
	HAL_FLASH_LOCK_OPT_REG();
}

void optreg_unset_security(void){
	//unlock option registers
	HAL_FLASH_UNLOCK_OPT_REG();

	// Verify Flash Option Bytes are unlocked
	if (BIT_READ(HAL_FLASH_OPTCR_REG, 0) != 0U)
	{
		//some error handling?
		//the rest won't do anything anyway so no need to stop
	}

	BIT_CLEAR(HAL_FLASH_OPTSR_REG, HAL_FLASH_OPTSR_SECURITY_POS);
	//apply & lock
	HAL_FLASH_LOCK_OPT_REG();
}

/* 
 * *** WARNING ***
 * Make sure you know you can disable RP1 before enabling it!
 * Other security features (secure areas!) might make RDP regression less straight-forward
 * In particular, either the bootloader should be able to disable rdp itself,
 * Or it must successfully leave the secure area, otherwise recovering the board might be complicated...
 * This will make access to flash from the outside impossible
 * Any connexion to a debugger will make access (even internal, so instant crash) to flash impossible until a power-down reset
 * Flash option registers can still be accessed though, reverting RDP1 to RDP0 will wipe the flash
 */
void optreg_enable_rdp1(void){
	//unlock option registers
	HAL_FLASH_UNLOCK_OPT_REG();

	// Verify Flash Option Bytes are unlocked
	if (BIT_READ(HAL_FLASH_OPTCR_REG, 0) != 0U)
	{
		//some error handling?
		//the rest won't do anything anyway so no need to stop
	}

	//enable rdp level 1
	HAL_FLASH_OPTSR_REG = (HAL_FLASH_OPTSR_REG & (~HAL_FLASH_OPTSR_RDP)) | HAL_FLASH_OPTSR_RDP1;
	//apply & lock
	HAL_FLASH_LOCK_OPT_REG();
}

void optreg_disable_secure_area_rdp1(void){
	//unlock option registers
	HAL_FLASH_UNLOCK_OPT_REG();

	//Verify Flash Option Bytes are unlocked
	if (BIT_READ(HAL_FLASH_OPTCR_REG, 0) != 0U)
	{
		//some error handling?
		//the rest won't do anything anyway so no need to stop
	}

	//unset secure area
	//0x0FE could be anything (by default 0x0FF) larger than 0, but we set 0x0FE to see
	//that our own change was indeed applied
	//don't forget to set DMES! forgetting this would not reset the secure area and lock the board forever!
	HAL_FLASH_SCAR1_REG = (HAL_FLASH_SCAR1_REG & (~(HAL_FLASH_SCAR1_SEC_AREA_START1 | HAL_FLASH_SCAR1_SEC_AREA_END1))) | 
					HAL_FLASH_SCAR1_DMES1 | (0x0FE << HAL_FLASH_SCAR1_SEC_AREA_START1_POS) | (0x000 << HAL_FLASH_SCAR1_SEC_AREA_END1_POS);

	//disable rdp
	HAL_FLASH_OPTSR_REG = (HAL_FLASH_OPTSR_REG & (~HAL_FLASH_OPTSR_RDP)) | HAL_FLASH_OPTSR_RDP0;
	//apply & lock
	HAL_FLASH_LOCK_OPT_REG();
}

board_security_state_t security_get_board_security(void){
	if (HAL_OPTREG_GET_RDP_LEVEL() == HAL_OPTREG_GET_RDP0() &&
		HAL_OPTREG_GET_SECURE_MODE() == 0 &&
		HAL_OPTREG_GET_SEC_AREA_START() > HAL_OPTREG_GET_SEC_AREA_END()){

		//Board unlocked and in valid state
		return security_none;
	}
	else if (HAL_OPTREG_GET_RDP_LEVEL() == HAL_OPTREG_GET_RDP0() &&
		HAL_OPTREG_GET_SECURE_MODE() == 1 &&
		HAL_OPTREG_GET_SEC_AREA_START() > HAL_OPTREG_GET_SEC_AREA_END()){
		
		//Secure mode set and secure area not set
		return security_secure_bit;
	}
	else if (HAL_OPTREG_GET_RDP_LEVEL() == HAL_OPTREG_GET_RDP0() &&
		HAL_OPTREG_GET_SECURE_MODE() == 1 &&
		HAL_OPTREG_GET_SEC_AREA_START() < HAL_OPTREG_GET_SEC_AREA_END()){

		//Secure mode set and secure area set, rdp level is 0
		return security_secure_area;
	}
	else if (HAL_OPTREG_GET_RDP_LEVEL() != HAL_OPTREG_GET_RDP0() && HAL_OPTREG_GET_RDP_LEVEL() != HAL_OPTREG_GET_RDP2() &&
		HAL_OPTREG_GET_SECURE_MODE() == 1 &&
		HAL_OPTREG_GET_SEC_AREA_START() < HAL_OPTREG_GET_SEC_AREA_END()){

		//Device fully locked
		return security_rdp1;
	}
	else{
		//Unexpected state, something is not right...
		return security_invalid;
	}
}

void security_check_and_increase_security(){
	board_security_state_t current_state = security_get_board_security();
	switch(current_state){
		case security_none:
			optreg_set_security();
			if (security_get_board_security() != security_secure_bit){
				//something is not right
				break;
			}
			//fallthrough, no reset needed just enable next step
		case security_secure_bit:
			resetAndInitializeSecureAreas();
			//should never be reached, a reset is done
			break;

		case security_secure_area:
			optreg_enable_rdp1();
			//should never be reached, a reset is done
			break;

		case security_rdp1:
			//security is on, nothing to do
			break;

		default:
			//something is not right
			break;
	}
}

#ifdef ENABLE_BIOS

extern uint32_t _stImgContent;
extern uint32_t _stImg2Header;

void security_print_security_status(){
	HAL_UART_PRINTF("Status of device:");

	HAL_UART_PRINTF("Start address of primary image (if one is loaded): 0x%08lX", (uint32_t)&_stImgContent);
	HAL_UART_PRINTF("Load address of secondary image (if one needs to be downloaded): 0x%08lX", (uint32_t)&_stImg2Header);

	if (HAL_OPTREG_GET_SECURE_MODE() != 0){
		HAL_UART_PRINTF("Secure mode: ON");
	}
	else{
		HAL_UART_PRINTF("Secure mode: OFF");
	}

	uint32_t rdp = HAL_OPTREG_GET_RDP_LEVEL();
	if (rdp == HAL_OPTREG_GET_RDP0()){
		HAL_UART_PRINTF("RDP level: 0 (0x%02lX, nothing is locked)", rdp);
	}
	else if (rdp == HAL_OPTREG_GET_RDP2()){
		HAL_UART_PRINTF("RDP level: *2* (0x%02lX, everything is locked forever, that better be a bug!)", rdp);
	}
	else{
		HAL_UART_PRINTF("RDP level: 1 (0x%02lX, device is temporarily locked)", rdp);
	}

	uint32_t secureAreaStart = HAL_OPTREG_GET_SEC_AREA_START();
	uint32_t secureAreaEnd = HAL_OPTREG_GET_SEC_AREA_END();
	uint32_t dmes = HAL_OPTREG_GET_DMES();

	if (dmes == 0){
		HAL_UART_PRINTF("DMES for secure area is OFF");
	}
	else{
		HAL_UART_PRINTF("DMES for secure area is ON");
	}

	if (secureAreaStart == secureAreaEnd){
		HAL_UART_PRINTF("Secure area is enabled, it covers the entire region");
		HAL_UART_PRINTF("(start: 0x%08lX, end: 0x%08lX)", secureAreaStart, secureAreaEnd);
	}
	else if (secureAreaStart < secureAreaEnd){
		HAL_UART_PRINTF("Secure area is enabled, it covers 0x%08lX to 0x%08lX", secureAreaStart, secureAreaEnd);
	}
	else{
		HAL_UART_PRINTF("Secure area is disabled");
		HAL_UART_PRINTF("(start: 0x%08lX, end: 0x%08lX)", secureAreaStart, secureAreaEnd);
	}
	
	//print secure area + DMES bit
}

#endif /* ENABLE_BIOS */