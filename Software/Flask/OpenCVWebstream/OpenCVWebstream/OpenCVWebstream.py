import os
import cv2
import time
import numpy

from threading import Thread, Lock
from multiprocessing import Process
from flask import Flask
from flask import request
from flask import jsonify
from flask import Response
from flask import render_template

from .Camera import Camera

class OpenCVWebstream():
    def __init__(self, CameraID = 0, onStop = None, IP = "0.0.0.0", Port = 5000):
        self.__app = Flask(__name__)

        # On stop handler
        self.__onStop = onStop

        self.__OutputImage = None

        self.__VideoPaused = True

        self.__Camera = Camera()

        # Create a blank image for the video pause (this prevents the browser from loading all the time)
        self.__Blank = numpy.zeros((100, 100, 3), numpy.uint8)

        # Register endpoints
        self.__app.add_url_rule(rule = "/", endpoint = "/", view_func = self.__showIndex)
        self.__app.add_url_rule(rule = "/VideoFrame", endpoint = "VideoFrame", view_func = self.__getVideoFrame)
        self.__app.add_url_rule(rule = "/post", endpoint = "post", view_func = self.__getData, methods = ["POST"])

        self.__Server = Thread(target = self.__flaskThread, args = (IP, Port,))
        self.__Server.setDaemon(True)

    def __getFrame(self):
        while(True):
            if(not(self.__VideoPaused)):
                Image = self.__Camera.GetFrame((320, 240))
            else:
                Image = self.__Blank

            # Save the image as jpeg
            (_, Image) = cv2.imencode(".jpg", Image)

            yield(b"--frame\r\n"
                b"Content-Type: image/jpeg\r\n\r\n" + bytearray(Image) + b"\r\n")

            # Firefox needs some time to display image
            time.sleep(0.01)    

    def __flaskThread(self, IP, Port):
        self.__app.run(host = IP, port = Port, threaded = True)

    """
    Endpoints
    """
    def __getData(self):
        if(request.method == "POST"):
            Input = request.json
            
            if(Input == "Start"):
                self.StartCapture()
            elif(Input == "Pause"):
                self.PauseCapture()
            elif(Input == "Shutdown"):
                self.Stop()

            return jsonify(Input)

    def __showIndex(self):   
        return render_template("index.html", data = {"Paused" : self.__VideoPaused})

    def __getVideoFrame(self):
        return Response(self.__getFrame(), mimetype = "multipart/x-mixed-replace; boundary=frame")

    """
    Public methods
    """
    def StartCapture(self):
        print("[DEBUG - {}] Capture started!".format(time.strftime("%H:%M:%S")))

        self.__VideoPaused = False

    def PauseCapture(self):
        print("[DEBUG - {}] Capture paused!".format(time.strftime("%H:%M:%S")))

        self.__VideoPaused = True

    def Run(self):
        print("[DEBUG - {}] Starting server...".format(time.strftime("%H:%M:%S")))

        # Start the server
        self.__Server.start()

    def Stop(self):
        print("[DEBUG - {}] Shutdown server...".format(time.strftime("%H:%M:%S")))

        self.__Camera.Stop()

        if(self.__onStop is not None):
            self.__onStop()