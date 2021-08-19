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

from test_utils import powerset,TestDef
from config.test_config import *
from enum import Enum

class Slot(Enum):
	PRIMARY = 0
	SECONDARY = 1
	SCRATCH = 2

class Swap(Enum):
	ONE_THIRD = 0
	TWO_THIRDS = 1

# Returns commands to corrupt a given slot
# @param slot_ids an array containing slots (0-2) to corrupt
# @return the corresponding commands
def corrupt_command(slot_ids):
	command = []
	for id in slot_ids:
		# Issue corrupt primary command
		if id == Slot.PRIMARY:
			command = command + [('ct')]
		# Issue corrupt secondary command
		elif id == Slot.SECONDARY:
			command = command + [('dt')]
		# Issue corrupt scratch command
		elif id == Slot.SCRATCH:
			command = command + [('et')]
	return command

# Returns commands tomark an image for update and perform the update
# Handles the fact that MCUboot will prompt 2 test menus during the swap
# Resets the system during the swap as requested by the input
# @param resets an array containing the resets (Swap.ONE_THIRD = at 1/3, Swap.TWO_THIRDS = at 2/3 of the update)
# @return the corresponding commands
def swap_command(resets):
	# First, mark image in secondary for update, reset the board, let MCUboot start and wait at 1/3 progress
	command = [('ut'),('ri'),('cm')]

	if Swap.ONE_THIRD in resets:
		# If we want to reset at 1/3, reset, then tell the bootloader to continue with MCUboot
		command = command + [('ri'),('cm')]

	# In any case, device should be waiting at first MCUboot menu, so continue
	command = command + [('cm')]

	if Swap.TWO_THIRDS in resets:
		# If we want to reset at 2/3, reset, then tell the bootloader to continue with MCUboot
		# Also tell MCUboot to advance to 2/3
		command = command + [('ri'),('cm'),('cm')]
	
	# In any case, device should be be done with both MCUboot menu, so expect a result
	# Image was installing, so it should be valid; expect a successful boot
	command = command + [('cv')]
	return command

# Generates a series of tests
#	- Check that two valid images can be swapped, even with multiple resets and corrupted scratch
#	- Check that valid update (no primary) should be applied, even with multiple resets and corrupted primary or scratch
#	- Check that invalid update is refused, even with corrupted primary or scratch
#	- Check that valid primary image should be booted, even with corrupted secondary or scratch
#	- Check that invalid primary image should not be booted, even with corrupted secondary or scratch
#	- Check that update can be reverted, even with multiple resets
# @return the generated tests
def generate_tests():
	generated_tests = []
	for corrupted in powerset([Slot.PRIMARY,Slot.SCRATCH]):
		for resets in powerset([Swap.ONE_THIRD,Swap.TWO_THIRDS]):

			# Both valid images, expect correct swap
			name = "Two valid image should be swapped, resets = "+str(resets)+", corrupted sectors = "+str(corrupted)

			new_test = 	TestDef(name,
								image_valid_primary,
								image_valid_secondary,
								True,
								[('tt')] + corrupt_command(corrupted) + swap_command(resets))
			generated_tests = generated_tests + [new_test]

			if Slot.PRIMARY not in corrupted:
				# If the primary image is invalid, no point in trying to corrupt it, would be the same as previous test
				# Only secondary is valid, Expect correct swap
				name = "Valid image should be applied, resets = "+str(resets)+", corrupted sectors = "+str(corrupted)

				new_test = 	TestDef(name,
									None,
									image_valid_secondary,
									True,
									[('tt')] + corrupt_command(corrupted) + swap_command(resets))
				generated_tests = generated_tests + [new_test]
		
		# Invalid update, expect failure in swapping
		name = "Invalid image should not be applied, corrupted sectors = "+str(corrupted)
		new_test = 	TestDef(name,
							None,
							image_invalid_signature_secondary,
							True,
							[('tt')] + corrupt_command(corrupted) + [('ut'),('ri'),('cx')])
		generated_tests = generated_tests + [new_test]

	for corrupted in powerset([Slot.SECONDARY,Slot.SCRATCH]):
		# Valid primary, expect successful boot
		name = "Valid image should be verified, corrupted sectors = "+str(corrupted)
		new_test = 	TestDef(name,
							image_valid_primary,
							None,
							True,
							[('tt')] + corrupt_command(corrupted) + [('ri'),('cv')])

		generated_tests = generated_tests + [new_test]

		# Invalid primary, expect no boot
		name = "Invalid image should not be verified, corrupted sectors = "+str(corrupted)
		new_test = 	TestDef(name,
							image_invalid_signature_primary,
							None,
							True,
							[('tt')] + corrupt_command(corrupted) + [('ri'),('cx')])
		
		generated_tests = generated_tests + [new_test]

	for resets in powerset([Swap.ONE_THIRD,Swap.TWO_THIRDS]):

			# Both valid images, expect correct first update, corrupt primary, expect correct second swap
			name = "Valid image should be applied then reverted, resets = "+str(resets)

			new_test = 	TestDef(name,
								image_valid_primary,
								image_valid_secondary,
								True,
								[('tt')] + swap_command(resets) + [('ri'),('tt'),('ct')] + swap_command(resets))
			generated_tests = generated_tests + [new_test]
	return generated_tests

all_tests = [
				# Trigger a fault, observe a fault
				TestDef("Fault reaction test",
						None,
						None,
						False,
						[('tt'),('hh')]),
				# Install invalid image, don't boot
				TestDef("Modified image should not be verified",
						image_corrupted_primary,
						None,
						False,
						[('cx')]),
				# Write to bootloader, expect a fault
				TestDef("Image cannot write to bootloader area",
						None,
						None,
						False,
						[('tt'),('wh')]),
				# Try to read secret key, expect all 0s
				TestDef("Image cannot read bootloader's secret key",
						None,
						None,
						False,
						[('tt'),('ks')]),
				# Don't install anything, don't boot
				TestDef("Empty primary and secondary slots should not be verified",
						None,
						None,
						False,
						[('tt'),('ri'),('cx')]),
				# Install an image, corrupt it, don't boot
				TestDef("Bootloader cannot verify a valid image if its first slot is corrupted",
						image_valid_primary,
						None,
						True,
						[('tt'),('ct'),('ri'),('cx')]),
				# No primary image, install valid update, corrupt it, cannot swap and boot
				TestDef("Bootloader cannot decrypt and apply a valid image if its first slot is corrupted",
						None,
						image_valid_secondary,
						True,
						[('tt'),('dt'),('ut'),('ri'),('cx')]),
			] + generate_tests()