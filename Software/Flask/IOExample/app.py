from flask import Flask
from flask import request
from flask import render_template

app = Flask(__name__)

def GetIndex():
    Temperature = 0.0

    if(request.method == "GET"):
        if(request.args.get("action_button")):
            File = open("/sys/class/thermal/thermal_zone0/temp")
            Temperature = float(File.read()) / 1000.0
            print("[DEBUG] Temperature: {}".format(Temperature))

    return render_template("index.html", Data = Temperature)

app.add_url_rule("/", "index", GetIndex, methods = ["GET"])

if(__name__ == "__main__"):
    app.run(debug = True, host = "0.0.0.0")