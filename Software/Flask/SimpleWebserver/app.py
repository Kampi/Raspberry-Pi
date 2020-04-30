from flask import Flask
from flask import render_template

app = Flask(__name__)

def GetIndex():
    return render_template("index.html")

app.add_url_rule(rule = "/", endpoint = "index", view_func = GetIndex)

if(__name__ == "__main__"):
    app.run(host = "0.0.0.0")