/**
  ******************************************************************************
  * @file    hash.c
  * @author  CSEM SA
  * @brief   Source file of HASH module.
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

#include "hal/hal_hash.h"
#include "hal/hal_rcc.h"
#include "hal/hal_uart.h"

void hash_sha256_init(){
	//make sure hahs clock is enabled
	BIT_SET(HAL_RCC_AHB2ENR_REG, HAL_RCC_AHB2ENR_HASHEN_POS);

	//reset size of last word (unkown yet)
	HAL_HASH_STR_REG &= ~HAL_HASH_STR_NBLW;

	//SHA256, 32 bits words,
	HAL_HASH_CR_REG = (HAL_HASH_CR_REG & ~(HAL_HASH_CR_ALGO | HAL_HASH_CR_DATATYPE | HAL_HASH_CR_MODE)) | 
					(HAL_HASH_CR_ALGO_SHA256 | HAL_HASH_CR_DATATYPE_32 | HAL_HASH_CR_MODE_HASH | HAL_HASH_CR_INIT);
}

int hash_sha256_update(uint32_t *data, uint32_t length){
	uint32_t processed = 0;
	HAL_HASH_STR_REG &= ~HAL_HASH_STR_NBLW;
	while (processed < length){
		while (BIT_READ(HAL_HASH_SR_REG, HAL_HASH_SR_DINIS_POS) != 1 && BIT_READ(HAL_HASH_CR_REG, HAL_HASH_CR_DINNE_POS) != 1){}

		while (length - processed >= sizeof(uint32_t)){
			REG_WRITE(HAL_HASH_DIN_REG, LITTLE_TO_BIG_ENDIAN(*data));
			processed += sizeof(uint32_t);
			data++;
		}
		if (length != processed){
			HAL_HASH_STR_REG = (HAL_HASH_STR_REG & ~HAL_HASH_STR_NBLW) | ((length - processed)*8 << HAL_HASH_STR_NBLW_POS);
			REG_WRITE(HAL_HASH_DIN_REG, LITTLE_TO_BIG_ENDIAN(*data));
			processed = length;
		}
	}

	return 0;
}

int hash_sha256_finish(uint32_t *output){
	BIT_SET(HAL_HASH_STR_REG, HAL_HASH_STR_DCAL_POS);
	while (BIT_READ(HAL_HASH_SR_REG, HAL_HASH_SR_DCIS_POS) != 1){}

	for (int i=0; i<HAL_HASH_HR_COUNT; ++i){
		uint32_t res = HAL_HASH_HR_REG(i);
		*output = LITTLE_TO_BIG_ENDIAN(res);
		output++;
	}

	return 0;
}

void hash_hmac_sha256_init(){
	BIT_SET(HAL_RCC_AHB2ENR_REG, HAL_RCC_AHB2ENR_HASHEN_POS);
	HAL_HASH_STR_REG &= ~HAL_HASH_STR_NBLW;
	HAL_HASH_CR_REG = (HAL_HASH_CR_REG & ~(HAL_HASH_CR_ALGO | HAL_HASH_CR_DATATYPE | HAL_HASH_CR_MODE)) | 
					(HAL_HASH_CR_ALGO_SHA256 | HAL_HASH_CR_DATATYPE_32 | HAL_HASH_CR_MODE_HMAC | HAL_HASH_CR_INIT);
}

int hash_hmac_sha256_set_key(const uint8_t *key, unsigned int key_size){
	uint32_t processed = 0;
	uint32_t *data = (uint32_t*) key;
	HAL_HASH_STR_REG &= ~HAL_HASH_STR_NBLW;
	while (processed < key_size){
		while (BIT_READ(HAL_HASH_SR_REG, HAL_HASH_SR_DINIS_POS) != 1 && BIT_READ(HAL_HASH_CR_REG, HAL_HASH_CR_DINNE_POS) != 1){}

		while (key_size - processed >= sizeof(uint32_t)){
			REG_WRITE(HAL_HASH_DIN_REG, LITTLE_TO_BIG_ENDIAN(*data));
			processed += sizeof(uint32_t);
			data++;
		}
	}
	BIT_SET(HAL_HASH_STR_REG, HAL_HASH_STR_DCAL_POS);

	return 0;
}

int hash_hmac_sha256_update(uint32_t *data, uint32_t length){
	uint32_t processed = 0;
	HAL_HASH_STR_REG &= ~HAL_HASH_STR_NBLW;
	while (processed < length){
		while (BIT_READ(HAL_HASH_SR_REG, HAL_HASH_SR_DINIS_POS) != 1 && BIT_READ(HAL_HASH_CR_REG, HAL_HASH_CR_DINNE_POS) != 1){}

		
		while (length - processed >= sizeof(uint32_t)){
			REG_WRITE(HAL_HASH_DIN_REG, LITTLE_TO_BIG_ENDIAN(*data));
			processed += sizeof(uint32_t);
			data++;
		}
		if (length != processed){
			HAL_HASH_STR_REG = (HAL_HASH_STR_REG & ~HAL_HASH_STR_NBLW) | ((length - processed)*8 << HAL_HASH_STR_NBLW_POS);
			REG_WRITE(HAL_HASH_DIN_REG, LITTLE_TO_BIG_ENDIAN(*data));
			processed = length;
		}
	}

	return 0;
}

int hash_hmac_sha256_final(const uint8_t *key, unsigned int key_size, uint32_t *tag, uint32_t tag_len){
	BIT_SET(HAL_HASH_STR_REG, HAL_HASH_STR_DCAL_POS);
	hash_hmac_sha256_set_key(key, key_size);

	while (BIT_READ(HAL_HASH_SR_REG, HAL_HASH_SR_DCIS_POS) != 1){}

	for (int i=0; i<tag_len/sizeof(uint32_t); ++i){
		uint32_t res = HAL_HASH_HR_REG(i);
		*tag = LITTLE_TO_BIG_ENDIAN(res);
		tag++;
	}

	return 0;
}