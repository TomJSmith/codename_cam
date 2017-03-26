from physics import *
from component import *
from vehicle import *

class ChangeSurface:
    def __init__(self, friction):
        self.friction = friction

def triggered(event):
    print "#triggered"
    event.entity().fire_event(ChangeSurface(0.0))

def untriggered(event):
    print "#untriggered"
    event.entity().fire_event(ChangeSurface(1.0))

def init(self):
    self.entity().add_component(Trigger(self.physics(), PxSphere(10.0), PxTransform(PxVec3(0.0, 0.0, 0.0))))
    self.entity().register_triggerenter_handler(triggered)
    self.entity().register_triggerexit_handler(untriggered)
