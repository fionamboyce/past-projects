import serial, csv, time, sys

#from pykeyboard import PyKeyboard
#import pyautogui

from pynput.keyboard import Key, Controller

keyboard = Controller()

ser = serial.Serial(port='/dev/cu.HC06_RESETTEST-DevB', baudrate=9600)
ser.close()
ser.open()
k=""
#keyboard = PyKeyboard()

threshold = 3.00

#MMG1 = "0"
#MMG2 = "0"

while 1:

	a = ser.read()
	#if a != ',':
	k+=a
	
	#ser.close()
	if a == '\n':
		#print(k)
		MMG1 = k[0:4]
		MMG2 = k[5:9]
		k=""
		#print(MMG1)

		try: 
			float(MMG1)
			if float(MMG1) > threshold:
				print("left")
				#keyboard.tap_key(keyboard.left_key)
				#pyautogui.press(['left'])
				keyboard.press(Key.left)
				keyboard.release(Key.left)
		except ValueError:
			print("hoooo")

		try: 
			float(MMG2)
			if float(MMG2) > threshold:
				print("right")
				#keyboard.press_key('f')
				#pyautogui.press(['right'])
				keyboard.press(Key.right)
				keyboard.release(Key.right)
		except ValueError:
			print("no reading")



		
		









#if maxval >180.0:
	#print('computer do something')
#else:
	#print('computer do nothing')