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

import serial
import serial.tools.list_ports
from test_utils import print_results
from test_core import execute_tests
from config.test_config import *
from config.config import *


# Test main entry point; runs all tests
if __name__ == "__main__":

	ports = list(serial.tools.list_ports.comports())
	found = False

	for p in ports:
		if ("STLINK-V3" in p[1]):
			found = True
			ser = serial.Serial(p[0], baudrate = baudrate, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

			print(p[0]+" was detected as a valid port!")
			results = execute_tests(ser)
			print_results(results)
			break

	if (not found):
		print("No valid port detected!")