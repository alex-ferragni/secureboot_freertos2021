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

ifndef PROJECT_CONFIG
$(error PROJECT_CONFIG is not set)
endif

include $(PROJECT_CONFIG)

ifndef OUTPUT_DIR
$(error OUTPUT_DIR is not set)
endif

default: create_folders generate_enc generate_sign

KEY_ENC_NAME := key_enc
KEY_SIGN_NAME := key_sign

KEY_ENC_PRIV_PEM := $(OUTPUT_DIR)/$(KEY_ENC_NAME)_priv.pem
KEY_ENC_PUB_PEM := $(OUTPUT_DIR)/imgtool_keys/$(KEY_ENC_NAME)_pub.pem
KEY_ENC_PRIV_C := $(OUTPUT_DIR)/Includes/keys/$(KEY_ENC_NAME)_priv.c.import

KEY_SIGN_PRIV_PEM := $(OUTPUT_DIR)/imgtool_keys/$(KEY_SIGN_NAME)_priv.pem
KEY_SIGN_PUB_PEM := $(OUTPUT_DIR)/$(KEY_SIGN_NAME)_pub.pem
KEY_SIGN_PUB_C := $(OUTPUT_DIR)/Includes/keys/$(KEY_SIGN_NAME)_pub.c.import

IMGTOOL := $(PATH_MCUBOOT)/scripts/imgtool.py

create_folders:
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(OUTPUT_DIR)/Includes/keys
	@mkdir -p $(OUTPUT_DIR)/imgtool_keys

# will only work on mac! on llinux, remove the emtpy quotes in the sed command
generate_enc:
	@echo "generating private encryption key '$(KEY_ENC_PRIV_PEM)'..."
	$(IMGTOOL) keygen -k $(KEY_ENC_PRIV_PEM) -t ecdsa-p256
	@echo "extracting public encryption key to be used by images..."
	openssl ec -in $(KEY_ENC_PRIV_PEM) -pubout -out $(KEY_ENC_PUB_PEM)
	@echo "converting private encryption key to an MCUboot-readable format..."
	@$(IMGTOOL) getpriv -k $(KEY_ENC_PRIV_PEM) > $(KEY_ENC_PRIV_C).temp
	@sed -i "" "s/const unsigned/SECURE_STORAGE const unsigned/g" $(KEY_ENC_PRIV_C).temp
	@echo '#define SECURE_STORAGE __attribute__((section(".secure_storage")))\n' > $(KEY_ENC_PRIV_C)
	@cat $(KEY_ENC_PRIV_C).temp >> $(KEY_ENC_PRIV_C)
	@rm $(KEY_ENC_PRIV_C).temp
	@rm $(KEY_ENC_PRIV_PEM)

generate_sign:
	@echo "generating private signature key '$(KEY_SIGN_PRIV_PEM)' to be used by images..."
	$(IMGTOOL) keygen -k $(KEY_SIGN_PRIV_PEM) -t ecdsa-p256
	@echo "extracting public signature key..."
	openssl ec -in $(KEY_SIGN_PRIV_PEM) -pubout -out $(KEY_SIGN_PUB_PEM)
	@echo "converting public signature key to an MCUboot-readable format..."
	@$(IMGTOOL) getpub -k $(KEY_SIGN_PUB_PEM) > $(KEY_SIGN_PUB_C)
	@rm $(KEY_SIGN_PUB_PEM)