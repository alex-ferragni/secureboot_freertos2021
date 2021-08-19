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

from test_utils import *
from test_cases import *
from image_upload import send_image

# Execute all tests
# @param ser the serial handler
# @return the results of the tests, on the form [test_name -> booloean_result]
def execute_tests(ser):
	# First, check that tests are well-formed
	validate_tests(all_tests)

	# Group tests by images, to avoid a few re-flash
	tests_by_images = naive_group_by(all_tests)

	i = 1
	print("Initialization done, starting tests...")
	print("Found "+str(len(all_tests))+" tests, using "+str(len(tests_by_images))+" different image pairs")
	
	# Initially, nothing in both slots
	current_primary = "x"
	current_secondary = "x"

	results = {}

	print(" *** Waiting for UART response. Reset the board to start the tests")

	# Any sign of life from the device starts the tests
	response = receive_char(ser)

	# Forall tests with te same pair of images
	for image_pair, tests in tests_by_images.items():
		image_primary, image_secondary = image_pair
		print("===== Tests using "+str(image_primary)+" (primary) and "+str(image_secondary)+" (secondary):",len(tests),"=====")

		# Forall tests
		for test in tests:

			# Reset the board to start fresh
			ser.write(b'r')

			# If the currently loaded primary image needs to change, change it
			if (image_primary is not current_primary):
				# Thet test requests an empty slot, erase it
				if (image_primary is None):
					print(" *** Erasing primary slot...")

					# Goto test, erase primary
					response = receive_char(ser)
					ser.write(b't')
					response = receive_char(ser)
					ser.write(b'x')
				# Thet test requests a specific image, load it
				else:
					print(" *** Installing "+image_primary+" in primary slot...")

					# Goto test, send image in primary
					response = receive_char(ser)
					ser.write(b't')
					response = receive_char(ser)
					ser.write(b'p')
					send_image(get_full_image_path(image_primary),ser)

				print(" *** Done!")
				current_primary = image_primary
			
			# If the currently loaded secondary image needs to change, change it
			if (image_secondary is not current_secondary):
				# Thet test requests an empty slot, erase it
				if (image_secondary is None):
					print(" *** Erasing secondary slot...")

					# Goto test, erase secondary
					response = receive_char(ser)
					ser.write(b't')
					response = receive_char(ser)
					ser.write(b'y')
				else:
					print(" *** Installing "+image_secondary+" in secondary slot...")

					# Goto test, send image in secondary
					response = receive_char(ser)
					ser.write(b't')
					response = receive_char(ser)
					ser.write(b's')
					send_image(get_full_image_path(image_secondary),ser)

				print(" *** Done!")
				current_secondary = image_secondary

			#reset images if we might have corrupted them by doing a swap
			if(test.modify_images):
				#name it anything that is not an image, next test will automatically reload image for next test
				current_primary = "x"
				current_secondary = "x"

			print("(%d/%d):" % (i, len(all_tests)), test.test_name)

			success = True

			# Now the test can begin, reset board
			response = receive_char(ser)
			ser.write(b'r')
			print(' > r', end=';')

			response = receive_char(ser)
			print(' < '+response, end=';')

			mismatch = ""

			# Follow the test actions
			for action in test.expected_sequence:
				# Send command
				ser.write(action[0].encode(encoding, 'replace'))
				print(' > '+action[0], end=';')

				# Receive response
				response = receive_char(ser)
				print(' < '+response, end=';')

				# Check against expected response
				if (action[1] is not response):
					mismatch = "Expected '"+action[1]+"' but received '"+response+"'"
					success = False
					break

			print("")
			results[test.test_name] = success
			if (success):
				print("\t* Success *")
				#success
			else:
				print("\t"+mismatch)
				print("\t* Failure *")
				#failure

			i += 1
	return results