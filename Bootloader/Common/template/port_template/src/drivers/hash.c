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

#include "hal/hal_hash.h"

void hash_sha256_init(){
}

int hash_sha256_update(uint32_t *data, uint32_t length){
	return -1;
}

int hash_sha256_finish(uint32_t *output){
	return -1;
}

void hash_hmac_sha256_init(){	
}

int hash_hmac_sha256_update(uint32_t *data, uint32_t length){
	return -1;
}

int hash_hmac_sha256_set_key(const uint8_t *key, unsigned int key_size){
	return -1;
}

int hash_hmac_sha256_final(const uint8_t *key, unsigned int key_size, uint32_t *tag, uint32_t tag_len){
	return -1;
}