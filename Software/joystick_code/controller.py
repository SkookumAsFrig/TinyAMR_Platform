from inputs import devices
from evdev import InputDevice, categorize, ecodes, KeyEvent
import time
import threading


events = devices.gamepads[0].get_char_device_path()
gamepad = InputDevice(events)

"""
press the directional pad  ---- ABS event
release the dirctional pad ---- SYN event 
"""
def gamepad_control(gamepad, speed):
	for event in gamepad.read_loop():
		# For sticks, the range of their value is -32768-32767 (16-bit)
		# right and down direction - 32767, left and up direction - neg 32768
		if event.type == ecodes.EV_ABS:
			absevent = categorize(event)
			print(ecodes.bytype[absevent.event.type][absevent.event.code], absevent.event.value)
			# left stick operation
			if ecodes.bytype[absevent.event.type][absevent.event.code] == 'ABS_X':
				event_time = absevent.event.timestamp()
			if ecodes.bytype[absevent.event.type][absevent.event.code] == 'ABS_Y':
				event_time = absevent.event.timestamp()
			# right stick operation
			if ecodes.bytype[absevent.event.type][absevent.event.code] == 'ABS_RX':
				event_time = absevent.event.timestamp()
			if ecodes.bytype[absevent.event.type][absevent.event.code] == 'ABS_RY':
				event_time = absevent.event.timestamp()
		elif event.type == ecodes.EV_SYN:
			synevent = categorize(event)
			print(synevent)
			syntime = synevent.event.timestamp()
			move_duration = (syntime - event_time) * 1000
			print('Move duration: {}'.format(move_duration))
		elif event.type == ecodes.EV_KEY:
			keyevent = categorize(event)
			if keyevent.keystate == KeyEvent.key_down:
				print(keyevent.keycode)
				if keyevent.keycode[0] == 'BTN_A':
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
		print('test')
		time.sleep(1)	
