import serial
import time
from bitstring import Bits as bits

# python -m serial.tools.list_ports
SerPort = '/dev/ttyUSB0';
ser_obj = serial.Serial(SerPort, 115200, timeout=0.1)
now_time = time.time()
duration = 0


lower_mask = 0x00FF
def twos_complement(val, bits):
	return 2 ** bits - abs(val) if val < 0 else val
while True:
	duration = time.time() - now_time
	if duration > 2:
		res = ""
		now_time = time.time()
		num1 = twos_complement(-30000, 16)
		num2 = twos_complement(-25000, 16)
		num3 = twos_complement(-20000, 16)
		num4 = twos_complement(-10000, 16)
		num5 = twos_complement(-100, 8)
		char1 = chr(num1>>8)
		char2 = chr(num1 & lower_mask)
		char3 = chr(num2>>8)
		char4 = chr(num2 & lower_mask)
		char5 = chr(num3>>8)
		char6 = chr(num3 & lower_mask)
		char7 = chr(num4>>8)
		char8 = chr(num4 & lower_mask)
		char9 = chr(num5)
		print_res = [char1,char2,char3,char4,char5,char6,char7,char8,char9]
		print(print_res)
		print(''.join(print_res).encode())
		ser_obj.write(''.join(print_res).encode())

	if ser_obj.in_waiting > 0:
		linelist = ser_obj.readlines(ser_obj.in_waiting)
		for line in linelist:
			print(line.decode('utf-8'))

