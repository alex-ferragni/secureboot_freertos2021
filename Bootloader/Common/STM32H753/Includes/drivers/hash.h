/**
  ******************************************************************************
  * @file    hash.h
  * @author  CSEM SA
  * @brief   Header file of HASH module.
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

#ifndef __DRIVERS_HASH_H__
#define __DRIVERS_HASH_H__

#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_HASH_BASE							0x48021400

#define HAL_HASH_HR_COUNT						8

#define HAL_HASH_CR_OFFSET						0x000
#define HAL_HASH_DIN_OFFSET						0x004
#define HAL_HASH_STR_OFFSET						0x008
#define HAL_HASH_SR_OFFSET						0x024
#define HAL_HASH_HR_OFFSET(i)					(0x310+0x04*(i))

#define HAL_HASH_CR_REG							REG_CONTENT(HAL_HASH_BASE, HAL_HASH_CR_OFFSET)
#define HAL_HASH_DIN_REG						REG_CONTENT(HAL_HASH_BASE, HAL_HASH_DIN_OFFSET)
#define HAL_HASH_STR_REG						REG_CONTENT(HAL_HASH_BASE, HAL_HASH_STR_OFFSET)
#define HAL_HASH_SR_REG							REG_CONTENT(HAL_HASH_BASE, HAL_HASH_SR_OFFSET)
#define HAL_HASH_HR_REG(i)						REG_CONTENT(HAL_HASH_BASE, HAL_HASH_HR_OFFSET(i))


#define HAL_HASH_CR_INIT_POS					2
#define HAL_HASH_CR_INIT						(1 << HAL_HASH_CR_INIT_POS)
#define HAL_HASH_CR_DATATYPE_POS				4
#define HAL_HASH_CR_DATATYPE					(0x3 << HAL_HASH_CR_DATATYPE_POS)
#define HAL_HASH_CR_MODE_POS					6
#define HAL_HASH_CR_MODE						(1 << HAL_HASH_CR_MODE_POS)
#define HAL_HASH_CR_ALGO0_POS					7
#define HAL_HASH_CR_DINNE_POS					12
#define HAL_HASH_CR_DINNE						(1 << HAL_HASH_CR_DINNE_POS)
#define HAL_HASH_CR_ALGO1_POS					18
#define HAL_HASH_CR_ALGO						(1 << HAL_HASH_CR_ALGO0_POS | 1 << HAL_HASH_CR_ALGO1_POS)

#define HAL_HASH_CR_DATATYPE_8					(0x2 << HAL_HASH_CR_DATATYPE_POS)
#define HAL_HASH_CR_DATATYPE_32					(0 << HAL_HASH_CR_DATATYPE_POS)
#define HAL_HASH_CR_MODE_HASH					(0 << HAL_HASH_CR_MODE_POS)
#define HAL_HASH_CR_MODE_HMAC					(1 << HAL_HASH_CR_MODE_POS)
#define HAL_HASH_CR_ALGO_SHA256					HAL_HASH_CR_ALGO

#define HAL_HASH_STR_NBLW_POS					0
#define HAL_HASH_STR_NBLW						(0x1f << HAL_HASH_STR_NBLW_POS)
#define HAL_HASH_STR_DCAL_POS					8
#define HAL_HASH_STR_DCAL						(1 << HAL_HASH_STR_DCAL_POS)

#define HAL_HASH_SR_DINIS_POS					0
#define HAL_HASH_SR_DINIS						(1 << HAL_HASH_SR_DINIS_POS)
#define HAL_HASH_SR_DCIS_POS					1
#define HAL_HASH_SR_DCIS						(1 << HAL_HASH_SR_DCIS_POS)


/* -------------------- INTERFACE -------------------- */

#endif /* __DRIVERS_HASH_H__ */