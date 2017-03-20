import events

def collided(event):
    print "chaser collided"
    event.other().fire_event(events.Infected())

def init(self):
    self.entity().register_collided_handler(collided)
