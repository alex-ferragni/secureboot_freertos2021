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

import time
import os
import serial
import sys
import serial.tools.list_ports
from typing import NamedTuple
from config.config import *

# Send an image to UART
# @param image_name the name of the file in from which data will be read
# @param ser the serial handler
# @param verbose whether prints are enabled
def send_image(image_name, ser, verbose=False):
	assert os.path.isfile(image_name), "test image ('"+image_name+"') should exist"
	size = os.path.getsize(image_name)
	
	ack = ':'
	ser.write(ack.encode('utf-8'))
	ser.write(size.to_bytes(4, byteorder = 'little'))

	f = open(image_name, 'rb')
	data = f.read(32)

	sent = 0
	if verbose:
		print("sending image...")
	while(data):
		while(len(data) != 32):
			data += bytes([255])
		ser.read() #wait for signal
		ser.write(data) #then send chunk
		data = f.read(32)
		sent = sent + 32
		if verbose and sent % 1024 == 0:
			print(str(((10000*sent)//size)/100)+"%")
	if verbose:
		print("\ndone!")

# If the function is used as a stand-alone tool, define a main function
if __name__ == "__main__":

	ports = list(serial.tools.list_ports.comports())
	found = False

	assert len(sys.argv) >= 2, "an argument (full image path) is required!"

	for p in ports:
		if ("STLINK-V3" in p[1]):
			found = True
			ser = serial.Serial(p[0], baudrate = baudrate, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

			print(p[0]+" was detected as a valid port!")
			
			send_image(sys.argv[1], ser, True)
			
			break

	if (not found):
		print("No valid port detected!")