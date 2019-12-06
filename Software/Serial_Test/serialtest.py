import serial
import time

# python -m serial.tools.list_ports
SerPort = '/dev/ttyACM0'
ser_obj = serial.Serial(SerPort, 57600, timeout=0, write_timeout=0)
now_time = time.time()
duration = 0

ser_obj.write(200)
time.sleep(2)

lower_mask = 0x007F
def twos_complement(val, bits):
	return 2 ** bits - abs(val) if val < 0 else val

val = 50

i1 = i4 = 0

i2 = val
i3 = -val

while True:
	num1 = twos_complement(i1, 14)
	num2 = twos_complement(i2, 14)
	num3 = twos_complement(i3, 14)
	num4 = twos_complement(i4, 14)
	num5 = 180
	char1 = num1>>7
	char2 = num1 & lower_mask
	char3 = num2>>7
	char4 = num2 & lower_mask
	char5 = num3>>7
	char6 = num3 & lower_mask
	char7 = num4>>7
	char8 = num4 & lower_mask
	char9 = num5
	print_res = [char1,char2,char3,char4,char5,char6,char7,char8,char9,255]
	byte_res = bytearray(print_res)
	ser_obj.write(byte_res)

	time.sleep(1)

	if ser_obj.in_waiting > 0:
		linelist = ser_obj.readlines(ser_obj.in_waiting)
		for line in linelist:
			print(line.decode('utf-8'))

