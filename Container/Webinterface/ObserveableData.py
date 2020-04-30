class ObserveableData(object):
	def __init__(self, Value = None):
		self.__Observers = []
		
		if(Value == None):
			self.__Value = None
		else:
			self.__Value = Value
			
	def get_Value(self):
		return self.__Value
		
	def set_Value(self, Value):
		self.__Value = Value
		
		for Callback in self.__Observers:
			Callback(self, Value)
			
	def set_Callback(self, Callback):
		self.__Observers.append(Callback)