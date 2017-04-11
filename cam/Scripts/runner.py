import events

e = None


def init(self):
	global e

	e = self.entity()
	# e.register_infected_handler(infected)
	#e.register_destroyed_handler(destroyed)
