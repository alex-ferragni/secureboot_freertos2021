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

# All include paths
ifndef INCLUDE_PATHS
$(error INCLUDE_PATHS is not set)
endif

# Target name
ifndef TARGET
$(error TARGET is not set)
endif

# Linker script (specific to the image)
ifndef LDSCRIPT
$(error LDSCRIPT is not set)
endif

# Output build directory
ifndef BUILD_DIR
$(error BUILD_DIR is not set)
endif

# List of all source files
ifndef SRC_FILES
$(error SRC_FILES is not set)
endif

# Root directory (useful for logging purposes)
ifndef ROOT_DIR
$(error ROOT_DIR is not set)
endif

# Path of the MCUboot repo/fork folder
ifndef PATH_MCUBOOT
$(error PATH_MCUBOOT is not set)
endif

# Path of the folder of files common to the bootloader and the images
ifndef COMMON_DIR
$(error COMMON_DIR is not set)
endif

# Path of the folder of the porting (HW-dependent files)
ifndef PATH_PORT
$(error PATH_PORT is not set)
endif

# Path of the folder containing sections_config.ld (configures MCUboot's area sizes)
ifndef SECTIONS_CONFIG_DIR
$(error SECTIONS_CONFIG_DIR is not set)
endif

# optional
ASM_FILES ?=
ADDITIONAL_CFLAGS ?=

# Add files that always need to be included
SRC_FILES += \
	$(COMMON_DIR)/src/mcuboot_flash.c \
	$(PATH_MCUBOOT)/boot/bootutil/src/bootutil_public.c

Q ?= @

COMPILER ?= arm-none-eabi-gcc
OBJCOPY ?= arm-none-eabi-objcopy
OPENOCD ?= openocd

COMMON_SRC_DIR = $(COMMON_DIR)/src

OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/C/%.o,$(SRC_FILES))
OBJ_FILES += $(patsubst $(ROOT_DIR)/%.s,$(BUILD_DIR)/ASM/%.o,$(ASM_FILES))

# Include useful paths by default
INCLUDE_PATHS += \
	-I$(PATH_MCUBOOT)/boot/bootutil/include \
	-I$(COMMON_DIR)/Includes \
	-I$(PATH_PORT)/Includes

# A few MbedTLS flags are necessary to compile
CFLAGS += \
	-g3 \
	-nostdlib \
	-ffunction-sections -fdata-sections \
	-Wall \
	-fdebug-prefix-map=$(ROOT_DIR)=. \
	-Dunix \
	$(ADDITIONAL_CFLAGS) \
	-Og

MEMORY_MAP := $(COMMON_DIR)/memory-sections.ld
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf

LDFLAGS += -L$(COMMON_DIR) -L$(PATH_PORT) -L$(SECTIONS_CONFIG_DIR) -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map,$(BUILD_DIR)/$(TARGET).map

DEP_DIR = $(BUILD_DIR)/dep
DEP_CFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$(*F).d

IMG_TOOL := $(PATH_MCUBOOT)/scripts/imgtool.py

-include $(wildcard $(DEP_DIR)/*.d)

clean:
	rm -rf $(BUILD_DIR)

# shortcut
elf: $(TARGET_ELF)

# elf product file, contains no header (not signed, not encrypted)
# ready to be flashed in case of the bootloader, otherwise has to go though the imgtool first
$(TARGET_ELF): $(OBJ_FILES) $(LDSCRIPT) $(MEMORY_MAP)
	@echo "Linking library"
	@$(COMPILER) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) $(INCLUDE_PATHS) -o $@ -Wl,-lc -Wl,-lgcc
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

$(OBJ_FILES): $(COMMON_DIR)/Makefile $(COMMON_DIR)/core.mk

$(BUILD_DIR)/C/%.o: $(ROOT_DIR)/%.c | $(BUILD_DIR) $(DEP_DIR)
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(COMPILER) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

$(BUILD_DIR)/ASM/%.o: $(ROOT_DIR)/%.s | $(BUILD_DIR) $(DEP_DIR)
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(COMPILER) $(DEP_CFLAGS) -x assembler-with-cpp $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<