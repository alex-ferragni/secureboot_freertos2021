# 
# FreeRTOS V202012.00
# Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# http://www.FreeRTOS.org
# http://aws.amazon.com/freertos
# 
# 1 tab == 4 spaces!
# 

# Path of the project-specific config (should be shared with images to be compatible)
ifndef PROJECT_CONFIG
$(error PROJECT_CONFIG is not set)
endif

include $(PROJECT_CONFIG)

ROOT_DIR := $(abspath ../..)

COMMON_DIR = $(ROOT_DIR)/Bootloader/Common

FREERTOS_ROOT_DIR := $(ROOT_DIR)/FreeRTOS-Kernel
ST_CODE_DIR := $(ROOT_DIR)/FreeRTOS-Image/ST_Code

KEY_SIGN ?= $(KEYS_PATH)/imgtool_keys/key_sign_priv.pem
KEY_ENC ?= $(KEYS_PATH)/imgtool_keys/key_enc_pub.pem

FREERTOS_PORT ?= ARM_CM4_MPU

FREERTOS_PORT_ROOT += \
  $(FREERTOS_ROOT_DIR)/portable/GCC/$(FREERTOS_PORT)

FREERTOS_KERNEL_SOURCES += \
  $(FREERTOS_ROOT_DIR)/croutine.c \
  $(FREERTOS_ROOT_DIR)/event_groups.c \
  $(FREERTOS_ROOT_DIR)/list.c \
  $(FREERTOS_ROOT_DIR)/queue.c \
  $(FREERTOS_ROOT_DIR)/stream_buffer.c \
  $(FREERTOS_ROOT_DIR)/tasks.c \
  $(FREERTOS_ROOT_DIR)/timers.c \
  $(FREERTOS_ROOT_DIR)/portable/MemMang/heap_4.c \
  $(FREERTOS_ROOT_DIR)/portable/Common/mpu_wrappers.c \
  $(FREERTOS_PORT_ROOT)/port.c

ST_CODE_SOURCES += \
  $(wildcard $(ST_CODE_DIR)/Core/Src/*.c) \
  $(wildcard $(ST_CODE_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/*.c)

ST_CODE_SOURCES := $(filter-out $(ST_CODE_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/%_template.c, $(ST_CODE_SOURCES))
ST_CODE_SOURCES := $(filter-out $(ST_CODE_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c, $(ST_CODE_SOURCES))

PROJECT_SRC_DIR = $(ROOT_DIR)/FreeRTOS-Image
BUILD_DIR  := $(PROJECT_SRC_DIR)/build

SRC_FILES += \
  $(PROJECT_SRC_DIR)/App/src/app_main.c \
  $(PROJECT_SRC_DIR)/App/src/flash.c \
  $(PROJECT_SRC_DIR)/Startup/syscalls.c \
  $(PROJECT_SRC_DIR)/Startup/sysmem.c \
  $(ST_CODE_SOURCES) \
  $(FREERTOS_KERNEL_SOURCES)

# Files necessary for the bios (easiest way to ensure image can download an update)
# (Otherwise, all of those can be removed but image should hande this itself)
SRC_FILES += \
  $(COMMON_DIR)/src/bios.c \
  $(PATH_PORT)/src/bios_port/bios.c \
	$(COMMON_DIR)/src/image_util/image_util.c \
	$(COMMON_DIR)/src/image_util/image_download.c \
	$(COMMON_DIR)/src/image_util/image_upload.c \
  $(PATH_PORT)/src/drivers/security.c \
  $(PATH_PORT)/src/st/rss.c

ASM_FILES := $(PROJECT_SRC_DIR)/Startup/startup_stm32h743zitx.s

INCLUDE_PATHS += \
  -I$(FREERTOS_ROOT_DIR)/include \
  -I$(PROJECT_SRC_DIR)/Config \
  -I$(PROJECT_SRC_DIR)/App/include \
  -I$(ST_CODE_DIR)/Core/Inc \
  -I$(ST_CODE_DIR)/Drivers/CMSIS/Include \
  -I$(ST_CODE_DIR)/Drivers/CMSIS/Device/ST/STM32H7xx/Include \
  -I$(ST_CODE_DIR)/Drivers/STM32H7xx_HAL_Driver/Inc \
  -I$(ST_CODE_DIR)/Drivers/STM32H7xx_HAL_Driver/Legacy/Inc \
  -I$(FREERTOS_PORT_ROOT)

ADDITIONAL_CFLAGS += \
	-DUSE_HAL_DRIVER -DSTM32H743xx \
  -DENABLE_BIOS \
  -DHAL_INCLUDE_SECURITY -DHAL_INCLUDE_SCB \
  -DDISABLE_SAFEGUARD


TARGET := freertos
LDSCRIPT = $(PROJECT_SRC_DIR)/System/STM32H743ZITX_FLASH.ld

include $(COMMON_DIR)/Makefile

$(OBJ_FILES): Makefile