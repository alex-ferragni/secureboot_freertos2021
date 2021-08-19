/**
  ******************************************************************************
  * @file    cryp.h
  * @author  CSEM SA
  * @brief   Header file of CRYP module.
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

#ifndef __DRIVERS_CRYP_H__
#define __DRIVERS_CRYP_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_CRYP_BASE							0x48021000

#define HAL_CRYP_CR_OFFSET						0x000
#define HAL_CRYP_SR_OFFSET						0x004
#define HAL_CRYP_DIN_OFFSET						0x008
#define HAL_CRYP_DOUT_OFFSET					0x00C
#define HAL_CRYP_K2LR_OFFSET					0x030
#define HAL_CRYP_K2RR_OFFSET					0x034
#define HAL_CRYP_K3LR_OFFSET					0x038
#define HAL_CRYP_K3RR_OFFSET					0x03C
#define HAL_CRYP_IV0LR_OFFSET					0x040
#define HAL_CRYP_IV0RR_OFFSET					0x044
#define HAL_CRYP_IV1LR_OFFSET					0x048
#define HAL_CRYP_IV1RR_OFFSET					0x04C

#define HAL_CRYP_CR_REG							REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_CR_OFFSET)
#define HAL_CRYP_SR_REG							REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_SR_OFFSET)
#define HAL_CRYP_DIN_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_DIN_OFFSET)
#define HAL_CRYP_DOUT_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_DOUT_OFFSET)
#define HAL_CRYP_K2LR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_K2LR_OFFSET)
#define HAL_CRYP_K2RR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_K2RR_OFFSET)
#define HAL_CRYP_K3LR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_K3LR_OFFSET)
#define HAL_CRYP_K3RR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_K3RR_OFFSET)
#define HAL_CRYP_IV0LR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_IV0LR_OFFSET)
#define HAL_CRYP_IV0RR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_IV0RR_OFFSET)
#define HAL_CRYP_IV1LR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_IV1LR_OFFSET)
#define HAL_CRYP_IV1RR_REG						REG_CONTENT(HAL_CRYP_BASE, HAL_CRYP_IV1RR_OFFSET)


#define HAL_CRYP_CR_ALGOMODE_POS				3
#define HAL_CRYP_CR_DATATYPE_POS				6
#define HAL_CRYP_CR_DATATYPE					(0x3 << HAL_CRYP_CR_DATATYPE_POS)
#define HAL_CRYP_CR_KEYSIZE_POS					8
#define HAL_CRYP_CR_KEYSIZE						(0x3 << HAL_CRYP_CR_KEYSIZE_POS)
#define HAL_CRYP_CR_FFLUSH_POS					14
#define HAL_CRYP_CR_FFLUSH						(1 << HAL_CRYP_CR_FFLUSH_POS)
#define HAL_CRYP_CR_CRYPEN_POS					15
#define HAL_CRYP_CR_CRYPEN						(1 << HAL_CRYP_CR_CRYPEN_POS)
#define HAL_CRYP_CR_ALGOMODE3_POS				19
#define HAL_CRYP_CR_ALGOMODE					(0x7 << HAL_CRYP_CR_ALGOMODE_POS | 1 << HAL_CRYP_CR_ALGOMODE3_POS)

#define HAL_CRYP_SR_IFNF_POS					1
#define HAL_CRYP_SR_IFNF						(1 << HAL_CRYP_SR_IFNF_POS)
#define HAL_CRYP_SR_OFNE_POS					2
#define HAL_CRYP_SR_OFNE						(1 << HAL_CRYP_SR_OFNE_POS)
#define HAL_CRYP_SR_BUSY_POS					4
#define HAL_CRYP_SR_BUSY						(1 << HAL_CRYP_SR_BUSY_POS)

#define HAL_CRYP_CR_DATATYPE_8					(0x2 << HAL_CRYP_CR_DATATYPE_POS)
#define HAL_CRYP_CR_DATATYPE_32					(0 << HAL_CRYP_CR_DATATYPE_POS)
#define HAL_CRYP_CR_KEYSIZE_128					(0 << HAL_CRYP_CR_KEYSIZE_POS)
#define HAL_CRYP_CR_ALGOMODE_CTR				(0 << HAL_CRYP_CR_ALGOMODE3_POS | 0x6 << HAL_CRYP_CR_ALGOMODE_POS)

/* -------------------- INTERFACE -------------------- */

#endif /* __DRIVERS_CRYP_H__ */