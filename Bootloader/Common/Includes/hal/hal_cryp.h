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

#ifndef __HAL_CRYP_H__
#define __HAL_CRYP_H__

#include "hal/hal_config.h"

#ifdef HAL_INCLUDE_CRYP

#include "drivers/cryp.h"

/**
 * @brief Initializes hw for a new AES-CTR encryption/decryption
 */
void cryp_aes_ctr_init(void);

/**
 * @brief Abort any ongoing AES-CTR encryption/decryption
 */
void cryp_aes_ctr_drop(void);

/**
 * @brief Configures hw to use the given key
 *
 * @param k The 16 bytes encryption/decryption key
 *
 * @return 0 on success, -1 on error
 */
int cryp_aes_ctr_set_key(const uint8_t *k);

/**
 * @brief Performs AES-CTR encryption using the currently setup key
 *
 * @param counter The block counter of the first block
 * @param in The input data buffer
 * @param inlen The length (in bytes) of the input and output buffers
 * @param out The output data buffer
 *
 * @return 0 on success, -1 on error
 */
int cryp_aes_ctr_encrypt(uint32_t counter, const uint8_t *in, uint32_t inlen, uint8_t *out);

#endif /* HAL_INCLUDE_CRYP */

#endif /* __HAL_CRYP_H__ */