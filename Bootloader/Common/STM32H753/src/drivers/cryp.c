/**
  ******************************************************************************
  * @file    cryp.c
  * @author  CSEM SA
  * @brief   Source file of CRYP module.
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

#include "hal/hal_cryp.h"
#include "hal/hal_rcc.h"

/**
 * Initializes hw for a new AES-CTR encryption/decryption
 */
void cryp_aes_ctr_init(void){
	//make sure CRYP clock is enabled
	HAL_RCC_ENABLE_CRYPT();

	//configure CRYP: 128 bits key, AES-CTR, disabled,
	//8 bits words (so the little->big endian data conversion is automatic)
	//hence data in DIN and DOUT registers can simply be read / written in little-endian
	HAL_CRYP_CR_REG = (HAL_CRYP_CR_REG & ~(HAL_CRYP_CR_CRYPEN | HAL_CRYP_CR_KEYSIZE | HAL_CRYP_CR_DATATYPE | HAL_CRYP_CR_ALGOMODE)) |
					(HAL_CRYP_CR_KEYSIZE_128 | HAL_CRYP_CR_DATATYPE_8 | HAL_CRYP_CR_ALGOMODE_CTR);
}

/**
 * Abort any ongoing AES-CTR encryption/decryption
 */
void cryp_aes_ctr_drop(void){
	//to abandon current AES-CTR, simply disable CRYP and set the FLUSH bit
	BIT_CLEAR(HAL_CRYP_CR_REG, HAL_CRYP_CR_CRYPEN_POS);
	BIT_SET(HAL_CRYP_CR_REG, HAL_CRYP_CR_FFLUSH_POS);
}

/**
 * Configures hw to use the given key
 */
int cryp_aes_ctr_set_key(const uint8_t *k){
	if (k == 0){
		return -1;
	}

	//wait for the end of previous operations, if any
	while (BIT_READ(HAL_CRYP_SR_REG, HAL_CRYP_SR_BUSY_POS) == 1){}

	//disable CRYP
	BIT_CLEAR(HAL_CRYP_CR_REG, HAL_CRYP_CR_CRYPEN_POS);

	//for now, length of the key must always be 16
	uint32_t *key = (uint32_t*) k;

	//our data comes in little-endian fashion, config registers need big-endian
	HAL_CRYP_K2LR_REG = LITTLE_TO_BIG_ENDIAN(key[0]);
	HAL_CRYP_K2RR_REG = LITTLE_TO_BIG_ENDIAN(key[1]);
	HAL_CRYP_K3LR_REG = LITTLE_TO_BIG_ENDIAN(key[2]);
	HAL_CRYP_K3RR_REG = LITTLE_TO_BIG_ENDIAN(key[3]);

	return 0;
}

/**
 * Performs AES-CTR encryption usng the currently setup key
 */
int cryp_aes_ctr_encrypt(uint32_t counter, const uint8_t *in, uint32_t inlen, uint8_t *out){
	//inlen must be a multiple of 16, for now
	if (in == 0 || out == 0 || inlen % 16 != 0){
		return -1;
	}

	//wait for the end of previous operations, if any
	while (BIT_READ(HAL_CRYP_SR_REG, HAL_CRYP_SR_BUSY_POS) == 1){}

	//disable CRYP
	BIT_CLEAR(HAL_CRYP_CR_REG, HAL_CRYP_CR_CRYPEN_POS);

	//IV set to 0, except for the counter
	HAL_CRYP_IV0LR_REG = 0;
	HAL_CRYP_IV0RR_REG = 0;
	HAL_CRYP_IV1LR_REG = 0;
	//counter should be IV0LR according to doc
	//observations showed it is IV1RR...
	//also it is in little-endian, the registers need big-endian
	HAL_CRYP_IV1RR_REG = LITTLE_TO_BIG_ENDIAN(counter);

	//make sure no previous encryption carries over & enable CRYP
	BIT_SET(HAL_CRYP_CR_REG, HAL_CRYP_CR_FFLUSH_POS);
	BIT_SET(HAL_CRYP_CR_REG, HAL_CRYP_CR_CRYPEN_POS);

	uint32_t remaining = inlen;
	uint32_t *in_word = (uint32_t*) in;
	uint32_t *out_word = (uint32_t*) out;

	while (remaining > 0){
		//wait until more data can be sent
		//"Input FIFO Not Full"
		while (BIT_READ(HAL_CRYP_SR_REG, HAL_CRYP_SR_IFNF_POS) != 1){}

		//write block
		for (int i=0; i<4; ++i){
			HAL_CRYP_DIN_REG = *in_word;
			in_word++;
		}

		//wait until out data is available
		//"Output FIFO Not Empty"
		while (BIT_READ(HAL_CRYP_SR_REG, HAL_CRYP_SR_OFNE_POS) != 1){}

		//read block
		for (int i=0; i<4; ++i){
			*out_word = HAL_CRYP_DOUT_REG;
			out_word++;
		}

		remaining -= 4 * sizeof(uint32_t);
	}

	//wait until end of any other operation
	while (BIT_READ(HAL_CRYP_SR_REG, HAL_CRYP_SR_BUSY_POS) == 1){}

	//disable CRYP
	BIT_CLEAR(HAL_CRYP_CR_REG, HAL_CRYP_CR_CRYPEN_POS);

	return 0;
}