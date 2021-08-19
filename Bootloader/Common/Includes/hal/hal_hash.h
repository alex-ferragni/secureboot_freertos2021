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

#ifndef __HAL_HASH_H__
#define __HAL_HASH_H__

#include "hal/hal_config.h"

#ifdef HAL_INCLUDE_HASH

#include <stdint.h>
#include "drivers/hash.h"

/**
 * @brief Initializes the hash processor for SHA256
 */
void hash_sha256_init();

/**
 * @brief Processes a block to the current digest
 *
 * @param data The input data buffer
 * @param length The length (in bytes) of the input buffer; number of bytes that will be processed
 *
 * @return 0 on success, -1 on error
 */
int hash_sha256_update(uint32_t *data, uint32_t length);

/**
 * @brief Finishes the current digest computation and outputs it
 *
 * @param output The output data buffer, of size BOOTUTIL_CRYPTO_SHA256_DIGEST_SIZE
 *
 * @return 0 on success, -1 on error
 */
int hash_sha256_finish(uint32_t *output);

/**
 * @brief Initializes the hash processor for HMAC-SHA256
 */
void hash_hmac_sha256_init();

/**
 * @brief Processes a block to the current digest
 *
 * @param data The input data buffer
 * @param length The length (in bytes) of the input buffer; number of bytes that will be processed
 *
 * @return 0 on success, -1 on error
 */
int hash_hmac_sha256_update(uint32_t *data, uint32_t length);

/**
 * @brief Sets the HMAC-SHA256 key
 *
 * @param key The input key
 * @param key_size The length (in bytes) of the input key
 *
 * @return 0 on success, -1 on error
 */
int hash_hmac_sha256_set_key(const uint8_t *key, unsigned int key_size);

/**
 * @brief Finishes the current digest computation using the given key and outputs the digest
 *
 * @param key The input key used to complete the computation
 * @param key_size The size of the input key
 * @param tag The output data buffer
 * @param tag_len The length of the output data buffer
 *
 * @return 0 on success, -1 on error
 */
int hash_hmac_sha256_final(const uint8_t *key, unsigned int key_size, uint32_t *tag, uint32_t tag_len);

#endif /* HAL_INCLUDE_HASH */

#endif /* __HAL_HASH_H__ */