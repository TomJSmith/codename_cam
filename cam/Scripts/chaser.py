import events


def collided(event):
	pass


def init(self):
	self.entity().register_collided_handler(collided)
