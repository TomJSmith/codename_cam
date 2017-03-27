

singleton = dict()
singleton['theSetter'] = None

def getTheSetter():
	if singleton['theSetter'] is not None:
		return singleton['theSetter']
	else:
		singleton['theSetter'] = locationSetter()
		return singleton['theSetter']


class locationSetter():
	def __init__(self):
		# self.startingPositions = [(200.0, -220.0), (220.0, 225.0), (-227, 222.0), (-207, -228.0)]
		self.startingPositions = [(20.0, -22.0), (22.0, 22.0), (-22, 22.0), (-20, -22.0)]
		self.listPointer = 0

	def getPosition(self):
		position = self.startingPositions[self.listPointer]
		self.listPointer += 1
		return position

