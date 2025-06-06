#!/usr/bin/python3
#-*- encoding: utf-8 -*-
import sys
import argparse
import subprocess
import socket
import time
import signal

def getAddress(host):
	command = "getent ahostsv4 {}".format(host)
	ret = subprocess.run(command, shell=True, capture_output=True, text=True)
	#print("ret.returncode={}".format(ret.returncode))
	if (ret.returncode != 0):
		return None

	#print("ret.stdout={}".format(ret.stdout))
	lines = ret.stdout.splitlines()
	#print("lines={}".format(lines))
	lines = lines[0].split()
	#print("lines={}".format(lines))
	return lines[0]
	
def handler(signal, frame):
	global running
	print('handler')
	running = False

if __name__=='__main__':
	signal.signal(signal.SIGINT, handler)
	running = True

	parser = argparse.ArgumentParser()
	parser.add_argument('--host', help='udp host', default="esp32.local")
	parser.add_argument('--port', type=int, help='udp port', default=9090)
	args = parser.parse_args()
	print("args.host={}".format(args.host))
	print("args.port={}".format(args.port))

	ip = getAddress(args.host)
	print("ip={}".format(ip))
	if (ip is None):
		print("{} not found in network".format(args.host))
		sys.exit()

	#serv_address = ('192.168.10.109', 8080)
	serv_address = (ip, args.port)

	while running:
		client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		t = time.time()
		local_time = time.localtime(t)
		asc_time = time.asctime(local_time)
		print("asc_time={}".format(asc_time))
		bytes_time = bytes(asc_time, 'utf-8')
		send_len = client.sendto(bytes_time, serv_address)
		#print('closing socket')
		client.close()
		time.sleep(1)

