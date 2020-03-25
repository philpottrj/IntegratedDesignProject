# Import Libraries
from flask import Flask, request, jsonify, render_template
import time
from time import sleep
import sys
import serial

# Global Variables
WRITE_TIMES = 5
SERIAL_OPEN = True
DEBUG = False

app = Flask(__name__) # Create Flask object
try:
    ser = serial.Serial('COM6', 9600) # Establish connection to Arduino
    ser.flushInput()
except:
    SERIAL_OPEN = False
    print("\n\t######################################")
    print("\tERROR: Serial Port Not Open/Connected.")
    print("\t       Running without Arduino...")
    print("\t######################################\n")

# Main Website
@app.route("/", methods=['GET','POST'])
def home():
    if SERIAL_OPEN:
        ser.flushOutput()
        ser.flushInput()
        if request.method == 'POST':
            if request.form.get('smart_switch') == 'Smart Switch':
                char = str.encode('s')
                for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
            elif request.form.get('intruder_alert') == 'Intruder Alert':
                char = str.encode('i')
                for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
    return render_template('index.html')

@app.route("/data/")
def get_serial():
    ser.flushOutput()
    ser.flushInput()
    ser_bytes = ser.readline()
    byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
    if DEBUG: print(byteline)
    while len(byteline) != 5:
        if DEBUG: print("ERROR ^")
        sleep(0.01)
        ser_bytes = ser.readline()
        byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
        if DEBUG: print(str(byteline))
    return str([int(i) for i in byteline])[1:-1]
    

# Start Flask Server
if __name__ == "__main__":
    app.run(debug=False)