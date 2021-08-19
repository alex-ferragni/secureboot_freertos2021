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

// flash_map_backend/flash_map_backend.h
#pragma once

#include <inttypes.h>

/* Definitions used to make the link between the bootloader and MCUboot. No need to use it otherwise */

struct flash_area {
	uint8_t  fa_id;         /** The slot/scratch identification */
	uint8_t  fa_device_id;  /** The device id (usually there's only one) */
	uint16_t pad16;
	uint32_t fa_off;        /** The flash offset from the beginning */
	uint32_t fa_size;       /** The size of this sector */
};

//! Structure describing a sector within a flash area.
struct flash_sector {
	//! Offset of this sector, from the start of its flash area (not device).
	uint32_t fs_off;

	//! Size of this sector, in bytes.
	uint32_t fs_size;
};

//! Opens the area for use. id is one of the `fa_id`s */
int flash_area_open(uint8_t id, const struct flash_area **area_outp);
void flash_area_close(const struct flash_area *fa);

//! Reads `len` bytes of flash memory at `off` to the buffer at `dst`
int flash_area_read(const struct flash_area *fa, uint32_t off,
                    void *dst, uint32_t len);
//! Writes `len` bytes of flash memory at `off` from the buffer at `src`
int flash_area_write(const struct flash_area *fa, uint32_t off,
                     const void *src, uint32_t len);
//! Erases `len` bytes of flash memory at `off`
int flash_area_erase(const struct flash_area *fa,
                     uint32_t off, uint32_t len);

//! Returns this `flash_area`s alignment
size_t flash_area_align(const struct flash_area *area);
//! Returns the value read from an erased flash area byte
uint8_t flash_area_erased_val(const struct flash_area *area);

//! Given flash area ID, return info about sectors within the area
int flash_area_get_sectors(int fa_id, uint32_t *count,
                           struct flash_sector *sectors);

static inline uint32_t flash_area_get_off(const struct flash_area *fa)
{
	return fa->fa_off;
}

static inline uint32_t flash_area_get_size(const struct flash_area *fa)
{
	return fa->fa_size;
}

static inline uint8_t flash_area_get_id(const struct flash_area *fa)
{
	return fa->fa_id;
}

static inline uint8_t flash_area_get_device_id(const struct flash_area *fa){
	return fa->fa_device_id;
}

static inline uint32_t flash_sector_get_off(const struct flash_sector *fs)
{
    return fs->fs_off;
}

static inline uint32_t flash_sector_get_size(const struct flash_sector *fs)
{
    return fs->fs_size;
}

//! Returns the `fa_id` for slot, where slot is 0 (primary) or 1 (secondary).
//!
//! `image_index` (0 or 1) is the index of the image. Image index is
//!  relevant only when multi-image support support is enabled
int flash_area_id_from_multi_image_slot(int image_index, int slot);
int flash_area_id_from_image_slot(int slot);

int flash_area_id_to_multi_image_slot(int image_index, int area_id);
int flash_area_id_to_image_slot(int area_id);
