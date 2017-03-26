from component import *
from physics import *

class PickedUpOilSlick:
    def __init__(self, entity):
        self.entity = entity

class OilSlickPowerup:
    def triggered(self, event):
        print "powerup #triggered"
        event.entity().fire_event(PickedUpOilSlick(self.entity))

    def start(self):
        self.entity.add_component(Trigger(self.physics, PxBox(PxVec3(3.0, 3.0, 3.0)), PxTransform(PxVec3(0.0, 0.0, 0.0))))
        self.entity.register_triggerenter_handler(self.triggered)
