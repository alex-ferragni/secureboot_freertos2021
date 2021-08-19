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

#ifndef __HAL_APP_H__
#define __HAL_APP_H__

#include "stm32h7xx_hal.h"
#include "st/rss.h"
#include <stdio.h>
#include <string.h>

//make the link between our drivers and ST's drivers, for the bootloader generic interface
#define HAL_FLASH_WORD_SIZE						32

#define HAL_FLASH_SECTOR_SIZE 					FLASH_SECTOR_SIZE
#define HAL_FLASH_NB_32BITWORD_IN_FLASHWORD 	FLASH_NB_32BITWORD_IN_FLASHWORD
#define HAL_FLASH_RESET_32BIT_STATE 			0xffffffff

#define HAL_FLASH_OPTKEYR_REG					FLASH->OPTKEYR
#define HAL_FLASH_OPTCR_REG						FLASH->OPTCR
#define HAL_FLASH_OPTSR_REG						FLASH->OPTSR_PRG
#define HAL_FLASH_SCAR1_REG						FLASH->SCAR_PRG1
#define HAL_FLASH_OPTKEY1						FLASH_OPT_KEY1
#define HAL_FLASH_OPTKEY2						FLASH_OPT_KEY2
#define HAL_FLASH_OPTSR_SECURITY				FLASH_OPTSR_SECURITY
#define HAL_FLASH_OPTSR_SECURITY_POS			FLASH_OPTSR_SECURITY_Pos
#define HAL_FLASH_OPTSR_RDP						FLASH_OPTSR_RDP
#define HAL_FLASH_OPTSR_RDP_POS					FLASH_OPTSR_RDP_Pos
#define HAL_FLASH_OPTCR_OPTLOCK					FLASH_OPTCR_OPTLOCK
#define HAL_FLASH_OPTCR_OPTSTART				FLASH_OPTCR_OPTSTART
#define HAL_FLASH_SCAR1_DMES1					FLASH_SCAR_DMES
#define HAL_FLASH_SCAR1_DMES1_POS				FLASH_SCAR_DMES_Pos
#define HAL_FLASH_SCAR1_SEC_AREA_END1_POS		FLASH_SCAR_SEC_AREA_END_Pos
#define HAL_FLASH_SCAR1_SEC_AREA_END1			FLASH_SCAR_SEC_AREA_END
#define HAL_FLASH_SCAR1_SEC_AREA_START1_POS		FLASH_SCAR_SEC_AREA_START_Pos
#define HAL_FLASH_SCAR1_SEC_AREA_START1			FLASH_SCAR_SEC_AREA_START

#define HAL_FLASH_OPTSR_RDP0					(0xAA << FLASH_OPTSR_RDP_Pos)
#define HAL_FLASH_OPTSR_RDP1					(0xBB << FLASH_OPTSR_RDP_Pos) 

#define EXIT_BOOTLOADER(target_address)			exitSecureArea(target_address)

#define HAL_UART_MAX_MSG_LEN 500
#define GLOBAL_UART_HANDLE huart

extern UART_HandleTypeDef GLOBAL_UART_HANDLE;

#define HAL_UART_PRINT_STRING(string)	do{ \
											char msg[] = string; \
											HAL_UART_Transmit(&GLOBAL_UART_HANDLE, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY); \
										} while(0)

#define HAL_UART_PRINTF_NOLN(...)		do{ \
											char buf[HAL_UART_MAX_MSG_LEN]; \
											snprintf(buf, HAL_UART_MAX_MSG_LEN, __VA_ARGS__); \
											HAL_UART_Transmit(&GLOBAL_UART_HANDLE, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY); \
										} while (0)
							
#define HAL_UART_PRINTF(...)			do{ \
											HAL_UART_PRINTF_NOLN(__VA_ARGS__); \
											HAL_UART_PRINT_STRING("\n"); \
										} while (0)

#define HAL_UART_RECEIVE(data, size)	HAL_UART_Receive(&GLOBAL_UART_HANDLE, data, size, HAL_MAX_DELAY)

#endif /* __HAL_APP_H__ */