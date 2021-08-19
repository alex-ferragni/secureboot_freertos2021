/**
  ******************************************************************************
  * @file    flash.c
  * @author  CSEM SA
  * @brief   Source file of FLASH module.
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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hal/hal_flash.h"
#include "hal/hal_scb.h"
#include "sysflash/sysflash.h"

/* Private        ------------------------------------------------------------*/

/**
    * @brief  FLASH handle Structure definition
    */
typedef struct
{
    _IO uint32_t               ErrorCode;          /*!< FLASH error code                                                              */
}FLASH_process_t;

FLASH_process_t flash_process = {0};

#define HAL_FLASH_ERROR_NONE   0

#define HAL_FLASH_FLAG_QW_BANK1    HAL_FLASH_SR_QW
#define HAL_FLASH_FLAG_QW_BANK2    (HAL_FLASH_SR_QW | 0x80000000)

#define FLASH_VOLTAGE_RANGE_3  (2 << HAL_FLASH_CR_PSIZE_POS)

#ifndef ENABLE_FLASH_CORRUPTION
uint32_t HAL_FLASH_Program(uint32_t* flash_address, uint32_t const* data_address);
#endif /* ENABLE_FLASH_CORRUPTION */

uint32_t HAL_FLASH_erase(uint32_t bank_num, uint32_t first_sector, uint32_t num_sectors, uint32_t* SectorError);
uint32_t HAL_FLASH_Lock(void);
uint32_t HAL_FLASH_Unlock(void);

uint32_t FLASH_wait_for_last_operation(uint32_t Bank)
{
    /* Wait for the FLASH operation to complete by polling on QW flag to be reset.
        Even if the FLASH operation fails, the QW flag will be reset and an error
        flag will be set */

    uint32_t errorflag;
    //uint32_t tickstart = HAL_GetTick();

    //assert_param(IS_FLASH_BANK_EXCLUSIVE(Bank));

    /* Select bsyflag depending on Bank */
    if(Bank == HAL_FLASH_BANK_1)
    {
        while(__HAL_FLASH_GET_FLAG_BANK1(HAL_FLASH_SR_QW_POS)){}
        while(__HAL_FLASH_GET_FLAG_BANK1(HAL_FLASH_SR_BSY_POS)){}
    }
    else
    {
        while(__HAL_FLASH_GET_FLAG_BANK2(HAL_FLASH_SR_QW_POS)){}
        while(__HAL_FLASH_GET_FLAG_BANK2(HAL_FLASH_SR_BSY_POS)){}
    }


    /* Get Error Flags */
    if (Bank == HAL_FLASH_BANK_1)
    {
        errorflag = HAL_FLASH_SR1_REG & HAL_FLASH_FLAG_ALL_ERRORS_BANK1;
    }
    else
    {
        errorflag = (HAL_FLASH_SR2_REG & HAL_FLASH_FLAG_ALL_ERRORS_BANK2) | 0x80000000U;
    }

    /* In case of error reported in Flash SR1 or SR2 registers (ECCC not managed as an error) */
    if((errorflag & 0x7DFFFFFFU) != 0U)
    {
        /*Save the error code*/
        flash_process.ErrorCode |= errorflag;

        /* Clear error programming flags */
        if ((errorflag & 0x80000000U) == 0){
            //BANK 1
            __HAL_FLASH_CLEAR_FLAG_BANK1(errorflag);
        }
        else{
            //BANK 2
            __HAL_FLASH_CLEAR_FLAG_BANK2(errorflag);
        }
        
        return RET_ERROR;
    }

    /* Check FLASH End of Operation flag  */
    if(Bank == HAL_FLASH_BANK_1)
    {
        if (__HAL_FLASH_GET_FLAG_BANK1(HAL_FLASH_SR_EOP_POS))
        {
            /* Clear FLASH End of Operation pending bit */
            __HAL_FLASH_CLEAR_FLAG_BANK1(HAL_FLASH_SR_EOP);
        }
    }
    else
    {
        if (__HAL_FLASH_GET_FLAG_BANK2(HAL_FLASH_SR_EOP_POS))
        {
            /* Clear FLASH End of Operation pending bit */
            __HAL_FLASH_CLEAR_FLAG_BANK2(HAL_FLASH_SR_EOP);
        }
    }

    return RET_OK;
}

void FLASH_erase_sector(uint32_t Sector, uint32_t Bank)
{
    if(Bank == HAL_FLASH_BANK_1)
    {
        /* Reset Program/erase VoltageRange and Sector Number for Bank1 */
        HAL_FLASH_CR1_REG &= ~(HAL_FLASH_CR_PSIZE | HAL_FLASH_CR_SNB);

        HAL_FLASH_CR1_REG |= (HAL_FLASH_CR_SER | FLASH_VOLTAGE_RANGE_3 | (Sector << HAL_FLASH_CR_SNB_POS) | HAL_FLASH_CR_START);
    }

    if(Bank == HAL_FLASH_BANK_2)
    {
        /* Reset Program/erase VoltageRange and Sector Number for Bank2 */
        HAL_FLASH_CR2_REG &= ~(HAL_FLASH_CR_PSIZE | HAL_FLASH_CR_SNB);

        HAL_FLASH_CR2_REG |= (HAL_FLASH_CR_SER | FLASH_VOLTAGE_RANGE_3  | (Sector << HAL_FLASH_CR_SNB_POS) | HAL_FLASH_CR_START);
    }
}

/* Public         ------------------------------------------------------------*/

uint32_t HAL_FLASH_Unlock(void) {
    if(BIT_READ(HAL_FLASH_CR1_REG, HAL_FLASH_CR_LOCK_POS) != 0U)
    {
        /* Authorize the FLASH Bank1 Registers access */
        REG_WRITE(HAL_FLASH_KEYR1_REG, HAL_FLASH_KEY1);
        REG_WRITE(HAL_FLASH_KEYR1_REG, HAL_FLASH_KEY2);

        /* Verify Flash Bank1 is unlocked */
        if (BIT_READ(HAL_FLASH_CR1_REG, HAL_FLASH_CR_LOCK_POS) != 0U)
        {
            return RET_ERROR;
        }
    }

    if(BIT_READ(HAL_FLASH_CR2_REG, HAL_FLASH_CR_LOCK_POS) != 0U)
    {
        /* Authorize the FLASH Bank2 Registers access */
        REG_WRITE(HAL_FLASH_KEYR2_REG, HAL_FLASH_KEY1);
        REG_WRITE(HAL_FLASH_KEYR2_REG, HAL_FLASH_KEY2);

        /* Verify Flash Bank2 is unlocked */
        if (BIT_READ(HAL_FLASH_CR2_REG, HAL_FLASH_CR_LOCK_POS) != 0U)
        {
            return RET_ERROR;
        }
    }

    return RET_OK;
}

uint32_t HAL_FLASH_Lock(void) {
      /* Set the LOCK Bit to lock the FLASH Bank1 Control Register access */
    BIT_SET(HAL_FLASH_CR1_REG, HAL_FLASH_CR_LOCK_POS);

    /* Verify Flash Bank1 is locked */
    if (BIT_READ(HAL_FLASH_CR1_REG, HAL_FLASH_CR_LOCK_POS) == 0U)
    {
        return RET_ERROR;
    }

    /* Set the LOCK Bit to lock the FLASH Bank2 Control Register access */
    BIT_SET(HAL_FLASH_CR2_REG, HAL_FLASH_CR_LOCK_POS);

    /* Verify Flash Bank2 is locked */
    if (BIT_READ(HAL_FLASH_CR2_REG, HAL_FLASH_CR_LOCK_POS) == 0U)
    {
        return RET_ERROR;
    }

    return RET_OK;
}

#ifdef ENABLE_FLASH_CORRUPTION
uint32_t HAL_FLASH_Program(uint32_t* flash_address, uint32_t const* data_address, uint32_t write_size){
#else /* ENABLE_FLASH_CORRUPTION */
uint32_t HAL_FLASH_Program(uint32_t* flash_address, uint32_t const* data_address){
#endif /* ENABLE_FLASH_CORRUPTION */
    uint32_t status;
    _IO uint32_t *dest_addr = (_IO uint32_t *)flash_address;
    _IO uint32_t const *src_addr = (_IO uint32_t const*)data_address;
    uint32_t bank;

    uint32_t FlashAddress = (uint32_t)flash_address;

#ifdef ENABLE_FLASH_CORRUPTION
	uint8_t row_index = write_size/4;
#else /* ENABLE_FLASH_CORRUPTION */
    uint8_t row_index = HAL_FLASH_NB_32BITWORD_IN_FLASHWORD;
#endif /* ENABLE_FLASH_CORRUPTION */

    /* Check the parameters */
    //assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));
    //assert_param(IS_FLASH_PROGRAM_ADDRESS(FlashAddress))

    if(IS_FLASH_PROGRAM_ADDRESS_BANK1(FlashAddress))
    {
        bank = HAL_FLASH_BANK_1;
    }
    else
    {
        bank = HAL_FLASH_BANK_2;
    }

    /* Reset error code */
    flash_process.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Wait for last operation to be completed */
    status = FLASH_wait_for_last_operation(bank);

    if(status == RET_OK)
    {
        if(bank == HAL_FLASH_BANK_1)
        {
			/* Set PG bit */
			BIT_SET(HAL_FLASH_CR1_REG, HAL_FLASH_CR_PG_POS);
        }
        else
        {
            /* Set PG bit */
            BIT_SET(HAL_FLASH_CR2_REG, HAL_FLASH_CR_PG_POS);
        }

        ISB();
        DSB();

        {
            /* Program the flash word */
            do
            {
                *dest_addr = *src_addr;
                dest_addr++;
                src_addr++;
                row_index--;
            } while (row_index != 0U);
        }

        ISB();
        DSB();

#ifdef ENABLE_FLASH_CORRUPTION
		if(write_size/4 != HAL_FLASH_NB_32BITWORD_IN_FLASHWORD){
			if(bank == HAL_FLASH_BANK_1)
            {
                BIT_SET(HAL_FLASH_CR1_REG, 6); //Force-write, likely to cause ECC errors if we write again
            }
            else
            {
                BIT_SET(HAL_FLASH_CR2_REG, 6); //Force-write, likely to cause ECC errors if we write again
            }
		}
#endif /* ENABLE_FLASH_CORRUPTION */

        /* Wait for last operation to be completed */
        status = FLASH_wait_for_last_operation(bank);

        {
            if(bank == HAL_FLASH_BANK_1)
            {
                /* If the program operation is completed, disable the PG */
                BIT_CLEAR(HAL_FLASH_CR1_REG, HAL_FLASH_CR_PG_POS);
            }
            else
            {
                /* If the program operation is completed, disable the PG */
                BIT_CLEAR(HAL_FLASH_CR2_REG, HAL_FLASH_CR_PG_POS);
            }
        }
    }

    return status;
}

uint32_t HAL_FLASH_erase(uint32_t bank_num, uint32_t first_sector, uint32_t num_sectors, uint32_t* SectorError){
    uint32_t status = RET_OK;
    uint32_t sector_index;

    /* Check the parameters */
    //assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));
    //assert_param(IS_FLASH_BANK(pEraseInit->Banks));

    /* Reset error code */
    flash_process.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Wait for last operation to be completed on Bank1 */
    if(bank_num == HAL_FLASH_BANK_1)
    {
        if(FLASH_wait_for_last_operation(HAL_FLASH_BANK_1) != RET_OK)
        {
            status = RET_ERROR;
        }
    }

    /* Wait for last operation to be completed on Bank2 */
    if(bank_num == HAL_FLASH_BANK_2)
    {
        if(FLASH_wait_for_last_operation(HAL_FLASH_BANK_2) != RET_OK)
        {
            status = RET_ERROR;
        }
    }

    if(status == RET_OK)
    {
        /*Initialization of SectorError variable*/
        *SectorError = 0xFFFFFFFFU;

        /* Erase by sector by sector to be done*/
        for(sector_index = first_sector; sector_index < (num_sectors + first_sector); sector_index++)
        {
            FLASH_erase_sector(sector_index, bank_num);

            if(bank_num == HAL_FLASH_BANK_1)
            {
                /* Wait for last operation to be completed */
                status = FLASH_wait_for_last_operation(HAL_FLASH_BANK_1);

                /* If the erase operation is completed, disable the SER Bit */
                HAL_FLASH_CR1_REG &= (~(HAL_FLASH_CR_SER | HAL_FLASH_CR_SNB));
            }
            if(bank_num == HAL_FLASH_BANK_2)
            {
                /* Wait for last operation to be completed */
                status = FLASH_wait_for_last_operation(HAL_FLASH_BANK_2);

                /* If the erase operation is completed, disable the SER Bit */
                HAL_FLASH_CR2_REG &= (~(HAL_FLASH_CR_SER | HAL_FLASH_CR_SNB));
            }

            if(status != RET_OK)
            {
                /* In case of error, stop erase procedure and return the faulty sector */
                *SectorError = sector_index;
                break;
            }
        }
    }

    return status;
}

uint32_t get_flash_sector_by_address(void* addr){
    //assert(IS_FLASH_PROGRAM_ADDRESS_VALID((uint32_t)addr));
    return (((uint32_t)addr - HAL_FLASH_BANK1_BASE) / HAL_FLASH_SECTOR_SIZE) % HAL_FLASH_SECTOR_PER_BANK;
}

uint32_t get_flash_bank_by_address(void* addr){
    //assert(IS_FLASH_PROGRAM_ADDRESS_VALID((uint32_t)addr));
    if (IS_FLASH_PROGRAM_ADDRESS_BANK1((uint32_t)addr)){
        return HAL_FLASH_BANK_1;
    }
    else if (IS_FLASH_PROGRAM_ADDRESS_BANK2((uint32_t)addr)){
        return HAL_FLASH_BANK_2;
    }
    else{
        //never reached
        return -1;
    }
}

uint32_t minimal_flash_write(uint32_t addr, const void *buf, size_t length) {
	uint32_t status = RET_OK;

    HAL_FLASH_Unlock();
	
	//wait for any unexpected operation and reset previous error flags
	FLASH_wait_for_last_operation(HAL_FLASH_BANK_1);
	FLASH_wait_for_last_operation(HAL_FLASH_BANK_2);

	for(size_t i = 0; i*HAL_FLASH_WORD_SIZE < length; ++i) {

	#ifdef ENABLE_FLASH_CORRUPTION
		if (((i+1)*HAL_FLASH_WORD_SIZE > length) && (length % HAL_FLASH_WORD_SIZE != 0)){
			status = HAL_FLASH_Program((uint32_t* )(addr), ((uint32_t const*)buf) + (i*HAL_FLASH_NB_32BITWORD_IN_FLASHWORD), length%HAL_FLASH_WORD_SIZE);
		}
		else{
			status = HAL_FLASH_Program((uint32_t* )(addr), ((uint32_t const*)buf) + (i*HAL_FLASH_NB_32BITWORD_IN_FLASHWORD), HAL_FLASH_WORD_SIZE);
		}
	#else /* ENABLE_FLASH_CORRUPTION */
		//note: if length % FLASH_WORD_SIZE != 0, last word will still be a full word
		status = HAL_FLASH_Program((uint32_t* )(addr), ((uint32_t const*)buf) + (i*HAL_FLASH_NB_32BITWORD_IN_FLASHWORD));
	#endif /* ENABLE_FLASH_CORRUPTION */

        if (status != RET_OK){
			HAL_FLASH_Lock();
			return status;
		}
		addr += HAL_FLASH_WORD_SIZE;
	}
	
	HAL_FLASH_Lock();

	return status;
}

uint32_t minimal_flash_erase_sector(uint32_t addr) {
	uint32_t status = RET_OK;
	
    HAL_FLASH_Unlock();

	uint32_t PAGEError = 0;

	//wait for any unexpected operation and reset previous error flags
	FLASH_wait_for_last_operation(HAL_FLASH_BANK_1);
	FLASH_wait_for_last_operation(HAL_FLASH_BANK_2);
	status = HAL_FLASH_erase(get_flash_bank_by_address((void*) addr), 
                    get_flash_sector_by_address((void*)addr), 
                    1, 
                    &PAGEError); 

    HAL_FLASH_Lock();

	return status;
}

uint32_t minimal_flash_read(uint32_t addr, void* buf, size_t length){
	uint32_t status = RET_OK;

#ifdef SAFE_ECC_DBERRORS
	//We should make sure the access is absolutely safe when we mask bus fault 
	//interrupts. I could be used to bypass the mpu for example
	//but the bootloader doesn't need one anyway so that's fine

	if (addr < HAL_FLASH_BANK1_BASE || addr + length - 1 > HAL_FLASH_END){
		return RET_ERROR;
	}

	//make sure we don't include any previous ecc error that was forgotten
	BIT_SET(HAL_FLASH_CCR1_REG, HAL_FLASH_SR_DBECCERR_POS);
	BIT_SET(HAL_FLASH_CCR2_REG, HAL_FLASH_SR_DBECCERR_POS);

	//ignore all exceptions except non-maskable ones
	SCB_SET_FAULTMASK(1);

	//don't trigger hardfaults-level fault (the one that is caused by a double ecc error)
	BIT_SET(HAL_SCB_CCR_REG, HAL_SCB_CCR_BFHFNMIGN_POS);
	DSB();
	ISB();

#endif /* SAFE_ECC_DBERRORS */

	memcpy(buf, (uint8_t *) addr, length);

#ifdef SAFE_ECC_DBERRORS

	//restore exceptions
	SCB_SET_FAULTMASK(0);

	//restore hardfault-level exceptions
	BIT_CLEAR(HAL_SCB_CCR_REG, HAL_SCB_CCR_BFHFNMIGN_POS);
	DSB();
	ISB();


	//read status register and look for a double ECC error
	if ((BIT_READ(HAL_FLASH_SR1_REG, HAL_FLASH_SR_DBECCERR_POS) | BIT_READ(HAL_FLASH_SR2_REG, HAL_FLASH_SR_DBECCERR_POS)) != 0){
		//we avoided a double ECC error that would have caused a crash

		//clear error bits
		BIT_SET(HAL_FLASH_CCR1_REG, HAL_FLASH_SR_DBECCERR_POS);
		BIT_SET(HAL_FLASH_CCR2_REG, HAL_FLASH_SR_DBECCERR_POS);

		//don't signal the error (mcuboot cannot recover otherwise)
        //but still act as if flash were erased, fill with 1s

        memset(buf, -1, length);
	}

#endif /* SAFE_ECC_DBERRORS  */

	return status;
}