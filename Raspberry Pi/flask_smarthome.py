# Import Libraries
from flask import Flask, request, jsonify, render_template
import time
from time import sleep
import sys
import serial

# Global Variables
WRITE_TIMES = 5

app = Flask(__name__) # Create Flask object
ser = serial.Serial('COM6', 9600) # Establish connnection to Arduino
ser.flushInput()

# Main Website
@app.route("/", methods=['GET','POST'])
def home():
    if request.method == 'POST':
        ser_bytes = ser.readline()
        byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        ser.flushInput()
        ser.flushOutput()
        print(byteline)
        if request.form.get('smart_switch') == 'Smart Switch':
            char = str.encode('s')
            start = time.time()
            for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
            end = time.time()
            print("Smart Switch Toggled, Time: " + str(end - start))
        elif request.form.get('intruder_alert') == 'Intruder Alert':
            char = str.encode('i')
            start = time.time()
            for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
            end = time.time()
            print("Intruder Alert Toggled, Time: " + str(end - start))
    return render_template('index.html')

# Start Flask Server
if __name__ == "__main__":
    app.run(debug=False)
