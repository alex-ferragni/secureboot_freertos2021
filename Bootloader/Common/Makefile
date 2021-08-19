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

# This makefile should be used to produce an MCUboot-compatible image
# A few variables should be set, the rest is configured automatically
# Required variables:

# Output build directory
ifndef BUILD_DIR
$(error BUILD_DIR is not set)
endif

# List of source files
ifndef SRC_FILES
$(error SRC_FILES is not set)
endif

# List of include pahts
ifndef INCLUDE_PATHS
$(error INCLUDE_PATHS is not set)
endif

# Target name
ifndef TARGET
$(error TARGET is not set)
endif

# Location of linker script
ifndef LDSCRIPT
$(error LDSCRIPT is not set)
endif

# Location of the MCUboot repo/fork folder
ifndef PATH_MCUBOOT
$(error PATH_MCUBOOT is not set)
endif

# Path of the folder of the porting (HW-dependent files)
ifndef PATH_PORT
$(error PATH_PORT is not set)
endif

# Private ECDSA key used to sign the image (should match the bootloader's public one)
ifndef KEY_SIGN
$(error KEY_SIGN is not set)
endif

# Public ECIES key used to encrypt the image (should match the bootloader's private one)
ifndef KEY_ENC
$(error KEY_ENC is not set)
endif

# Path of the folder containing sections_config.ld (configures MCUboot's area sizes)
ifndef SECTIONS_CONFIG_DIR
$(error SECTIONS_CONFIG_DIR is not set)
endif

ROOT_DIR ?= $(abspath ..)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

#get the location of this makefile
COMMON_DIR := $(mkfile_dir)

TARGET_BIN = $(BUILD_DIR)/$(TARGET)-no-header.bin
IMG_BIN = $(BUILD_DIR)/$(TARGET).bin
IMG_ENC = $(BUILD_DIR)/$(TARGET)_enc.bin

all: $(IMG_BIN) $(IMG_ENC)

# contains core of makefile (common to both images and bootloader)
include $(COMMON_DIR)/core.mk

elf: $(TARGET_ELF)

# handy aliases
flash: flash_img
flash_sec: flash_upgd

# flash a signed & unencrypted image to primary slot
flash_img: LOAD_ADDR = $(shell objdump $(TARGET_ELF) -t | grep "_stImgHeader$$" | grep -o "^........" | head -1)
flash_img: $(IMG_BIN)
	$(OPENOCD) -f $(PATH_PORT)/stlink.cfg \
	-c "program $(IMG_BIN) 0x$(LOAD_ADDR) verify reset exit"

# flash a signed & encrypted image to secondary slot
flash_upgd: LOAD_ADDR = $(shell objdump $(TARGET_ELF) -t | grep "_stImg2Header$$" | grep -o "^........" | head -1)
flash_upgd: $(IMG_ENC)
	$(OPENOCD) -f $(PATH_PORT)/stlink.cfg \
	-c "program $(IMG_ENC) 0x$(LOAD_ADDR) verify reset exit"

# image bin, ready to be flashed, signed & unencrypted
$(IMG_BIN): IMG_SIZE = $(shell objdump $(TARGET_ELF) -t | grep "_lnImg$$" | grep -o "^........" | head -1)
$(IMG_BIN): HEADER_SIZE = $(shell objdump $(TARGET_ELF) -t | grep "_lnImgHeader$$" | grep -o "^........" | head -1)
$(IMG_BIN): $(TARGET_BIN) $(MEMORY_MAP)
	$(IMG_TOOL) sign -k $(KEY_SIGN) -E $(KEY_ENC) --plaintext --align 32 -v 1.0.0 -H 0x$(HEADER_SIZE) -S 0x$(IMG_SIZE) --pad-header $(TARGET_BIN) $(IMG_BIN)
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"

# image bin, ready to be flashed, signed & encrypted
$(IMG_ENC): IMG_SIZE = $(shell objdump $(TARGET_ELF) -t | grep "_lnImg$$" | grep -o "^........" | head -1)
$(IMG_ENC): HEADER_SIZE = $(shell objdump $(TARGET_ELF) -t | grep "_lnImgHeader$$" | grep -o "^........" | head -1)
$(IMG_ENC): $(TARGET_BIN) $(MEMORY_MAP)
	$(IMG_TOOL) sign -k $(KEY_SIGN) -E $(KEY_ENC) --align 32 -v 1.0.0 -H 0x$(HEADER_SIZE) -S 0x$(IMG_SIZE) --pad-header $(TARGET_BIN) $(IMG_ENC)
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"

# intermediate product, input of imgtool
$(TARGET_BIN): $(TARGET_ELF)
	@$(OBJCOPY) $< $@ -O binary
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"

