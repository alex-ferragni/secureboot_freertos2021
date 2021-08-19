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

#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "hal/hal.h"
#include "sysflash/sysflash.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "bios/bios.h"
#include "image_util/image_util.h"

static void do_boot(struct boot_rsp *rsp);

int main(int argc, char *argv[]) {
    struct boot_rsp rsp;

    // Initialize HAL
    HAL_INIT();

	MCUBOOT_LOG_INF_NO_PREFIX("\n\n================ BOOTLOADER ================");
	MCUBOOT_LOG_INF("Hardware initialized");
	MCUBOOT_LOG_INF("Compiled %s %s", __DATE__, __TIME__);

    // Call test hook
	TEST_INIT_MENU();
	
#ifdef ENABLE_BIOS
    // Call the BIOS if it is included
	MCUBOOT_LOG_INF("Press the user button during initialization to start the mini bios");

    if(GET_USER_INPUT_STATE()){
		BIOS_START();
    }
#endif /* ENABLE_BIOS */

	MCUBOOT_LOG_INF("Executing MCUBoot...");

    // Call MCUboot and measure its execution time
    uint32_t time0 = HAL_TIM_GET_TIME_MS();
    uint32_t boot_res = boot_go(&rsp);
    uint32_t time1 = HAL_TIM_GET_TIME_MS();

    // Only print the time if some time passed (and if drivers are implemented!)
    // (Empty driver will return 0, so ignore this results)
    if (time0 != time1){
        MCUBOOT_LOG_INF("Elapsed time: %lu.%03lus", HAL_TIM_ELAPSED_TIME(time0, time1)/1000, HAL_TIM_ELAPSED_TIME(time0, time1)%1000);
    }
    
    if (boot_res == 0){
        // Image in primary slot has been validated, ready to boot
        // Call test hook
		TEST_VALID_IMAGE_MENU();
		MCUBOOT_LOG_INF("Image verification successful, booting...");

        // Boot
        do_boot(&rsp);
    }
    else{
        // No valid images were found or installed, stop everything
        // Call test hook
		TEST_INVALID_IMAGE_MENU();
		MCUBOOT_LOG_ERR("Image verification failed!");

		MCUBOOT_LOG_INF_NO_PREFIX("============= END OF BOOTLOADER ============");

        //Halt
        while (1) { }
    }
    return 0;
}

static void do_boot(struct boot_rsp *rsp) {
    // We run from internal flash.
    uint32_t vector_table = rsp->br_image_off + rsp->br_hdr->ih_hdr_size;

    // Flush the stack, in case MCUboot left any sensitive information
	// Don't flush the rest of RAM, we still need some data & MCUboot has no dynamic allocation anyway
    for (uint32_t *sp = STACK_START; sp < STACK_END;) {
		*(sp++) = 0;
	}

	boot_from_image((uint32_t*)vector_table);
}