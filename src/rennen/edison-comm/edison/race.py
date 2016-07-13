#!/usr/bin/python

from bluerider.BlueRiderSpi import *
from printing_utils import *
from time import *
import signal

MAX_VALID_POS = 1750

# Receiving message types
POS_MSG = 1
START_MSG = 2

# Sending message types
SOLL_V_LANE_MSG = 1

default_speed = 20
route_info = dict()
# route_info[>>position in interrupts<<] = (speed, lane)
# TODO: check values and sector positions
route_info[0]    = (20, 2)		# Sector 1
route_info[150]  = (20, 2)		# Sector 2
route_info[260]  = (25, 2)		# Sector 3
route_info[330]  = (30, 2)
route_info[365]  = (40, 2)		# Sector 4
route_info[420]  = (20, 2)
route_info[485]  = (18, 2)
route_info[495]  = (18, 2)		# Sector 5
route_info[520]  = (20, 2)
route_info[580]  = (20, 2)
route_info[700]  = (30, 2)		# Sector 6
route_info[725]  = (19, 2)
route_info[745]  = (18, 2)		# Sector 7
route_info[870]  = (25, 2)		# Sector 8
route_info[910]  = (23, 2)
route_info[950]  = (23, 2)		# Sector 9
route_info[1100] = (25, 2)		# Sector 10
route_info[1140] = (20, 2)
route_info[1185] = (20, 2)		# Sector 11
route_info[1255] = (100, 2)		# Sector 12
route_info[1380] = (100, 2)		# Sector 13
route_info[1410] = (100, 2)		# Sector 14

blueComm = BlueRiderSpi()

def send(msg_id, sollv, soll_lane = 0xFF):
	if blueComm.txQueue.full():
		print_error("Can not send %d:%d:%d because the queue is full!" % (msg_id, sollv, soll_lane))
		return

	soll_lane |= 0x80

	checksum = (msg_id + sollv + soll_lane) & 0xFF
	print_sending("%d:%d:%d:%d" % (msg_id, sollv, soll_lane, checksum))
	print_sollv("%d" % sollv)
	print_soll_lane("%d" % soll_lane)

	blueComm.txQueue.put(msg_id & 0xFF)
	blueComm.txQueue.put(sollv & 0xFF)
	blueComm.txQueue.put(soll_lane & 0xFF)
	blueComm.txQueue.put(checksum & 0xFF)

def receive():
	while blueComm.rxQueue.empty():
		sleep(0.005)

	rx_list = [4, -1, -1, -1]

	while (rx_list[0] >> 2) != 0:
		rx_list[0] = blueComm.rxQueue.get()

	for i in range(1, 4):
		rx_list[i] = blueComm.rxQueue.get()

	print_received("%d:%d:%d:%d" % (rx_list[0], rx_list[1], rx_list[2], rx_list[3]))
	checksum = (rx_list[0] + rx_list[1] + rx_list[2] + rx_list[3]) & 0xFF

	if checksum != rx_list[3]:
		print_error("Checksum is %d and should be %d (msg: %d:%d:%d:%d)" % (checksum, rx_list[3], rx_list[0], rx_list[1], rx_list[2], rx_list[3]))
		return (START_MSG, 0)
	print_position("%d" % pos)

	return (rx_list[0], pos)

def get_sollv_and_soll_lane(pos):
	needed_key = -1
	for key in route_info:
		if key <= pos and needed_key < key:
			needed_key = key

	return route_info[needed_key]

def main():
	print_status("Cleaning rxQueue...")
	while not blueComm.rxQueue.empty():
		blueComm.rxQueue.get()

	print_status("Ready to handle messages...")

	while 1:
		print_cur_time()
		rec = receive()
		if not rec:
			continue

		msg_id, position = rec

		if msg_id == POS_MSG:
			sollv, soll_lane = get_sollv_and_soll_lane(position)
			send(SOLL_V_LANE_MSG, sollv, soll_lane)
		elif msg_id == START_MSG:
			send(SOLL_V_LANE_MSG, default_speed)
		else:
			print_error("Received message with invalid id: %d!" % msg_id)

def sigint_handler(signum, frame):
	for _ in range(4):
		send(SOLL_V_LANE_MSG, 0)

	shutdown()
	exit()

if __name__ == "__main__":
	signal.signal(signal.SIGINT, sigint_handler)
	print("\033[2J")
	print_received("")
	print_sending("")
	print_position("")
	print_sollv("")
	print_soll_lane("")
	print_cur_time("")
	print_error("None so far! ;)")
	main()
