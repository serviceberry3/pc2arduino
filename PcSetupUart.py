#!/usr/bin/python
# main.py
  
import usb
import _thread
import serial
import os


def setup():
	
	
	busses = usb.busses()
	handle = 0
	print ("Setting up...")
	for bus in busses:
		print ("Getting bus devices...")
		devices = bus.devices
		for dev in devices:
			if dev.idVendor == 0x2341:
				print("Found Arduino")
				handle = dev.open()
				
				#set configuration to the first and only configuration for the Arduino
				handle.setConfiguration(dev.configurations[0])

				#looking for the second interface (usb-serial) of the Arduino Uno, but since we removed the Arduino USB DFU bootloader firmware (used for flashing MCU), this will be the only interface left
				handle.claimInterface(dev.configurations[0].interfaces[0][0])
				

    #set the control line state to 0 according to CDCClassCommon.h in LUFA-lib                        
	handle.controlMsg(requestType = 0x21, request = 34, value = 0, index = 0, buffer = 0, timeout = 0)

    #set the line encoding 8N1, 9600 bits/sec
	handle.controlMsg(requestType = 0x21, request = 32, value = 0, index = 0, buffer = [0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08], timeout = 0)

	print ("Done with setup")
	return handle
		
def sendCommand(handle):
	#prompt user for one byte to send
	while True:
		msg = input("Enter byte in hex (0xnn): ")
		if msg=="Read":
			#ret = ard.read(ard.inWaiting())
			#print ("Message from arduino: ")
			#print (ret)
			ret = handle.bulkRead(0x83, 1, 500)
			print(chr(ret[0]))
			return
		try:
			num = int(msg, 16)
			print ("Sending "+ hex(num))
			handle.bulkWrite(0x04, [num,], 0)
		except Exception as inst:
			print (inst)
			print ("ERROR: Cannot parse number")

if __name__=='__main__':
	handle = setup()
	sendCommand(handle)
