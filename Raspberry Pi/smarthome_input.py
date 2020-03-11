from time import sleep
import serial
import curses

stdscr = curses.initscr()
ser = serial.Serial('/dev/ttyACM0', 9600) # Establish connnection to Arduino
while True:
    char = stdscr.getch()
    if chr(char) is 'q':
        break
    else:
        ser.write(str(chr(char))) # Send char to Arduino
        ser.write(str(chr(char))) # Send char to Arduino
        ser.write(str(chr(char))) # Send char to Arduino
        ser.write(str(chr(char))) # Send char to Arduino
        ser.write(str(chr(char))) # Send char to Arduino
print("Done!")