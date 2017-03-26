foo = 0


def update(self, dt):
	global foo
	print("object " + str(self.id) + " updating: it's been " + str(dt) + " milliseconds and foo is " + str(foo))
	foo = dt
