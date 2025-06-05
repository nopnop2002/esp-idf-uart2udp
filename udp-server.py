#!/usr/bin/env python3
#-*- encoding: utf-8 -*-
import sys
import select, socket
import time
import signal
import argparse

def handler(signal, frame):
	print('handler')
	sys.exit()

if __name__=='__main__':
	signal.signal(signal.SIGINT, handler)
	parser = argparse.ArgumentParser()
	parser.add_argument('--port', type=int, help='udp port', default=8080)
	args = parser.parse_args()
	print("args.port={}".format(args.port))

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
	# It can only be received when the sender is using Limited Broadcast.
	#sock.bind(('<broadcast>', args.port))
	# It can receive broadcasts regardless of whether the sender is Limited Broadcast/Directed Broadcast/Unicast.
	sock.bind(('', args.port))
	sock.setblocking(0)

	while True:
		result = select.select([sock],[],[])
		msg = result[0][0].recv(1024)
		if (type(msg) is bytes):
			msg=msg.decode('utf-8')
		msg = msg.rstrip('\r')
		msg = msg.rstrip('\n')
		print(msg)
