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
        self.entity.add_component(Mesh(ModelShader("oil_pickup_texture.jpg"), "pickup_mesh.fbx", Vec3(0.5, 0.5, 0.5), Vec3(1.0, 1.0, 1.0), 4))
        self.entity.register_triggerenter_handler(self.triggered)
        self.time = 0
        self.direction = Vec3(0.0, 1.5, 0.0)

    def update(self, dt):
        self.entity.transform().position += dt * self.direction
        self.time += dt
        if self.time > 3.0:
            self.direction = -self.direction
            self.time = 0.0
