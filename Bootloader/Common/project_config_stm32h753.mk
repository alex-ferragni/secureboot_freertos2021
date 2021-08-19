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

# Shortcuts:
PATH_GIT = /Users/afn/Desktop/github/secureboot_freertos2021

# Required variables:
PATH_MCUBOOT = $(PATH_GIT)/mcuboot
PATH_PORT = $(PATH_GIT)/Bootloader/Common/STM32H753
SECTIONS_CONFIG_DIR = $(PATH_GIT)/Bootloader/Common/template
BOOTLOADER_CONFIG = $(PATH_GIT)/Bootloader/bootloader_config_stm32h753.mk
TEST_CONFIG = $(PATH_GIT)/Bootloader/tests/test_config_stm32h753.mk
KEYS_PATH = $(PATH_GIT)/Bootloader/Common/keys

# Additional compilation variables:
CFLAGS += \
	-mcpu=cortex-m7 \
	-mfloat-abi=hard -mfpu=fpv5-sp-d16 \
	-mthumb

COMPILER := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OPENOCD := openocd-cortex