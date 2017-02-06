import entity
import physx
import vehicle
import scriptcomponent

intialized = False

def init(self):
    global initialized

    vehicle = vehicle.Vehicle()
    self.AddComponent(vehicle)
    initialized = true

def update(self, dt):
    if not initialized:
        init(self)
