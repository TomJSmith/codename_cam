from events import *

class Chaser:
    def collided(self, event):
        thisEvent = Infected()
        thisEvent.other = self.entity
        event.getother().fire_event(thisEvent)
        print "chaser collided with " + str(event.getother().id)
	
    def start(self):
        self.entity.register_collided_handler(self.collided)
