# Import Libraries
from flask import Flask, request, jsonify, render_template
import time
from time import sleep
import sys
import serial

# Global Variables
WRITE_TIMES = 5
nl = ac = ss = intruder = False
temp = 70

app = Flask(__name__) # Create Flask object
ser = serial.Serial('COM6', 9600) # Establish connection to Arduino
ser.flushInput()

# Main Website
@app.route("/", methods=['GET','POST'])
def home():
    ser.flushInput()
    ser.flushOutput()
    if request.method == 'POST':
        if request.form.get('smart_switch') == 'Smart Switch':
            char = str.encode('s')
            for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
        elif request.form.get('intruder_alert') == 'Intruder Alert':
            char = str.encode('i')
            for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
    sleep(0.03)
    ser_bytes = ser.readline()
    byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
    print(byteline)
    while len(byteline) != 5:
        print("ERROR ^")
        sleep(0.01)
        ser_bytes = ser.readline()
        byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
        print(str(byteline))
    nl = int(byteline[0])
    ac = int(byteline[1])
    ss = int(byteline[2])
    temp = int(byteline[3])
    intruder = int(byteline[4])
    return render_template('index.html', nl=nl, ac=ac, ss=ss, temp=temp, intruder=intruder)


# Start Flask Server
if __name__ == "__main__":
    app.run(debug=False)