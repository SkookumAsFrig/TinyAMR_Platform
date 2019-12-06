from inputs import devices
from evdev import InputDevice, categorize, ecodes, KeyEvent
import time
import threading

import serial
import time

# python -m serial.tools.list_ports
SerPort = '/dev/ttyACM0'
ser_obj = serial.Serial(SerPort, 57600, timeout=0, write_timeout=0)

lower_mask = 0x007F

def twos_complement(val, bits):
	return 2 ** bits - abs(val) if val < 0 else val

ser_obj.write(200)
time.sleep(2)

events = devices.gamepads[0].get_char_device_path()
gamepad = InputDevice(events)

"""
press the directional pad  ---- ABS event
release the dirctional pad ---- SYN event 
"""
global i1
global i2
global i3
global i4
i1 = 0
i2 = 0
i3 = 0
i4 = 0

global spd_mode
spd_mode = 1

def gamepad_control(gamepad, speed):
	global spd_mode
	global i1
	global i2
	global i3
	global i4
	for event in gamepad.read_loop():
		# For sticks, the range of their value is -32768-32767 (16-bit)
		# right and down direction - 32767, left and up direction - neg 32768
		if event.type == ecodes.EV_ABS:
			absevent = categorize(event)
			stick_id = ecodes.bytype[absevent.event.type][absevent.event.code]
			# left stick operation
			if stick_id == 'ABS_X':
				event_time = absevent.event.timestamp()
			elif stick_id == 'ABS_Y':
				if abs(absevent.event.value) < 160:
					oneval = 0
				else:
					oneval = absevent.event.value

				if spd_mode == 1:
					i1 = i2 = (int)(oneval/4)
				else:
					i1 = i2 = (int)(oneval/16)
			# right stick operation
			elif stick_id == 'ABS_RX':
				event_time = absevent.event.timestamp()
			elif stick_id == 'ABS_RY':
				if abs(absevent.event.value) < 160:
					two_val = 0
				else:
					two_val = absevent.event.value
				
				if spd_mode == 1:
					i3 = i4 = (int)(two_val/4)
				else:
					i3 = i4 = (int)(two_val/16)

		elif event.type == ecodes.EV_KEY:
			keyevent = categorize(event)
			if keyevent.keystate == KeyEvent.key_down:
				print(keyevent.keycode)
				if keyevent.keycode[0] == 'BTN_A':
					if spd_mode == 1:
						spd_mode = 2
					else:
						spd_mode = 1
					print('Back')
				elif keyevent.keycode[1] == 'BTN_Y':
					print('Foward')
				elif keyevent.keycode[0] == 'BTN_B':
					print('Right')
				elif keyevent.keycode[1] == 'BTN_X':
					print('Left')
				elif keyevent.keycode == 'BTN_THUMBR' or keyevent.keycode == 'BTN_THUMBL':
					print('Stop')
				elif keyevent.keycode == 'BTN_TR':
					speed += 50
					if speed > 550:
						print('Maximum speed: 550')
						speed = 550
					else:
						print('Faster: {} -> {}'.format(speed - 50, speed))
				elif keyevent.keycode == 'BTN_TL':
					speed -= 50
					if speed < 50:
						print('Minimum speed: 50')
						speed = 50
					else:
						print('Slower: {} -> {}'.format(speed + 50, speed))


if __name__ == '__main__':

	speed = 50
	
	gamepad_thread = threading.Thread(target=gamepad_control, args=(gamepad, speed))
	gamepad_thread.start()	

	while True:
		num1 = twos_complement(i1, 14)
		num2 = twos_complement(i2, 14)
		num3 = twos_complement(i3, 14)
		num4 = twos_complement(i4, 14)
		num5 = 150
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
		linelist = ser_obj.readlines(ser_obj.in_waiting)
		for line in linelist:
			print(line.decode('utf-8'))
		time.sleep(0.05)	
