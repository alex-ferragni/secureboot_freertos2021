/**
  ******************************************************************************
  * @file    security.h
  * @author  CSEM SA
  * @brief   Header file of SECURITY module.
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

#ifndef __DRIVERS_SECURITY_H__
#define __DRIVERS_SECURITY_H__

#include "hal/hal_util.h"
#include "hal/hal_flash.h"
#include "st/rss.h"

// All states in which the board protection can be
typedef enum  {
	// Invalid state (none of the following)
	security_invalid,
	// No protections
	security_none,
	// Only secure bit is enabled
	security_secure_bit,
	// Secure bit + secure area enabled
	security_secure_area,
	// Secure bit + secure area + rdp1
	security_rdp1,
} board_security_state_t;

#define HAL_FLASH_UNLOCK_OPT_REG()			REG_WRITE(HAL_FLASH_OPTKEYR_REG, HAL_FLASH_OPTKEY1); \
											REG_WRITE(HAL_FLASH_OPTKEYR_REG, HAL_FLASH_OPTKEY2)
#define HAL_FLASH_LOCK_OPT_REG()			HAL_FLASH_OPTCR_REG |= HAL_FLASH_OPTCR_OPTLOCK | HAL_FLASH_OPTCR_OPTSTART

#define HAL_OPTREG_GET_SECURE_MODE()	((HAL_FLASH_OPTSR_REG & HAL_FLASH_OPTSR_SECURITY) >> HAL_FLASH_OPTSR_SECURITY_POS)

#define HAL_OPTREG_GET_RDP_LEVEL()		((HAL_FLASH_OPTSR_REG & HAL_FLASH_OPTSR_RDP) >> HAL_FLASH_OPTSR_RDP_POS)
#define HAL_OPTREG_GET_RDP0()			(HAL_FLASH_OPTSR_RDP0 >> HAL_FLASH_OPTSR_RDP_POS)
#define HAL_OPTREG_GET_RDP2()			(0xCC >> HAL_FLASH_OPTSR_RDP_POS)
//hardcoded so that we cannot use it directly with FLASH_OPTSR_RDP2, if we use this one it should be shifted first, so fewer accidents

#define HAL_OPTREG_GET_SEC_AREA_START()	((((HAL_FLASH_SCAR1_REG & HAL_FLASH_SCAR1_SEC_AREA_START1) >> HAL_FLASH_SCAR1_SEC_AREA_START1_POS) << 8) | 0x8000000)
#define HAL_OPTREG_GET_SEC_AREA_END()	((((HAL_FLASH_SCAR1_REG & HAL_FLASH_SCAR1_SEC_AREA_END1) >> HAL_FLASH_SCAR1_SEC_AREA_END1_POS) << 8) | 0x80000ff)
#define HAL_OPTREG_GET_DMES()			((HAL_FLASH_SCAR1_REG & HAL_FLASH_SCAR1_DMES1) >> HAL_FLASH_SCAR1_DMES1_POS)

// Function that is called whenever the bootloader jumps to an image;
// Signals to the hw that some mechanisms can be locked/unlocked
// for other applications (e.g., lock secure bootloader area and indicate
// that we exited the secure area in STM32H753)
// Should jump to address *target_address* and never return
#define EXIT_BOOTLOADER(target_address)		exitSecureArea(target_address)

/**
 * @brief Enables the secure mode
 * Fully reversible, not dangerous
 */
void optreg_set_security(void);

/**
 * @brief Disables the secure mode
 * Not dangerous
 */
void optreg_unset_security(void);

/**
 * @brief Enables the readout protection level 1
 * *** WARNING ***
 * Make sure you know you can disable RP1 before enabling it!
 * Other security features (secure areas!) might make RDP regression less straight-forward
 * In particular, either the bootloader should be able to disable rdp itself,
 * Or it must successfully leave the secure area, otherwise recovering the board might be complicated...
 * This will make access to flash from the outside impossible
 * Any connexion to a debugger will make access (even internal, so instant crash) to flash impossible until a power-down reset
 * Flash option registers can still be accessed though, reverting RDP1 to RDP0 will wipe the flash
 */
void optreg_enable_rdp1(void);

/**
 * @brief Performs an RDP regression; disables the secure area
*/
void optreg_disable_secure_area_rdp1(void);

/**
 * @brief Returns the current state of the board protection
 * 
 * @return the current state of the board protection
*/
board_security_state_t security_get_board_security(void);

#endif /* __DRIVERS_SECURITY_H__ */