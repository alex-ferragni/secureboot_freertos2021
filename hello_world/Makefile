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

ROOT_DIR := $(abspath ..)
Q ?= @

COMMON_DIR = $(ROOT_DIR)/Bootloader/Common

TARGET ?= hello_world
KEY_SIGN ?= $(KEYS_PATH)/imgtool_keys/key_sign_priv.pem
KEY_ENC ?= $(KEYS_PATH)/imgtool_keys/key_enc_pub.pem

PROJECT_DIR = $(ROOT_DIR)/hello_world
BUILD_DIR  := $(PROJECT_DIR)/build
PROJECT_SRC_DIR = $(PROJECT_DIR)/src

SRC_FILES += \
  $(PROJECT_SRC_DIR)/main.c \
  $(COMMON_DIR)/src/startup_template.c \
  $(PATH_PORT)/src/drivers/flash.c \
  $(PATH_PORT)/src/drivers/uart.c

# Files necessary for the bios (easiest way to ensure image can download an update)
# (Otherwise, all of those can be removed but image should handle this itself)
SRC_FILES += \
  $(COMMON_DIR)/src/bios.c \
  $(PATH_PORT)/src/bios_port/bios.c \
  $(PATH_PORT)/src/drivers/security.c \
	$(COMMON_DIR)/src/image_util/image_util.c \
	$(COMMON_DIR)/src/image_util/image_download.c \
	$(COMMON_DIR)/src/image_util/image_upload.c \
  $(PATH_PORT)/src/st/rss.c

INCLUDE_PATHS := -I$(PROJECT_DIR)/Includes

LDSCRIPT = $(COMMON_DIR)/link_template.ld

$(OBJ_FILES): Makefile

# -DENABLE_BIOS can be removed if you do not want the bios, see comment above
ADDITIONAL_CFLAGS +=  \
	-DHAL_INCLUDE_ALL \
  -DENABLE_BIOS \
  -DDISABLE_SAFEGUARD

include $(COMMON_DIR)/Makefile