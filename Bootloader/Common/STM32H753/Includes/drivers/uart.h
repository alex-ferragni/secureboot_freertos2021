/**
  ******************************************************************************
  * @file    uart.h
  * @author  CSEM SA
  * @brief   Header file of UART module.
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

#ifndef __DRIVERS_UART_H__
#define __DRIVERS_UART_H__

#include "hal/hal_util.h"
#include "hal/hal_rcc.h"
#include <stdio.h>
#include <string.h>

/* -------------------- REGISTERS -------------------- */

#define HAL_USART3_BASE								0x40004800

#define HAL_USART_CR1_OFFSET						0x000
#define HAL_USART_CR2_OFFSET						0x004
#define HAL_USART_CR3_OFFSET						0x008
#define HAL_USART_BRR_OFFSET						0x00C
#define HAL_USART_ISR_OFFSET						0x01C
#define HAL_USART_RDR_OFFSET						0x024
#define HAL_USART_TDR_OFFSET						0x028

#define HAL_USART3_CR1_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_CR1_OFFSET)
#define HAL_USART3_CR2_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_CR2_OFFSET)
#define HAL_USART3_CR3_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_CR3_OFFSET)
#define HAL_USART3_BRR_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_BRR_OFFSET)
#define HAL_USART3_ISR_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_ISR_OFFSET)
#define HAL_USART3_RDR_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_RDR_OFFSET)
#define HAL_USART3_TDR_REG							REG_CONTENT(HAL_USART3_BASE, HAL_USART_TDR_OFFSET)


#define HAL_USART_CR1_UE_POS         				0
#define HAL_USART_CR1_UE							(1 << HAL_USART_CR1_UE_POS)
#define HAL_USART_CR1_RE_POS         				2
#define HAL_USART_CR1_RE							(1 << HAL_USART_CR1_RE_POS)
#define HAL_USART_CR1_TE_POS         				3
#define HAL_USART_CR1_TE							(1 << HAL_USART_CR1_TE_POS)
#define HAL_USART_CR1_PS_POS         				9
#define HAL_USART_CR1_PS							(1 << HAL_USART_CR1_PS_POS)
#define HAL_USART_CR1_PCE_POS         				10
#define HAL_USART_CR1_PCE							(1 << HAL_USART_CR1_PCE_POS)
#define HAL_USART_CR1_M0_POS         				12
#define HAL_USART_CR1_M0							(1 << HAL_USART_CR1_M0_POS)
#define HAL_USART_CR1_OVER8_POS         			15
#define HAL_USART_CR1_OVER8							(1 << HAL_USART_CR1_OVER8_POS)
#define HAL_USART_CR1_M1_POS         				28
#define HAL_USART_CR1_M1							(1 << HAL_USART_CR1_M1_POS)
#define HAL_USART_CR1_FIFOEN_POS         			29
#define HAL_USART_CR1_FIFOEN						(1 << HAL_USART_CR1_FIFOEN_POS)

#define HAL_USART_CR2_CLKEN_POS         			11
#define HAL_USART_CR2_CLKEN							(1 << HAL_USART_CR2_CLKEN_POS)
#define HAL_USART_CR2_STOP_POS         				12
#define HAL_USART_CR2_STOP							(0x3 << HAL_USART_CR2_STOP_POS)
#define HAL_USART_CR2_LINEN_POS         			14
#define HAL_USART_CR2_LINEN							(1 << HAL_USART_CR2_LINEN_POS)

#define HAL_USART_CR3_IREN_POS         				1
#define HAL_USART_CR3_IREN							(1 << HAL_USART_CR3_IREN_POS)
#define HAL_USART_CR3_HDSEL_POS         			3
#define HAL_USART_CR3_HDSEL							(1 << HAL_USART_CR3_HDSEL_POS)
#define HAL_USART_CR3_SCEN_POS         				5
#define HAL_USART_CR3_SCEN							(1 << HAL_USART_CR3_SCEN_POS)
#define HAL_USART_CR3_CTSE_POS         				8
#define HAL_USART_CR3_CTSE							(1 << HAL_USART_CR3_CTSE_POS)
#define HAL_USART_CR3_RTSE_POS         				9
#define HAL_USART_CR3_RTSE							(1 << HAL_USART_CR3_RTSE_POS)
#define HAL_USART_CR3_OVRDIS_POS         			12
#define HAL_USART_CR3_OVRDIS						(1 << HAL_USART_CR3_OVRDIS_POS)

#define HAL_USART_ISR_RXNE_POS         				5
#define HAL_USART_ISR_RXNE							(1 << HAL_USART_ISR_RXNE_POS)
#define HAL_USART_ISR_TC_POS         				6
#define HAL_USART_ISR_TC							(1 << HAL_USART_ISR_TC_POS)
#define HAL_USART_ISR_TXE_POS         				7
#define HAL_USART_ISR_TXE							(1 << HAL_USART_ISR_TXE_POS)

// Default baudrate used to communicate via UART
#define HAL_USART_DEFAULT_BAUDRATE					460800

/* -------------------- INTERFACE -------------------- */

//specific to current settings
#define HAL_USARTDIV(clk, baudrate)				((clk)/(baudrate))
#define HAL_DEFAULT_USART_CLK					HAL_RCC_DEFAULT_CLK_INT

#endif /* __DRIVERS_UART_H__ */