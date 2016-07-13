#!/usr/bin/python

from bluerider.BlueRiderSpi import *

from time import *

blueComm = BlueRiderSpi()

print ("\033[2J")

curData = 0

startTime = float(time())
dataNumber = 0
dataRate = 0.0
while 1:
	blueComm.txQueue.put((curData >> 24) & 0xFF)
	blueComm.txQueue.put((curData >> 16) & 0xFF)
	blueComm.txQueue.put((curData >> 8) & 0xFF)
	blueComm.txQueue.put(curData & 0xFF)
	
	if (blueComm.rxQueue.empty() == False):
		rxByte = blueComm.rxQueue.get() << 24
		rxByte |= blueComm.rxQueue.get() << 16
		rxByte |= blueComm.rxQueue.get() << 8
		rxByte |= blueComm.rxQueue.get()
		
		dataNumber += 4
		
		timeDiff = (float(time()) - startTime)
		if ((dataNumber % 100) == 0):
			dataRate = (dataNumber / timeDiff)
		
		print ("\033[3;3HID\033[3;11H%10d %d B/s (%d Bytes in %fs)" % (rxByte, dataRate, dataNumber, timeDiff))
		#time.sleep(0.1)
	curData = (curData + 1)
