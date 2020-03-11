from flask import Flask, render_template_string, request   # Importing the Flask modules 
from time import sleep      # Import sleep module from time library 
app = Flask(__name__)
#HTML Code 
TPL = '''
<html>
    <head><title>Web Page Controlled Servo</title></head>
    <body>
    <h2> Web Page to Control Servo</h2>
        <form method="POST" action="test">
            <h3> Use the slider to rotate servo  </h3>
            <p>Slider   <input type="range" min="1" max="12.5" name="slider" /> </p>
            <input type="submit" value="submit" />
            <p> Button <button type="button name "ss">Toggle Smart Switch</button>
        </form>
    </body>
</html>

'''
@app.route("/")
def home():                                                                                                                                                         
    return render_template_string(TPL)                        
@app.route("/test", methods=["POST"])
def test():
    # Get variable Values
    slider = request.form["slider"]
    button = request.form["ss"]
    print(slider)
    print(button)
    sleep(1)
    return render_template_string(TPL)
# Run the app on the local development server
if __name__ == "__main__":
    app.run()