#!/usr/bin/python

import mraa
import random
import binascii
import time
import Queue

from struct import *
from thread import *

class BlueRiderSpi:
	def __init__(self):
		self.txQueue = Queue.Queue(maxsize=0)
		self.rxQueue = Queue.Queue(maxsize=0)
		self.motor = mraa.Spi(0)
		self.motor.mode(3)
		self.motor.frequency(8000000)
		
		self.rxEscape = 0
		self.txEscape = 0
		
		self.edisonSyncCount = 9
		self.txSyncCount = self.edisonSyncCount
		start_new_thread(self.spiThread, ())
	
	def receiveByte(self, rxByte):
		insertByte = -1
		
		if (self.rxEscape):
			self.rxEscape = 0

			if (rxByte == 0xA1): # Invalid byte
				insertByte = 0x00
			elif (rxByte == 0xA2): # Escape byte
				insertByte = 0xA5
			elif (rxByte == 0xA3): # Idle byte
				insertByte = 0xFF
			elif (rxByte == 0xA5): # Escape byte
				self.rxEscape = 1
			elif ((rxByte == 0x00) or (rxByte == 0xFF)):
				insertByte = -1 # Do nothing
			else:
				insertByte = rxByte
		else:
			if (rxByte == 0xA5): # Escape byte
				self.rxEscape = 1
			elif ((rxByte == 0x00) or (rxByte == 0xFF)): # Invalid byte or Idle byte
				insertByte = -1 # Do nothing			
			else:
				insertByte = rxByte
		
		if (insertByte >= 0):
			self.rxQueue.put(insertByte & 0xFF)
	
	def sendByte(self):
		sendByte = 0xFF
		if (self.txEscape != 0):
			sendByte = self.txEscape
			self.txEscape = 0
		elif (self.txQueue.empty() == False):
			sendByte = self.txQueue.get()
			if (sendByte == 0x00): # Invalid byte
				sendByte = 0xA5
				self.txEscape = 0xA1
			elif (sendByte == 0xA5): # Escape byte
				sendByte = 0xA5
				self.txEscape = 0xA2
			elif (sendByte == 0xFF): # Idle byte
				sendByte = 0xA5
				self.txEscape = 0xA3
			
			# else: Do nothing
		return sendByte
	
	def spiThread(self):
		# Initialize communication
		txbuf = bytearray(1)
		
		txId = 0
		txRemain = 0
		while 1:
			if (txRemain != 0):
				txbuf[0] = txRemain
				txRemain = 0
			else:
				txbuf[0] = self.sendByte()
			#txbuf[1] = self.sendByte()
			#txbuf[2] = self.sendByte()
			#if (txbuf[2] == 0xA5):
			#	txbuf[2] = 0xFF
			#	txRemain = 0xA5
			#txbuf[3] = 0xFF
			
			while 1:
				rxbuf = self.motor.write(txbuf)
				
				# Parse the rx buffer
				self.receiveByte(rxbuf[0])
				#self.receiveByte(rxbuf[1])
				#self.receiveByte(rxbuf[2])
				#self.receiveByte(rxbuf[3])
				
				#if ((rxbuf[0] != 0) and (rxbuf[1] != 0) and (rxbuf[2] != 0) and (rxbuf[3] != 0)):
				if (rxbuf[0] != 0):
					break

	def read(self):
		return 

	def write(self):
		return
