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

#include <stdint.h>
#include <stdio.h>
#include "hal/hal_flash.h"
#include "sysflash/sysflash.h"

/* Private        ------------------------------------------------------------*/

/* Public         ------------------------------------------------------------*/

#ifdef ENABLE_FLASH_CORRUPTION
uint32_t HAL_FLASH_Program(uint32_t* flash_address, uint32_t const* data_address, uint32_t write_size){
	return RET_ERROR;
}
#endif /* ENABLE_FLASH_CORRUPTION */

uint32_t HAL_FLASH_Unlock(void) {
    return RET_ERROR;
}

uint32_t HAL_FLASH_Lock(void) {
    return RET_ERROR;
}

uint32_t minimal_flash_write(uint32_t addr, const void *buf, size_t length) {
	return RET_ERROR;
}

uint32_t minimal_flash_erase_sector(uint32_t addr) {
	return RET_ERROR;
}

uint32_t minimal_flash_read(uint32_t addr, void* buf, size_t length){
	return RET_ERROR;	
}