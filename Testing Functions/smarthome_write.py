from time import sleep
import serial
from msvcrt import getch
import os

ser = serial.Serial('COM6', baudrate=9600, timeout=1) # Establish connnection to Arduino
WRITE_TIMES = 5
DEBUG = False

num = 1
while True:
    if DEBUG: print(str(num) + ") Reading -> ",end='')
    num += 1
    char = getch().decode('ascii')
    if char == 'q':
        break
    else:
        if DEBUG: print("Writing -> ",end='')
        for x in range(WRITE_TIMES): 
            i = ser.write(char.encode('utf-8'))
            if DEBUG: print(str(i),end='')
        if DEBUG: print(" -> Done!")
if DEBUG: print("'q' Pressed, Exiting!")