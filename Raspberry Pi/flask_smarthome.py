# Import Libraries
from flask import Flask, request, render_template
import serial

# Global Variables
WRITE_TIMES = 5
SERIAL_OPEN = True
DEBUG = False

app = Flask(__name__) # Create Flask object
try:
    ser = serial.Serial('COM6', 9600) # Establish connection to Arduino
except:
    SERIAL_OPEN = False # If no Arduino detected, still run flask webpage
    print("\n\t######################################")
    print("\tERROR: Serial Port Not Open/Connected.")
    print("\t       Running without Arduino...")
    print("\t######################################\n")

# Main Website
@app.route("/", methods=['GET','POST'])
def home():
    if SERIAL_OPEN:
        ser.flushOutput() # Clean output (prevents overloading of serial port)
        ser.flushInput() # Clean input
        if request.method == 'POST': # If button pressed
            if request.form.get('smart_switch') == 'Smart Switch':
                char = str.encode('s')
                for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
            elif request.form.get('intruder_alert') == 'Intruder Alert':
                char = str.encode('i')
                for _ in range(WRITE_TIMES): ser.write(char) # Send char to Arduino
    return render_template('index.html') # Returns the website

@app.route("/data/")
def get_serial(): # Prints Arduino status to /data webpage
    if SERIAL_OPEN:
        ser.flushOutput() # Flush output
        ser.flushInput() # Flush input
        ser_bytes = ser.readline() # Read status of Arduino
        byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',') # Convert to list
        if DEBUG: print(byteline)
        while len(byteline) != 5 or len(byteline[0]) == 0: # While error detected in list
            if DEBUG: print("--------- ERROR ----------")
            ser_bytes = ser.readline()
            byteline = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(',')
            if DEBUG: print(byteline)
        return str([int(i) for i in byteline])[1:-1] # Return list to /data
    else: return "0, 0, 0, 0, 0" # If Arduino not connected, return dummy values
    

# Start Flask Server
if __name__ == "__main__":
    app.run(debug=False)