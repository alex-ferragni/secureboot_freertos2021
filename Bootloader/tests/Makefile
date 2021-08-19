# 
#     Copyright 2021 CSEM SA
# 
#     Licensed under the Apache License, Version 2.0 (the "License");
#     you may not use this file except in compliance with the License.
#     You may obtain a copy of the License at
# 
#         http://www.apache.org/licenses/LICENSE-2.0
# 
#     Unless required by applicable law or agreed to in writing, software
#     distributed under the License is distributed on an "AS IS" BASIS,
#     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#     See the License for the specific language governing permissions and
#     limitations under the License.
# 

# Path of the project-specific config (should be shared with images to be compatible)
ifndef PROJECT_CONFIG
$(error PROJECT_CONFIG is not set)
endif

include $(PROJECT_CONFIG)

ifndef TEST_CONFIG
$(error TEST_CONFIG is not set)
endif

include $(TEST_CONFIG)

BOOTLOADER_DIR := $(abspath ..)
Q ?= @

COMMON_DIR = $(BOOTLOADER_DIR)/Common

PROJECT_DIR = $(BOOTLOADER_DIR)/tests
BUILD_DIR  := $(PROJECT_DIR)/build
PROJECT_SRC_DIR = $(PROJECT_DIR)/src

SRC_FILES += \
  $(PROJECT_SRC_DIR)/main.c \
  $(PROJECT_SRC_DIR)/startup.c \
  $(PATH_PORT)/src/drivers/flash.c \
  $(PATH_PORT)/src/drivers/uart.c

# Files necessary for the bios
SRC_FILES += \
  $(COMMON_DIR)/src/bios.c \
  $(PATH_PORT)/src/bios_port/bios.c \
  $(PATH_PORT)/src/drivers/security.c \
	$(COMMON_DIR)/src/image_util/image_util.c \
	$(COMMON_DIR)/src/image_util/image_download.c \
	$(COMMON_DIR)/src/image_util/image_upload.c

INCLUDE_PATHS := -I$(PROJECT_DIR)/Includes \

#not used
KEY_ENC := none
#not used
KEY_SIGN := none
LDSCRIPT = $(PROJECT_DIR)/link.ld

$(OBJ_FILES): Makefile

ADDITIONAL_CFLAGS += -DENABLE_TEST -DHAL_INCLUDE_ALL
TARGET := test_helper

default:elf

include $(COMMON_DIR)/Makefile