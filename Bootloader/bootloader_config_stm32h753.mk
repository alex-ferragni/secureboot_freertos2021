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

ADDITIONAL_CFLAGS += \
	-DMCUBOOT_USE_STM32H753_CRYP \
	-DENABLE_BIOS \
	-DSAFE_ECC_DBERRORS \
	-DDISABLE_SAFEGUARD
	
###### STM32H753_CRYP files ######
# contains functions necessary to make the link between MCUboot's interface
# and common's hw crypto drivers
STM32H753_CRYP_DIR := $(PATH_MCUBOOT)/ext/stm32h753
STM32H753_CRYP_INC_DIR := $(STM32H753_CRYP_DIR)
STM32H753_CRYP_SRC_DIR := $(STM32H753_CRYP_DIR)

STM32H753_CRYP_SRC_FILES := \
	$(STM32H753_CRYP_SRC_DIR)/cryp/stm32h753_aes_ctr_glue.c \
	$(STM32H753_CRYP_SRC_DIR)/hash/stm32h753_sha256_glue.c \
	$(STM32H753_CRYP_SRC_DIR)/hash/stm32h753_hmac_sha256_glue.c

STM32H753_CRYP_INC_PATHS := -I$(STM32H753_CRYP_INC_DIR)

SRC_FILES += $(STM32H753_CRYP_SRC_FILES)
INCLUDE_PATHS += $(STM32H753_CRYP_INC_PATHS)

# Note: $(PATH_PORT)/src/st/rss.c is under St's licence, it cannot be open source
SRC_FILES += $(PATH_PORT)/src/st/rss.c