/**
  ******************************************************************************
  * @file    flash.h
  * @author  CSEM SA
  * @brief   Header file of FLASH module.
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

#ifndef __DRIVERS_FLASH_H__
#define __DRIVERS_FLASH_H__

#include <stddef.h>
#include "hal/hal_util.h"

/* -------------------- REGISTERS -------------------- */

#define HAL_FLASH_BASE                         0x52002000

#define HAL_FLASH_KEYR1_OFFSET                  0x004
#define HAL_FLASH_OPTKEYR_OFFSET				0x008
#define HAL_FLASH_CR1_OFFSET                    0x00C
#define HAL_FLASH_SR1_OFFSET                    0x010
#define HAL_FLASH_CCR1_OFFSET                   0x014
#define HAL_FLASH_OPTCR_OFFSET                  0x018
#define HAL_FLASH_OPTSR_OFFSET                  0x020
#define HAL_FLASH_SCAR1_OFFSET                  0x034
#define HAL_FLASH_KEYR2_OFFSET                  0x104
#define HAL_FLASH_CR2_OFFSET                    0x10C
#define HAL_FLASH_SR2_OFFSET                    0x110
#define HAL_FLASH_CCR2_OFFSET                   0x114

#define HAL_FLASH_KEYR1_REG                     REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_KEYR1_OFFSET)
#define HAL_FLASH_OPTKEYR_REG                   REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_OPTKEYR_OFFSET)
#define HAL_FLASH_CR1_REG                       REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_CR1_OFFSET)
#define HAL_FLASH_SR1_REG                       REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_SR1_OFFSET)
#define HAL_FLASH_CCR1_REG                      REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_CCR1_OFFSET)
#define HAL_FLASH_OPTCR_REG                     REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_OPTCR_OFFSET)
#define HAL_FLASH_OPTSR_REG                     REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_OPTSR_OFFSET)
#define HAL_FLASH_SCAR1_REG                     REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_SCAR1_OFFSET)
#define HAL_FLASH_KEYR2_REG                     REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_KEYR2_OFFSET)
#define HAL_FLASH_CR2_REG                       REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_CR2_OFFSET)
#define HAL_FLASH_SR2_REG                       REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_SR2_OFFSET)
#define HAL_FLASH_CCR2_REG                      REG_CONTENT(HAL_FLASH_BASE,HAL_FLASH_CCR2_OFFSET)



#define HAL_FLASH_CR_LOCK_POS                   0
#define HAL_FLASH_CR_PG_POS                     1
#define HAL_FLASH_CR_PG                         (1 << HAL_FLASH_CR_PG_POS)        /*!< 0x00000002 */
#define HAL_FLASH_CR_SER_POS                    2
#define HAL_FLASH_CR_SER                        (1 << HAL_FLASH_CR_SER_POS)
#define HAL_FLASH_CR_PSIZE_POS                  4
#define HAL_FLASH_CR_PSIZE                      (3 << HAL_FLASH_CR_PSIZE_POS)     /*!< 0x00000030 */
#define HAL_FLASH_CR_START_POS                  7
#define HAL_FLASH_CR_START                      (1 << HAL_FLASH_CR_START_POS)     /*!< 0x00000080 */
#define HAL_FLASH_CR_SNB_POS                    8
#define HAL_FLASH_CR_SNB                        (7 << HAL_FLASH_CR_SNB_POS)


#define HAL_FLASH_SR_BSY_POS                    0
#define HAL_FLASH_SR_BSY                        (1 << HAL_FLASH_SR_BSY_POS)
#define HAL_FLASH_SR_QW_POS                     2
#define HAL_FLASH_SR_QW                         (1 << HAL_FLASH_SR_QW_POS)
#define HAL_FLASH_SR_EOP_POS                    16
#define HAL_FLASH_SR_EOP                        (1 << HAL_FLASH_SR_EOP_POS)       /*!< 0x00010000 */
#define HAL_FLASH_SR_WRPERR_POS                 17
#define HAL_FLASH_SR_WRPERR                     (1 << HAL_FLASH_SR_WRPERR_POS)    /*!< 0x00020000 */
#define HAL_FLASH_SR_PGSERR_POS                 18
#define HAL_FLASH_SR_PGSERR                     (1 << HAL_FLASH_SR_PGSERR_POS)    /*!< 0x00040000 */
#define HAL_FLASH_SR_STRBERR_POS                19
#define HAL_FLASH_SR_STRBERR                    (1 << HAL_FLASH_SR_STRBERR_POS)   /*!< 0x00080000 */
#define HAL_FLASH_SR_INCERR_POS                 21
#define HAL_FLASH_SR_INCERR                     (1 << HAL_FLASH_SR_INCERR_POS)    /*!< 0x00200000 */
#define HAL_FLASH_SR_OPERR_POS                  22
#define HAL_FLASH_SR_OPERR                      (1 << HAL_FLASH_SR_OPERR_POS)     /*!< 0x00400000 */
#define HAL_FLASH_SR_RDPERR_POS                 23
#define HAL_FLASH_SR_RDPERR                     (1 << HAL_FLASH_SR_RDPERR_POS)    /*!< 0x00800000 */
#define HAL_FLASH_SR_RDSERR_POS                 24
#define HAL_FLASH_SR_RDSERR                     (1 << HAL_FLASH_SR_RDSERR_POS)    /*!< 0x01000000 */
#define HAL_FLASH_SR_SNECCERR_POS               25
#define HAL_FLASH_SR_SNECCERR                   (1 << HAL_FLASH_SR_SNECCERR_POS)  /*!< 0x02000000 */
#define HAL_FLASH_SR_DBECCERR_POS               26
#define HAL_FLASH_SR_DBECCERR                   (1 << HAL_FLASH_SR_DBECCERR_POS)  /*!< 0x04000000 */
#define HAL_FLASH_SR_CRCRDERR_POS               28
#define HAL_FLASH_SR_CRCRDERR                   (1 << HAL_FLASH_SR_CRCRDERR_POS)  /*!< 0x10000000 */

#define HAL_FLASH_OPTCR_OPTLOCK_POS				0
#define HAL_FLASH_OPTCR_OPTLOCK					(1 << HAL_FLASH_OPTCR_OPTLOCK_POS)
#define HAL_FLASH_OPTCR_OPTSTART_POS			1
#define HAL_FLASH_OPTCR_OPTSTART				(1 << HAL_FLASH_OPTCR_OPTSTART_POS)

#define HAL_FLASH_OPTSR_RDP_POS					8
#define HAL_FLASH_OPTSR_RDP						(0xFF << HAL_FLASH_OPTSR_RDP_POS)
#define HAL_FLASH_OPTSR_RDP0					(0xAA << HAL_FLASH_OPTSR_RDP_POS)
#define HAL_FLASH_OPTSR_RDP1					(0xBB << HAL_FLASH_OPTSR_RDP_POS) 
//don't define RDP2, just in case
#define HAL_FLASH_OPTSR_SECURITY_POS			21
#define HAL_FLASH_OPTSR_SECURITY				(1 << HAL_FLASH_OPTSR_SECURITY_POS)

#define HAL_FLASH_SCAR1_SEC_AREA_START1_POS		0
#define HAL_FLASH_SCAR1_SEC_AREA_START1			(0xFFF << HAL_FLASH_SCAR1_SEC_AREA_START1_POS)
#define HAL_FLASH_SCAR1_SEC_AREA_END1_POS		16
#define HAL_FLASH_SCAR1_SEC_AREA_END1			(0xFFF << HAL_FLASH_SCAR1_SEC_AREA_END1_POS)
#define HAL_FLASH_SCAR1_DMES1_POS				31
#define HAL_FLASH_SCAR1_DMES1					(1 << HAL_FLASH_SCAR1_DMES1_POS)

#define HAL_FLASH_FLAG_WRPERR_BANK1             HAL_FLASH_SR_WRPERR          /*!< Write Protection Error on flag */
#define HAL_FLASH_FLAG_PGSERR_BANK1             HAL_FLASH_SR_PGSERR          /*!< Program Sequence Error on flag */
#define HAL_FLASH_FLAG_STRBERR_BANK1            HAL_FLASH_SR_STRBERR         /*!< Strobe Error flag */
#define HAL_FLASH_FLAG_INCERR_BANK1             HAL_FLASH_SR_INCERR          /*!< Inconsistency Error on flag */
#define HAL_FLASH_FLAG_OPERR_BANK1              HAL_FLASH_SR_OPERR           /*!< Operation Error on flag */
#define HAL_FLASH_FLAG_RDPERR_BANK1             HAL_FLASH_SR_RDPERR          /*!< Read Protection Error on flag */
#define HAL_FLASH_FLAG_RDSERR_BANK1             HAL_FLASH_SR_RDSERR          /*!< Read Secured Error on flag */
#define HAL_FLASH_FLAG_SNECCERR_BANK1           HAL_FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on flag */
#define HAL_FLASH_FLAG_DBECCERR_BANK1           HAL_FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on flag */
#define HAL_FLASH_FLAG_CRCEND_BANK1             HAL_FLASH_SR_CRCEND          /*!< CRC End of Calculation flag */
#define HAL_FLASH_FLAG_CRCRDERR_BANK1           HAL_FLASH_SR_CRCRDERR        /*!< CRC Read Error on bank flag */


#define HAL_FLASH_FLAG_ALL_ERRORS_BANK1         (HAL_FLASH_FLAG_WRPERR_BANK1   | HAL_FLASH_FLAG_PGSERR_BANK1   | \
                                                HAL_FLASH_FLAG_STRBERR_BANK1  | HAL_FLASH_FLAG_INCERR_BANK1   | \
                                                HAL_FLASH_FLAG_OPERR_BANK1    | HAL_FLASH_FLAG_RDPERR_BANK1   | \
                                                HAL_FLASH_FLAG_RDSERR_BANK1   | HAL_FLASH_FLAG_SNECCERR_BANK1 | \
                                                HAL_FLASH_FLAG_DBECCERR_BANK1 | HAL_FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */

#define HAL_FLASH_FLAG_WRPERR_BANK2             (HAL_FLASH_SR_WRPERR   | 0x80000000U)        /*!< Write Protection Error on Bank 2 flag */
#define HAL_FLASH_FLAG_PGSERR_BANK2             (HAL_FLASH_SR_PGSERR   | 0x80000000U)        /*!< Program Sequence Error on Bank 2 flag */
#define HAL_FLASH_FLAG_STRBERR_BANK2            (HAL_FLASH_SR_STRBERR  | 0x80000000U)        /*!< Strobe Error on Bank 2 flag */
#define HAL_FLASH_FLAG_INCERR_BANK2             (HAL_FLASH_SR_INCERR   | 0x80000000U)        /*!< Inconsistency Error on Bank 2 flag */
#define HAL_FLASH_FLAG_OPERR_BANK2              (HAL_FLASH_SR_OPERR    | 0x80000000U)        /*!< Operation Error on Bank 2 flag */
#define HAL_FLASH_FLAG_RDPERR_BANK2             (HAL_FLASH_SR_RDPERR   | 0x80000000U)        /*!< Read Protection Error on Bank 2 flag */
#define HAL_FLASH_FLAG_RDSERR_BANK2             (HAL_FLASH_SR_RDSERR   | 0x80000000U)        /*!< Read Secured Error on Bank 2 flag */
#define HAL_FLASH_FLAG_SNECCERR_BANK2           (HAL_FLASH_SR_SNECCERR | 0x80000000U)        /*!< Single ECC Error Correction on Bank 2 flag */
#define HAL_FLASH_FLAG_DBECCERR_BANK2           (HAL_FLASH_SR_DBECCERR | 0x80000000U)        /*!< Double Detection ECC Error on Bank 2 flag */
#define HAL_FLASH_FLAG_CRCRDERR_BANK2           (HAL_FLASH_SR_CRCRDERR | 0x80000000U)        /*!< CRC Read error on Bank 2 flag */


#define HAL_FLASH_FLAG_ALL_ERRORS_BANK2         (HAL_FLASH_FLAG_WRPERR_BANK2   | HAL_FLASH_FLAG_PGSERR_BANK2   | \
                                                HAL_FLASH_FLAG_STRBERR_BANK2  | HAL_FLASH_FLAG_INCERR_BANK2   | \
                                                HAL_FLASH_FLAG_OPERR_BANK2    | HAL_FLASH_FLAG_RDPERR_BANK2   | \
                                                HAL_FLASH_FLAG_RDSERR_BANK2   | HAL_FLASH_FLAG_SNECCERR_BANK2 | \
                                                HAL_FLASH_FLAG_DBECCERR_BANK2 | HAL_FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */

/* -------------------- INTERFACE -------------------- */

#define HAL_FLASH_BANK1_BASE                    0x08000000
#define HAL_FLASH_BANK2_BASE                    0x08100000

// Last valid address of the flash memory (i.e., this address + 1 is invalid)
#define HAL_FLASH_END                           0x081FFFFF

#define HAL_FLASH_KEY1                          0x45670123
#define HAL_FLASH_KEY2                          0xCDEF89AB


#define HAL_FLASH_OPTKEY1                       0x08192A3B
#define HAL_FLASH_OPTKEY2                       0x4C5D6E7F


#define __HAL_FLASH_GET_FLAG_BANK1(__POS__)     (BIT_READ(HAL_FLASH_SR1_REG, (__POS__)) != 0)

#define __HAL_FLASH_GET_FLAG_BANK2(__POS__)     (BIT_READ(HAL_FLASH_SR2_REG, (__POS__)) != 0)


#define __HAL_FLASH_CLEAR_FLAG_BANK1(__FLAG__)  REG_WRITE(HAL_FLASH_CCR1_REG, (__FLAG__))

#define __HAL_FLASH_CLEAR_FLAG_BANK2(__FLAG__)  REG_WRITE(HAL_FLASH_CCR2_REG, ((__FLAG__) & 0x7FFFFFFFU))


#define HAL_FLASH_BANK_1                        1
#define HAL_FLASH_BANK_2                        2

// Size, in bytes, of a flash sector (i.e., minimal erase size)
#define HAL_FLASH_SECTOR_SIZE 					0x20000

// Size in bytes of a flash word (i.e., smallest write size)
#define HAL_FLASH_WORD_SIZE                 	32

// Number of flash sectors per bank (needed because bank registers are handled separately)
#define HAL_FLASH_SECTOR_PER_BANK 				8

// Is a flash address in range?
#define IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) (((ADDRESS) >= HAL_FLASH_BANK1_BASE) && ((ADDRESS) < HAL_FLASH_BANK2_BASE))
#define IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS) (((ADDRESS) >= HAL_FLASH_BANK2_BASE ) && ((ADDRESS) <= HAL_FLASH_END))
#define IS_FLASH_PROGRAM_ADDRESS_VALID(ADDRESS) (IS_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) || IS_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS))

// State of a flash 32-bit word after reset
#define HAL_FLASH_RESET_32BIT_STATE				0xFFFFFFFF

#endif /* __DRIVERS_FLASH_H__ */