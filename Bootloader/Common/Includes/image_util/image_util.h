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

#ifndef __IMAGE_DOWNLOAD_H__
#define __IMAGE_DOWNLOAD_H__

#include <stdint.h>

//only tests should directly write to primary slot
#ifdef ENABLE_TEST

/**
 * @brief Downloads an image directly in primary slot (only intended to be used by tests!) using UART
 * Intended to be used with the image_upload.py script
 *
 * @return RET_OK on success, RET_ERROR on error
 */
int download_image_primary(void);
#endif /* ENABLE_TEST */

#ifdef ENABLE_FLASH_CORRUPTION

/**
 * @brief Corrupts a flash word (Shouldnever be included directly in the bootloader for safety reasons)
 * 
 * @param target_address The address that will be corrupted
 */
void corrupt_flash(uint32_t target_address);
#endif /* ENABLE_FLASH_CORRUPTION */

void boot_from_image(uint32_t* image_content_start);

/**
 * @brief Downloads an image in secondary slot using UART
 * Intended to be used with the image_upload.py script
 *
 * @return RET_OK on success, RET_ERROR on error
 */
int download_image_secondary(void);

/**
 * @brief Prints the content of memory
 *
 * @param start_address The flash starting target address
 * @param end_address The last flash address (not inclusive)
 * @param length The length (in bytes) of the output buffer; number of bytes that will be read
 */
void print_memory_content(uint32_t* start_address, uint32_t end_address);

/**
 * @brief Uploads the entire content of the primary slot via UART
 * Intended to be used with the image_download.py script
 */
int send_primary_image(void);

/**
 * @brief Uploads the entire content of the secondary slot via UART
 * Intended to be used with the image_download.py script
 */
int send_secondary_image(void);

/**
 * @brief Displays information about the current pc location (and the corresponding flash area)
 */
void print_current_location(void);

#endif /* __IMAGE_DOWNLOAD_H__ */