from component import *
from physics import *

class PickedUpOilSlick:
    def __init__(self, cmpnt):
        self.component = cmpnt

cmpnt = None

def triggered(event):
    global cmpnt

    print "powerup #triggered"
    event.entity().fire_event(PickedUpOilSlick(cmpnt))

def init(self):
    global cmpnt

    self.entity().add_component(Trigger(self.physics(), PxBox(PxVec3(3.0, 3.0, 3.0)), PxTransform(PxVec3(0.0, 0.0, 0.0))))
    self.entity().register_triggerenter_handler(triggered)

    cmpnt = self
