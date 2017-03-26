from events import *

def collided(event):
    event.other().fire_event(Infected())
    print "chaser collided with " + str(event.other().id)

def init(self):
    self.entity().register_collided_handler(collided)
