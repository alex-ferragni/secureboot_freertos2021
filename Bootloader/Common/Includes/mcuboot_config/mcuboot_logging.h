/*
 *    Code Released under MIT License
 *
 *    STM32 port
 *    Copyright (c) 2021 CSEM SA
 *
 *    Interrupt fork
 *    Copyright (c) 2019 Memfault, Inc.
 * 
 *    Original Jekyll Template
 *    Copyright (c) 2015 Jacopo Rabolini
 * 
 *    Permission is hereby granted, free of charge, to any person obtaining a 
 *    copy of this software and associated documentation files (the "Software"), 
 *    to deal in the Software without restriction, including without limitation 
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *    and/or sell copies of the Software, and to permit persons to whom the 
 *    Software is furnished to do so, subject to the following conditions:
 * 
 *    The above copyright notice and this permission notice shall be included 
 *    in all copies or substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 *    THE SOFTWARE.
 */

#ifndef __MCUBOOT_LOGGING_H__
#define __MCUBOOT_LOGGING_H__

#include "hal/hal_util.h"

#define MCUBOOT_LOG_MODULE_DECLARE(...)		GLOBAL_PASS
#define MCUBOOT_LOG_MODULE_REGISTER(...)	GLOBAL_PASS

#include "mcuboot_config/mcuboot_config.h"
#include "mcuboot_config/app_config.h"

#ifdef MCUBOOT_HAVE_LOGGING

#include "hal/hal_uart.h"

#endif /* MCUBOOT_HAVE_LOGGING */

/* Define Logging functions as used by MCUboot (+ an additional one for logs that were added to MCUboot) */

#if defined(MCUBOOT_LOG_ERR_ENABLE) && defined(MCUBOOT_HAVE_LOGGING)

#define MCUBOOT_LOG_ERR(...)	do{ \
										HAL_UART_PRINT_STRING("ERR: "); \
										HAL_UART_PRINTF(__VA_ARGS__); \
									} while (0)

#else

#define MCUBOOT_LOG_ERR(...)	PASS

#endif /* MCUBOOT_LOG_ERR_ENABLE */

#if defined(MCUBOOT_LOG_WRN_ENABLE) && defined(MCUBOOT_HAVE_LOGGING)

#define MCUBOOT_LOG_WRN(...)	do{ \
										HAL_UART_PRINT_STRING("WRN: "); \
										HAL_UART_PRINTF(__VA_ARGS__); \
									} while (0)

#else

#define MCUBOOT_LOG_WRN(...)	PASS

#endif /* MCUBOOT_LOG_WRN_ENABLE */

#if defined(MCUBOOT_LOG_INF_ENABLE) && defined(MCUBOOT_HAVE_LOGGING)

#define MCUBOOT_LOG_INF(...)	do{ \
										HAL_UART_PRINT_STRING("INF: "); \
										HAL_UART_PRINTF(__VA_ARGS__); \
									} while (0)
#define MCUBOOT_LOG_INF_NO_PREFIX(...)	HAL_UART_PRINTF(__VA_ARGS__)

#else

#define MCUBOOT_LOG_INF(...)				PASS
#define MCUBOOT_LOG_INF_NO_PREFIX(...)	PASS

#endif /* MCUBOOT_LOG_INF_ENABLE */

#if defined(MCUBOOT_LOG_INTERNAL_ENABLE) && defined(MCUBOOT_HAVE_LOGGING)

#define MCUBOOT_LOG_INTERNAL(...)	do{ \
										HAL_UART_PRINT_STRING("MCUboot: "); \
										HAL_UART_PRINTF(__VA_ARGS__); \
									} while (0)

#else

#define MCUBOOT_LOG_INTERNAL(...)	PASS

#endif /* MCUBOOT_LOG_INF_ENABLE */

#if defined(MCUBOOT_LOG_DBG_ENABLE) && defined(MCUBOOT_HAVE_LOGGING)

#define MCUBOOT_LOG_DBG(...)	do{ \
										HAL_UART_PRINT_STRING("DBG: "); \
										HAL_UART_PRINTF(__VA_ARGS__); \
									} while (0)

#else

#define MCUBOOT_LOG_DBG(...)	PASS

#endif /* MCUBOOT_LOG_DBG_ENABLE */


#endif /* __MCUBOOT_LOGGING_H__ */