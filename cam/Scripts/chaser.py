from events import *

class Chaser:
    def collided(self, event):
        event.other().fire_event(Infected())
        print "chaser collided with " + str(event.other().id)

    def start(self):
        self.entity.register_collided_handler(self.collided)
