from flask import Flask, request, jsonify, render_template
from time import sleep
import sys
import serial

# Global Variables
WRITE_TIMES = 5

app = Flask(__name__)
ser = serial.Serial('COM6', 9600) # Establish connnection to Arduino

@app.route("/", methods=['GET','POST'])
def home():
    if request.method == 'POST':
        if request.form.get('smart_switch') == 'Smart Switch':
            char = str.encode('s')
            print("Smart Switch Toggled")
            for i in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
        elif request.form.get('intruder_alert') == 'Intruder Alert':
            char = str.encode('i')
            print("Intruder Alert Toggled")
            for i in range(WRITE_TIMES): ser.write(char) 
    return render_template('index.html')
if __name__ == "__main__":
    app.run(debug=False)