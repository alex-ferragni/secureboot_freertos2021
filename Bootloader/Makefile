# 
#    Code Released under MIT License
# 
#    STM32 port
#    Copyright (c) 2021 CSEM SA
# 
#    Interrupt fork
#    Copyright (c) 2019 Memfault, Inc.
# 
#    Original Jekyll Template
#    Copyright (c) 2015 Jacopo Rabolini
# 
#    Permission is hereby granted, free of charge, to any person obtaining a 
#    copy of this software and associated documentation files (the "Software"), 
#    to deal in the Software without restriction, including without limitation 
#    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
#    and/or sell copies of the Software, and to permit persons to whom the 
#    Software is furnished to do so, subject to the following conditions:
# 
#    The above copyright notice and this permission notice shall be included 
#    in all copies or substantial portions of the Software.
# 
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
#    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
#    THE SOFTWARE.
# 

# Path of the project-specific config (should be shared with images to be compatible)
ifndef PROJECT_CONFIG
$(error PROJECT_CONFIG is not set)
endif

include $(PROJECT_CONFIG)

# Path of the bootloader-specific config
ifndef BOOTLOADER_CONFIG
$(error BOOTLOADER_CONFIG is not set)
endif

include $(BOOTLOADER_CONFIG)

# Path of the MCUboot repo/fork folder
ifndef PATH_MCUBOOT
$(error PATH_MCUBOOT is not set)
endif

# Path of the folder of the porting (HW-dependent files)
ifndef PATH_PORT
$(error PATH_PORT is not set)
endif

# Path of the folder containing sections_config.ld (configures MCUboot's area sizes)
ifndef SECTIONS_CONFIG_DIR
$(error SECTIONS_CONFIG_DIR is not set)
endif

# Path of the folder containing keys (signature + encryption) (as specified during key generation)
ifndef KEYS_PATH
$(error KEYS_PATH is not set)
endif

ROOT_DIR := $(abspath ..)

# Directory of files common to bootloader and images (i.e. drivers, linker scripts)
COMMON_DIR = $(ROOT_DIR)/Bootloader/Common


###### Bootloader files ######
BOOTLOADER_DIR = $(ROOT_DIR)/Bootloader
BOOTLOADER_SRC_DIR = $(BOOTLOADER_DIR)/src
BOOTLOADER_INC_DIR = $(BOOTLOADER_DIR)/Includes

SRC_FILES += \
	$(BOOTLOADER_SRC_DIR)/main.c \
	$(COMMON_DIR)/src/startup_template.c \
	$(BOOTLOADER_SRC_DIR)/keys.c \
	$(PATH_PORT)/src/drivers/security.c \
	$(PATH_PORT)/src/drivers/cryp.c \
	$(PATH_PORT)/src/drivers/hash.c \
	$(PATH_PORT)/src/drivers/uart.c \
	$(PATH_PORT)/src/drivers/flash.c \
	$(COMMON_DIR)/src/bios.c \
	$(PATH_PORT)/src/bios_port/bios.c \
	$(COMMON_DIR)/src/image_util/image_util.c \
	$(COMMON_DIR)/src/image_util/image_download.c \
	$(COMMON_DIR)/src/image_util/image_upload.c

INCLUDE_PATHS += \
	-I$(BOOTLOADER_INC_DIR) \
	-I$(KEYS_PATH)/Includes

###### MCUboot files ######
MCUBOOT_DIR := $(PATH_MCUBOOT)/boot/bootutil
MCUBOOT_SRC_DIR := $(MCUBOOT_DIR)/src
MCUBOOT_INC_DIR := $(MCUBOOT_DIR)/include

MCUBOOT_SRC_FILES += \
	$(MCUBOOT_SRC_DIR)/boot_record.c \
	$(MCUBOOT_SRC_DIR)/bootutil_misc.c \
	$(MCUBOOT_SRC_DIR)/caps.c \
	$(MCUBOOT_SRC_DIR)/encrypted.c \
	$(MCUBOOT_SRC_DIR)/fault_injection_hardening.c \
	$(MCUBOOT_SRC_DIR)/fault_injection_hardening_delay_rng_mbedtls.c \
	$(MCUBOOT_SRC_DIR)/image_ec.c \
	$(MCUBOOT_SRC_DIR)/image_ec256.c \
	$(MCUBOOT_SRC_DIR)/image_ed25519.c \
	$(MCUBOOT_SRC_DIR)/image_rsa.c \
	$(MCUBOOT_SRC_DIR)/image_validate.c \
	$(MCUBOOT_SRC_DIR)/loader.c \
	$(MCUBOOT_SRC_DIR)/swap_misc.c \
	$(MCUBOOT_SRC_DIR)/swap_move.c \
	$(MCUBOOT_SRC_DIR)/swap_scratch.c \
	$(MCUBOOT_SRC_DIR)/tlv.c

MCUBOOT_INC_PATHS := -I$(MCUBOOT_INC_DIR) 

SRC_FILES += $(MCUBOOT_SRC_FILES)
INCLUDE_PATHS += $(MCUBOOT_INC_PATHS)


###### Tinycrypt files ######
# (used mainly for signature verification and a bit of decryption)
TINYCRYPT_DIR := $(PATH_MCUBOOT)/ext/tinycrypt/lib
TINYCRYPT_SRC_DIR := $(TINYCRYPT_DIR)/source
TINYCRYPT_INC_DIR := $(TINYCRYPT_DIR)/include

TINYCRYPT_SRC_FILES += \
	$(TINYCRYPT_SRC_DIR)/aes_decrypt.c \
	$(TINYCRYPT_SRC_DIR)/aes_encrypt.c \
	$(TINYCRYPT_SRC_DIR)/cbc_mode.c \
	$(TINYCRYPT_SRC_DIR)/ccm_mode.c \
	$(TINYCRYPT_SRC_DIR)/cmac_mode.c \
	$(TINYCRYPT_SRC_DIR)/ctr_mode.c \
	$(TINYCRYPT_SRC_DIR)/ctr_prng.c \
	$(TINYCRYPT_SRC_DIR)/ecc.c \
	$(TINYCRYPT_SRC_DIR)/ecc_dsa.c \
	$(TINYCRYPT_SRC_DIR)/ecc_dh.c \
	$(TINYCRYPT_SRC_DIR)/ecc_platform_specific.c \
	$(TINYCRYPT_SRC_DIR)/hmac.c \
	$(TINYCRYPT_SRC_DIR)/hmac_prng.c \
	$(TINYCRYPT_SRC_DIR)/sha256.c \
	$(TINYCRYPT_SRC_DIR)/utils.c

TINYCRYPT_INC_PATHS := -I$(TINYCRYPT_INC_DIR)

SRC_FILES += $(TINYCRYPT_SRC_FILES)
INCLUDE_PATHS += $(TINYCRYPT_INC_PATHS)


###### EmbedTLS files ######
# (used only for decryption with ECIES P-256)
EMBEDTLS_DIR := $(PATH_MCUBOOT)/ext/mbedtls-asn1
EMBEDTLS_CRYPTO_DIR := $(PATH_MCUBOOT)/ext/mbedtls/crypto
EMBEDTLS_SRC_DIR := $(EMBEDTLS_DIR)/src
EMBEDTLS_INC_DIR := $(EMBEDTLS_DIR)/include

EMBEDTLS_SRC_FILES += \
	$(EMBEDTLS_SRC_DIR)/asn1parse.c \
	$(EMBEDTLS_SRC_DIR)/platform_util.c \
	$(EMBEDTLS_CRYPTO_DIR)/library/bignum.c

EMBEDTLS_INC_PATHS := -I$(EMBEDTLS_INC_DIR) -I$(EMBEDTLS_CRYPTO_DIR)/include

SRC_FILES += $(EMBEDTLS_SRC_FILES)
INCLUDE_PATHS += $(EMBEDTLS_INC_PATHS)

# A few variables necessary for the sub-makefile

TARGET := mcuboot
LDSCRIPT = $(BOOTLOADER_DIR)/link.ld

BUILD_DIR := $(BOOTLOADER_DIR)/build

# By default, compile elf file
all: elf

# MBedTLS requires a few defines
# Since MBedTLS is only used in the bootloader, define them here
# It needs the location of a config file, the default one is fine
# It also needs two debug macros, we leave them empty
CFLAGS += \
	-DMBEDTLS_CONFIG_FILE="<mbedtls/config.h>" \
	-D'MBEDTLS_INTERNAL_VALIDATE_RET(cond,ret)=do{}while(0)' \
	-D'MBEDTLS_INTERNAL_VALIDATE(cond)=do{}while(0)'

# The bootloader has no other drivers, always use the provided ones
CFLAGS += \
	-DHAL_INCLUDE_ALL

include $(COMMON_DIR)/core.mk

# Refresh everything if the (context-dependent) Makefile changes
$(OBJ_FILES): Makefile

PROGRAM_CONFIG := $(PATH_PORT)/stlink.cfg


# Handy target to directly flash the bootloader
flash: $(TARGET_ELF) $(PROGRAM_CONFIG)
	$(OPENOCD) -f $(PROGRAM_CONFIG) \
	-c "program $(TARGET_ELF) verify reset exit"

# A few variables useful for the test targets
TEST_DIR := $(BOOTLOADER_DIR)/tests
TEST_BUILD_DIR := $(TEST_DIR)/build
TEST_TARGET_ELF := $(TEST_BUILD_DIR)/test_helper.elf
HELLO_WORLD_DIR := ../hello_world
HELLO_WORLD_BUILD_DIR := $(HELLO_WORLD_DIR)/build

# Compile all files (images + helper) we'd like to have for tests
prepare_tests: IMG_OFFSET = $(shell objdump $(TARGET_ELF) -t | grep "_lnImgHeader$$" | grep -o "^........" | head -1)
prepare_tests: $(TARGET_ELF)
	@make -C $(TEST_DIR)
	@make -C $(HELLO_WORLD_DIR) TARGET=image_valid
	@make -C $(HELLO_WORLD_DIR) TARGET=image_wrong_sign KEY_SIGN=$(TEST_DIR)/wrong_sign_priv.pem
	@make -C $(HELLO_WORLD_DIR) TARGET=image_wrong_enc KEY_ENC=$(TEST_DIR)/wrong_enc_pub.pem
	cp $(HELLO_WORLD_BUILD_DIR)/image_valid.bin $(HELLO_WORLD_BUILD_DIR)/image_corrupted.bin
	printf '\xff' | dd of=$(HELLO_WORLD_BUILD_DIR)/image_corrupted.bin bs=1 seek=0x$(IMG_OFFSET) count=1 conv=notrunc

# Clean board, install makefile with test flag, compile tests, flash test helper
install_tests:
	make clean
	#always enable the bios during tests by default, since it is never a production build
	make all ADDITIONAL_CFLAGS="-DENABLE_BIOS -DENABLE_TEST $(ADDITIONAL_CFLAGS)"
	make flash
	make prepare_tests
	make flash_test

# Flash test helper
flash_test: $(PROGRAM_CONFIG)
	$(OPENOCD) -f $(PROGRAM_CONFIG) \
	-c "program $(TEST_TARGET_ELF) verify reset exit"

# Flash an invalid image in primary slot
flash_invalid: LOAD_ADDR = $(shell objdump $(TARGET_ELF) -t | grep "_stImgHeader$$" | grep -o "^........" | head -1)
flash_invalid:
	$(OPENOCD) -f $(PROGRAM_CONFIG) \
	-c "program $(HELLO_WORLD_BUILD_DIR)/image_wrong_sign.bin 0x$(LOAD_ADDR) verify reset exit"

# Also clean tests when cleaning everything
clean: clean_tests

clean_tests:
	make -C $(TEST_DIR) clean

generate_keys:
	make -f keygen.mk

include $(PATH_PORT)/lock.mk