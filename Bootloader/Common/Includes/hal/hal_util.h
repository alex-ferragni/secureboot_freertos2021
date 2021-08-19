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

#ifndef __HAL_UTIL_H__
#define __HAL_UTIL_H__

#include "drivers/util.h"

#include <stdint.h>

// Return codes
#define RET_OK         0
#define RET_ERROR      -1
#define RET_BUSY       2

// Make sure we refresh IO values
#define _IO                                volatile

// Shortcut to read data at an address given by base + offset values
#define REG_CONTENT(base,offset)            (*(uint32_t _IO*)((base) + (offset)))

/**
 * @brief Reads a single bit from a register
 *
 * @param reg The register
 * @param bit The bit position
 *
 * @return 0 or 1
 */
#define BIT_READ(reg,bit)                   (((reg) >> (bit)) & 1)

/**
 * @brief Sets ( = set to 1) a single bit from a register
 *
 * @param reg The register
 * @param bit The bit position
 */
#define BIT_SET(reg,bit)                    (reg) |= (1 << (bit))

/**
 * @brief Clears ( = set to 0) a single bit from a register
 *
 * @param reg The register
 * @param bit The bit position
 */
#define BIT_CLEAR(reg,bit)                  (reg) &= (~(1 << (bit)))

/**
 * @brief Writes a value to a register
 *
 * @param reg The register
 * @param value The value to be written
 */
#define REG_WRITE(reg,value)                (reg) = (value)

/**
 * @brief Converts a 32-bit word from little-endian to big-endian (or inversely)
 *
 * @param reg The register
 * @param bit The bit position
 *
 * @return The value in the other endianess
 */
#define LITTLE_TO_BIG_ENDIAN(word)			((((word) & 0xff) << 24) | \
											(((word) & 0xff00) << 8) | \
											(((word) & 0xff0000) >> 8) | \
											(((word) & 0xff000000) >> 24))

/**
 * @brief Do nothing
 */
#define PASS								do{}while(0)

/**
 * @brief Do nothing. Can be used outside of any function
 */
#define GLOBAL_PASS							struct GlobalScopeNoopTrick

#endif /* __HAL_UTIL_H__ */