/**
  ******************************************************************************
  * @file    uart.c
  * @author  CSEM SA
  * @brief   Source file of UART module.
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

/* Private        ------------------------------------------------------------*/

#include "hal/hal.h"
#include "hal/hal_uart.h"

void UART_WaitOnFlag(uint32_t Bit)
{
	/* Wait until flag is set */
	while (BIT_READ(HAL_USART3_ISR_REG, Bit) == 0)
	{
	}
}

/* Public         ------------------------------------------------------------*/
uint32_t HAL_UART_init(void){

	ENABLE_CLK_USART();
	SET_GPIO_MOD_ALT(D, 8);
	SET_GPIO_MOD_ALT(D, 9);
	SET_GPIO_ALT_FUNC(D, 8, 7);
	SET_GPIO_ALT_FUNC(D, 9, 7);

	BIT_CLEAR(HAL_USART3_CR1_REG, HAL_USART_CR1_UE_POS);

	uint32_t tmpreg = 0;

	tmpreg |= (0 << HAL_USART_CR1_M0_POS) | (0 << HAL_USART_CR1_M1_POS) | //8 bits word length
			(0 << HAL_USART_CR1_PCE_POS) | //no parity bits
			HAL_USART_CR1_RE | //TX enabled
			HAL_USART_CR1_TE; //RX enabled

	// Clear values & apply them
	HAL_USART3_CR1_REG = (HAL_USART3_CR1_REG & ~(HAL_USART_CR1_M0 | 
										HAL_USART_CR1_M1 |
										HAL_USART_CR1_PCE  | 
										HAL_USART_CR1_PS | 
										HAL_USART_CR1_RE | 
										HAL_USART_CR1_TE | 
										HAL_USART_CR1_OVER8 | 
										HAL_USART_CR1_FIFOEN)) | tmpreg;

	HAL_USART3_CR2_REG &= ~(HAL_USART_CR2_STOP | //1 stop bit
						HAL_USART_CR2_LINEN | HAL_USART_CR2_CLKEN); 

	HAL_USART3_CR3_REG = (HAL_USART3_CR3_REG & ~(HAL_USART_CR3_CTSE | HAL_USART_CR3_RTSE |//no hardware  flow control
						HAL_USART_CR3_SCEN | HAL_USART_CR3_HDSEL | HAL_USART_CR3_IREN)) |
						HAL_USART_CR3_OVRDIS; 

	HAL_USART3_BRR_REG = HAL_USARTDIV(HAL_DEFAULT_USART_CLK, HAL_USART_DEFAULT_BAUDRATE);


	BIT_SET(HAL_USART3_CR1_REG, HAL_USART_CR1_UE_POS);

	return RET_OK;
}

uint32_t HAL_UART_deinit(void){

	HAL_USART3_CR1_REG &= ~HAL_USART_CR1_UE;

	HAL_USART3_CR1_REG = 0;
	HAL_USART3_CR2_REG = 0;
	HAL_USART3_CR3_REG = 0;

	SET_GPIO_MOD_ANALOG(D,8);
	SET_GPIO_MOD_ANALOG(D,9);

	DISABLE_CLK_USART();

	return RET_OK;
}

uint32_t HAL_UART_Transmit(uint8_t *pData, uint16_t Size)
{
	if ((pData == NULL) || (Size == 0U))
	{
		return  RET_ERROR;
	}

	uint16_t TxXferCount = Size;

	while (TxXferCount > 0U)
	{
		// Wait for transmit ready
		UART_WaitOnFlag(HAL_USART_ISR_TXE_POS);

		HAL_USART3_TDR_REG = (uint8_t)(*pData & 0xFFU);
		pData++;

		TxXferCount--;
	}

	// Wait for end of transmit
	UART_WaitOnFlag(HAL_USART_ISR_TC_POS);

	return RET_OK;
}

uint32_t HAL_UART_Receive(uint8_t *pData, uint16_t Size)
{
	if ((pData == NULL) || (Size == 0U))
	{
		return  RET_ERROR;
	}
	
	uint16_t RxXferCount = Size;

	/* as long as data have to be received */
	while (RxXferCount > 0U)
	{
		// Wait for receive ready
		UART_WaitOnFlag(HAL_USART_ISR_RXNE_POS);

		*pData = HAL_USART3_RDR_REG & 0xFF;
		pData++;

		RxXferCount--;
	}

	return RET_OK;
}