from physics import *
from component import *
from vehicle import *
from copy import *

class ChangeSurface:
    def __init__(self, friction):
        self.friction = friction

class ChangeSpeed:
    def __init__(self, mod):
        self.mod = mod

class PickedUpPowerup:
    def __init__(self, entity, powerup, ui):
        self.entity = entity
        self.powerup = powerup
        self.ui = ui

class Powerup:
    def __init__(self, start_effect, end_effect, time, tex, mesh):
        self.effect = Effect(start_effect, end_effect, time)
        self.tex = tex
        self.mesh = mesh

    def start(self):
        self.entity.add_component(Trigger(self.physics, PxSphere(10.0), PxTransform(PxVec3(0.0, 0.0, 0.0))))
        self.entity.add_component(Mesh(ModelShader(self.tex), self.mesh, Vec3(0.5, 0.5, 0.5), Vec3(1.0, 1.0, 1.0), 4))
        self.entity.register_triggerenter_handler(self.triggered)

    def triggered(self, event):
        event.entity().add_component(deepcopy(self.effect), self.physics)

class PowerupPickup:
    def __init__(self, tex, mesh, ui, powerup):
        self.powerup = powerup
        self.tex = tex
        self.mesh = mesh
        self.ui = ui

    def start(self):
        self.entity.add_component(Trigger(self.physics, PxBox(PxVec3(3.0, 3.0, 3.0)), PxTransform(PxVec3(0.0, 0.0, 0.0))))
        self.entity.add_component(Mesh(ModelShader(self.tex), self.mesh, Vec3(0.5, 0.5, 0.5), Vec3(1.0, 1.0, 1.0), 4))
        self.entity.register_triggerenter_handler(self.triggered)
        self.time = 0
        self.direction = Vec3(0.0, 1.5, 0.0)

    def update(self, dt):
        self.entity.transform().position += dt * self.direction
        self.time += dt
        if self.time > 3.0:
            self.direction = -self.direction
            self.time = 0.0

    def triggered(self, event):
        event.entity().fire_event(PickedUpPowerup(self.entity, self.powerup, self.ui))

class Effect:
    def __init__(self, start_event, end_event, duration):
        self.time = 0.0
        self.duration = duration
        self.done = False
        self.start_event = start_event
        self.end_event = end_event

    def start(self):
        self.entity.fire_event(self.start_event)

    def update(self, dt):
        self.time += dt
        if self.time > self.duration and not self.done:
            self.done = True
            self.entity.fire_event(self.end_event)
