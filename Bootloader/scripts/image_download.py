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

# Receive an image from UART and store it in a file
# @param image_name the name of the file in which the image will be stored
# @param ser the serial handler
# @param verbose whether prints are enabled
def receive_image(image_name, ser, verbose=False):

	f = open(image_name, 'wb')

	if verbose:
		print("contacting device...")

	ack = ':'
	ser.write(ack.encode('utf-8'))

	data = ser.read(8)
	size = int(data, 16)

	if verbose:
		print("received metadata, size=%d" % size)
		print("receiving image...")

	received = 0
	while(received < size):

		#signal we want a flash word
		ack = ':'
		ser.write(ack.encode('utf-8'))

		for i in range(8):
			data = ser.read(8)

			number = int(data, 16)
			#print (number)
			f.write(number.to_bytes(4, byteorder='big'))
			received += 4

			if verbose and received % 1024 == 0:
				print(str(((10000*received)//size)/100)+"%")

	if verbose:
		print("\ndone!")

# If the function is used as a stand-alone tool, define a main function
if __name__ == "__main__":

	ports = list(serial.tools.list_ports.comports())
	found = False

	assert len(sys.argv) >= 2, "an argument (output image file) is required!"

	for p in ports:
		if ("STLINK-V3" in p[1]):
			found = True
			ser = serial.Serial(p[0], baudrate = baudrate, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

			print(p[0]+" was detected as a valid port!")
			
			receive_image(sys.argv[1], ser, True)
			
			break

	if (not found):
		print("No valid port detected!")