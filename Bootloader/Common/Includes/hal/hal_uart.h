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

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "hal/hal_config.h"

#ifdef HAL_INCLUDE_UART

#include "drivers/uart.h"
#include <stdio.h>
#include <string.h>

/* -------------------- INTERFACE -------------------- */

#define HAL_UART_MAX_MSG_LEN					500

/**
 * @brief Initializes the UART module
 * 
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t HAL_UART_init(void);

/**
 * @brief Deinitialized the UART module
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t HAL_UART_deinit(void);

/**
 * @brief Sends data via the UART canal
 *
 * @param pData The input data to be sent
 * @param Size The number of bytes to be sent
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t HAL_UART_Transmit(uint8_t *pData, uint16_t Size);

/**
 * @brief Receives data from the UART canal; blocks until all data are received
 *
 * @param pData The output data buffer
 * @param Size The number of bytes to be received
 *
 * @return RET_OK on success, RET_ERROR on error
 */
uint32_t HAL_UART_Receive(uint8_t *pData, uint16_t Size);

#define HAL_UART_INIT()							HAL_UART_init()
#define HAL_UART_DEINIT()						HAL_UART_deinit()

// Print a constant string via UART
#define HAL_UART_PRINT_STRING(string)			do { \
													char msg[] = string;\
													HAL_UART_Transmit((uint8_t*)msg, sizeof(msg)-1);\
												} while(0)

// Printf; outputs to UART instead of stdout
#define HAL_UART_PRINTF_NOLN(...)				do{ \
													char buf[HAL_UART_MAX_MSG_LEN]; \
													snprintf(buf, HAL_UART_MAX_MSG_LEN, __VA_ARGS__); \
													HAL_UART_Transmit((uint8_t*)buf, strlen(buf)); \
												} while (0)

// Receive a single char from UART; blocks until it is received
#define HAL_UART_RECEIVE(data, size)			HAL_UART_Receive(data, size)

// Printf with a new line; outputs to UART instead of stdout
#define HAL_UART_PRINTF(...)					do{ \
													HAL_UART_PRINTF_NOLN(__VA_ARGS__); \
													HAL_UART_PRINT_STRING("\n"); \
												} while (0)

#endif /* HAL_INCLUDE_UART */

#endif /* __HAL_UART_H__ */