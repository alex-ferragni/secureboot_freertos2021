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

ifndef PROGRAM_CONFIG
$(error PROGRAM_CONFIG is not set)
endif

set_security:
	make set_security_ SEC=3
unset_security:
	make set_security_ SEC=1

set_security_: $(PROGRAM_CONFIG)
	openocd-cortex -f $(PROGRAM_CONFIG) \
	$(subst {SEC},$(SEC),$(SET_SECURITY_CMD))

mass_erase:
	openocd-cortex -f $(PROGRAM_CONFIG) \
	-c "init" \
	-c "halt" \
	-c "stm32h7x mass_erase 0" \
	-c "stm32h7x mass_erase 1" \
	-c "shutdown"

#Unlocks the flash control register
#Allows to modify flash settings until next reboot or lock
#Use the lock counterpart to apply changes
FLASH_UNLOCK_REGISTER_CMD := -c "echo \"Unlocking flash control registers... \"" \
	-c "mww 0x52002008 0x08192A3B" \
	-c "mww 0x52002008 0x4C5D6E7F"
FLASH_LOCK_REGISTER_CMD := -c "echo \"Applying changes & locking flash control registers...\"" \
	-c "mww 0x52002018 3" \
	-c "sleep 500"

#Sets the Flash security mode
#SEC should be C (disabled) or E (enabled)
#Warning: after enabling it, the debugger will not be able to halt/reset the board
#In particular, "make flash" won't work...
#Fortunately, "make unset_security" doesn't need to halt the target and fixes the problem
SET_SECURITY_CMD := -c "init" \
	-c "echo \" ----OLD VALUE---- \"" \
	-c "mdw 0x5200201C" \
	$(FLASH_UNLOCK_REGISTER_CMD) \
	-c "mww 0x52002020 0x10{SEC}6AAF0" \
	-c "echo \" ----VALUE TO BE APPLIED---- \"" \
	-c "mdw 0x52002020" \
	$(FLASH_LOCK_REGISTER_CMD) \
	-c "echo \" ----NEW VALUE---- \"" \
	-c "mdw 0x5200201C" \
	-c "shutdown"
#FIXME: should only modify bit 21 regardless of content of other bits, without hardcoding their default value...

#Prepare the board & reset it
INIT_CMD := -c "init" \
	-c "reset init"

#Reset the board & shutdown gdb session
SHUTDOWN_CMD := -c "reset" \
	-c "shutdown"