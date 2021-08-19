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

from config.test_config import *
from config.config import *
from typing import NamedTuple
from itertools import chain, combinations
import os

# Definition of a single test
class TestDef(NamedTuple):
	# Name of the test
	test_name: str
	# Image file that should be loaded in primary slot for this test; None if slot should be empty
	primary_name: str
	# Image file that should be loaded in secondary slot for this test; None if slot should be empty
	secondary_name: str
	# Will the test modify the content of images? (should we reload them for next test?)
	modify_images: bool
	# Expected sequence of commands
	expected_sequence: list #list of (sent message + expected response) (two chars)

# Performs a group by image pair
# @param tests the list of tests
# @return the tests grouped by image pair
def naive_group_by(tests):
	out = {}
	for test in tests:
		pair = (test.primary_name, test.secondary_name)
		if (pair not in out):
			out[pair] = []
		out[pair].append(test)
	return out

# Computes the power set of an array
# @param iterable the iterable
# @return its power set
def powerset(iterable):
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))

# Listens for a test menu
# Waits for a "$>" sequence, then returns the following character
# @param ser the serial handler
# @return the test menu identifier
def receive_char(ser):
	token1 = ser.read().decode(encoding, 'replace')
	token2 = ser.read().decode(encoding, 'replace')
	char = ser.read().decode(encoding, 'replace')

	while (token1 is not '$' or token2 is not '>'):
		token1 = token2
		token2 = char
		char = ser.read().decode(encoding, 'replace')
	
	return char

# Get the full image path by looking at the configuration
# @param image_name the image name (without path nor extension)
# @return the full image file path
def get_full_image_path(image_name):
	return image_build_dir+"/"+image_name+".bin"

# Nicely print results of tests
# @param results the results of the tests
def print_results(results):
	print("\n\nSummary of tests:")
	print(str(len([1 for succeeded in results.values() if succeeded]))+"/"+str(len(results))+" tests successful\n")

	for name, succeeded in results.items():
		if not succeeded:
			print("\t* Failed *: "+name)

	print("")

	for name, succeeded in results.items():
		if succeeded:
			print("\t* Succeeded *: "+name)
			
	print("")

# Checks that tests are well-formed and that requested images exist
# Throws an error if any test is invalid
# @param tests the tests
def validate_tests(tests):
	for test in tests:
		assert test is not None, "test should not be None" 
		assert test.test_name is not None, "test name should not be None"

		if (test.primary_name is not None):
			full_path = get_full_image_path(test.primary_name)
			assert os.path.isfile(full_path), "test image ('"+full_path+"') should exist"
		if (test.secondary_name is not None):
			full_path = get_full_image_path(test.secondary_name)
			assert os.path.isfile(full_path), "test image ('"+full_path+"') should exist"

		assert test.expected_sequence is not None, "expected sequence should not be None"
		for action in test.expected_sequence:
			assert type(action) is str, "expected sequence should be made of strings"
			assert len(action) is 2, "action should be made of two characters"