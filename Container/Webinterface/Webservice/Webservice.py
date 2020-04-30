import datetime

from flask import Flask, render_template, url_for, request

from flask_socketio import SocketIO, emit

from threading import Thread

class Webservice(object):
	def __init__(self, Data):		
		self.__Data = Data
		self.__ThreadRunning = False
		self.__WebKeys = []
	
		# Flask setup
		self.__FlaskNamespace = "Raspberry"
		self.__app = Flask(__name__)
		self.__socketio = SocketIO(self.__app, async_mode = None)
		
		# Show some websites
		self.__app.add_url_rule("/", "index", self.__showIndex)
		self.__app.add_url_rule("/shutdown", "shutdown", self.__shutdown, methods = ["GET"])
		
		# Register event for getting post data from web
		self.__app.add_url_rule("/transmitData", "transmitData", self.__getPostJavascriptData, methods = ["POST"])
		
		# Setup the thread for flask
		self.__flaskThread = Thread(target = self.__flaskThreadCall)
		
	def __flaskThreadCall(self):
		if(self.__ThreadRunning):
			self.__socketio.run(self.__app, host = "0.0.0.0")
		
	def __getPostJavascriptData(self):
		if(request.method == "POST"):
			print("{} - Receive data from web...".format(datetime.datetime.now().time()))
			
			JSON = request.get_json()
			
			if(JSON["Element"] in self.__WebKeys):
				if(JSON["Element"] in self.__Data):
					self.__Data[JSON["Element"]].set_Value(JSON["Value"])
		elif(request.method == "GET"):
			pass

		return "JSON received"
		
	def __shutdown(self):
		if(self.__ThreadRunning):
			print("[INFO] Shutdown server...")
			self.Shutdown_Server()
			return "Server shutting down..."
		
	def __showIndex(self):
		return render_template("index.html")
		
	def Shutdown_Server(self):
		func = request.environ.get("werkzeug.server.shutdown")
		if func is None:
			raise RuntimeError("Not running with the Werkzeug Server")
		func()
		
		self.__ThreadRunning = False
		print("[INFO] Wait until Thread is terminating")
		self.__flaskThread.join()
	
	def Run(self):
		self.__ThreadRunning = True
		self.__flaskThread.start()
	
	def IsRunning(self):
		return self.__ThreadRunning
	
	def GetDataFromWeb(self, Key):
		if(not(Key in self.__WebKeys)):
			self.__WebKeys.append(Key)