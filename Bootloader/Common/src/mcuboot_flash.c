/*
 *    Code Released under MIT License
 *
 *    STM32 port
 *    Copyright (c) 2021 CSEM SA
 *
 *    Interrupt fork
 *    Copyright (c) 2019 Memfault, Inc.
 * 
 *    Original Jekyll Template
 *    Copyright (c) 2015 Jacopo Rabolini
 * 
 *    Permission is hereby granted, free of charge, to any person obtaining a 
 *    copy of this software and associated documentation files (the "Software"), 
 *    to deal in the Software without restriction, including without limitation 
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *    and/or sell copies of the Software, and to permit persons to whom the 
 *    Software is furnished to do so, subject to the following conditions:
 * 
 *    The above copyright notice and this permission notice shall be included 
 *    in all copies or substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 *    THE SOFTWARE.
 */

// mcuboot_port.c

#include <string.h>
#include <stdlib.h>

#include "flash_map_backend/flash_map_backend.h"
#include "sysflash/sysflash.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "mcuboot_config/mcuboot_assert.h"

#include "hal/hal_flash.h"

extern	uint32_t	_stFlash;
extern	uint32_t	_lnBootloader;
extern	uint32_t	_stImgHeader;
extern	uint32_t	_lnImg;
extern	uint32_t	_stImg2Header;
extern	uint32_t	_stScratch;
extern	uint32_t	_lnScratch;

// Interface that MCUboot uses to interract with the flash drivers; the link is made here

// Bootloader area
static const struct flash_area bootloader = {
    .fa_id = FLASH_AREA_BOOTLOADER,
    .fa_device_id = FLASH_DEVICE_BOOTLOADER,
    .fa_off = (uint32_t)&_stFlash,
    .fa_size = (uint32_t)&_lnBootloader,
};

// Primary slot
static const struct flash_area primary_img0 = {
    .fa_id = FLASH_AREA_IMAGE_PRIMARY(0),
    .fa_device_id = FLASH_DEVICE_PRIMARY,
    .fa_off = (uint32_t)&_stImgHeader,
    .fa_size = (uint32_t)&_lnImg,
};

// Secondary slot
static const struct flash_area secondary_img0 = {
    .fa_id = FLASH_AREA_IMAGE_SECONDARY(0),
    .fa_device_id = FLASH_DEVICE_SECONDARY,
    .fa_off = (uint32_t)&_stImg2Header,
    .fa_size = (uint32_t)&_lnImg,
};

// Scratch area
static const struct flash_area scratch = {
    .fa_id = FLASH_AREA_IMAGE_SCRATCH,
    .fa_device_id = FLASH_DEVICE_SCRATCH,
    .fa_off = (uint32_t)&_stScratch,
    .fa_size = (uint32_t)&_lnScratch,
};

static const struct flash_area *s_flash_areas[] = {
    &bootloader,
    &primary_img0,
    &secondary_img0,
    &scratch,
};

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])

// our flash areas are static, this is a simple lookup by id
static const struct flash_area *prv_lookup_flash_area(uint8_t id) {
    for (size_t i = 0; i < ARRAY_SIZE(s_flash_areas); i++) {
        const struct flash_area *area = s_flash_areas[i];
        if (id == area->fa_id) {
            return area;
        }
    }
    return NULL;
}

int flash_area_open(uint8_t id, const struct flash_area **area_outp) {
    MCUBOOT_LOG_DBG("%s: ID=%d", __func__, (int)id);

    const struct flash_area *area = prv_lookup_flash_area(id);
    *area_outp = area;
    return area != NULL ? 0 : -1;
}

void flash_area_close(const struct flash_area *area) {
    // no cleanup to do for this flash part
}

#undef VALIDATE_PROGRAM_OP

int flash_area_read(const struct flash_area *fa, uint32_t off, void *dst,
                    uint32_t len) {
    if (!IS_DEVICE_ID_VALID(fa->fa_device_id)) {
        return -1;
    }

    const uint32_t end_offset = off + len;
    if (end_offset > fa->fa_size) {
        MCUBOOT_LOG_ERR("%s: Out of Bounds (0x%lu vs 0x%lu)", __func__, end_offset, fa->fa_size);
        return -1;
    }

	uint32_t status = minimal_flash_read(fa->fa_off + off, dst, len);
	if (status != RET_OK){
		MCUBOOT_LOG_ERR("Failed to read flash at 0x%08lX, len = %lu", fa->fa_off + off, len);
		//__asm__("bkpt");
	}
	return status;
}

int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src,
                     uint32_t len) {
    if (!IS_DEVICE_ID_VALID(fa->fa_device_id)) {
        return -1;
    }

    const uint32_t end_offset = off + len;
    if (end_offset > fa->fa_size) {
        MCUBOOT_LOG_ERR("%s: Out of Bounds (0x%lu vs 0x%lu)", __func__, end_offset, fa->fa_size);
        return -1;
    }

    const uint32_t addr = fa->fa_off + off;
    MCUBOOT_LOG_DBG("%s: Addr: 0x%08x Length: %d", __func__, (int)addr, (int)len);

	uint32_t status = 0;
	minimal_flash_write(addr, src, len);

#ifdef VALIDATE_PROGRAM_OP
    if (memcmp((void *)addr, src, len) != 0) {
        MCUBOOT_LOG_ERR("%s: Program Failed", __func__);
        assert(0);
    }
#endif

    return status;
}

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len) {
    if (!IS_DEVICE_ID_VALID(fa->fa_device_id)) {
        return -1;
    }

    if ((len % HAL_FLASH_SECTOR_SIZE) != 0 || (off % HAL_FLASH_SECTOR_SIZE) != 0) {
        MCUBOOT_LOG_ERR("%s: Not aligned on sector Offset: 0x%x Length: 0x%x", __func__,
                        (int)off, (int)len);
        return -1;
    }

    const uint32_t start_addr = fa->fa_off + off;
    MCUBOOT_LOG_DBG("%s: Addr: 0x%08x Length: %d", __func__, (int)start_addr, (int)len);

    for (size_t i = 0; i < len; i+= HAL_FLASH_SECTOR_SIZE) {
        const uint32_t addr = start_addr + i;
        minimal_flash_erase_sector(addr);
    }

#ifdef VALIDATE_PROGRAM_OP
    for (size_t i = 0; i < len; i++) {
        uint8_t *val = (void *)(start_addr + i);
        if (*val != 0xff) {
            MCUBOOT_LOG_ERR("%s: Erase at 0x%x Failed", __func__, (int)val);
            assert(0);
        }
    }
#endif

    return 0;
}

size_t flash_area_align(const struct flash_area *area) {
    // the smallest unit a flash write can occur along.
    // Note: Image trailers will be scaled by this size
    return HAL_FLASH_WORD_SIZE;
}

uint8_t flash_area_erased_val(const struct flash_area *area) {
    return 0xff & HAL_FLASH_RESET_32BIT_STATE;
}

int flash_area_get_sectors(int fa_id, uint32_t *count,
                           struct flash_sector *sectors) {
    const struct flash_area *fa = prv_lookup_flash_area(fa_id);
    if (!IS_DEVICE_ID_VALID(fa->fa_device_id)) {
        return -1;
    }

    // All sectors for the STM32H753 are the same size
    const size_t sector_size = HAL_FLASH_SECTOR_SIZE;
    uint32_t total_count = 0;
    for (size_t off = 0; off < fa->fa_size; off += sector_size) {
        // Note: Offset here is relative to flash area, not device
        sectors[total_count].fs_off = off;
        sectors[total_count].fs_size = sector_size;
        total_count++;
    }

    *count = total_count;
    return 0;
}

int flash_area_id_from_multi_image_slot(int image_index, int slot) {
    switch (slot) {
        case 0:
            return FLASH_AREA_IMAGE_PRIMARY(image_index);
        case 1:
            return FLASH_AREA_IMAGE_SECONDARY(image_index);
		default:
			break;
    }

    MCUBOOT_LOG_ERR("Unexpected Request: image_index=%d, slot=%d", image_index, slot);
    return -1; /* flash_area_open will fail on that */
}

int flash_area_id_from_image_slot(int slot) {
  return flash_area_id_from_multi_image_slot(0, slot);
}

int flash_area_id_to_multi_image_slot(int image_index, int area_id)
{
    if (area_id == FLASH_AREA_IMAGE_PRIMARY(image_index)) {
        return 0;
    }
	else if (area_id == FLASH_AREA_IMAGE_SECONDARY(image_index)){
		return 1;
	}

    MCUBOOT_LOG_ERR("invalid flash area ID");
    return -1;
}

int flash_area_id_to_image_slot(int area_id)
{
    return flash_area_id_to_multi_image_slot(0, area_id);
}

// Hang in case of failed assertion, make sure the loop is not optimized away
#pragma GCC push_options
#pragma GCC optimize ("O0") 
void example_assert_handler(const char *file, int line) {
	MCUBOOT_LOG_ERR("Failed assertion in file %s at line %u",  file, line);
    do {
        //hang to indicate an exception
    } while (1);
}
#pragma GCC pop_options