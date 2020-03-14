import serial
from time import sleep
ser = serial.Serial('COM6', 9600)
import pathlib
import codecs

path = pathlib.Path(__file__).parent.absolute()
while True:
    file = open(str(path) + '\\' + 'data.txt','w+')
    ser.flushInput()
    ser.flushOutput()
    ser_bytes = ser.readline()
    byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
    print(byteline)
    while len(byteline) != 5:
        print("ERROR ^")
        sleep(0.01)
        ser_bytes = ser.readline()
        byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
        print(str(byteline))
    file.write(codecs.encode(ser_bytes,'base64','strict').decode("utf-8"))
    file.close()