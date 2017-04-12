import camera
import physics
import vehicle
import types
import controller
import math
# import runner
from start_game import *
from entity import *
from component import *
from oil_slick import *
from camera_control import *

class VehicleScript:
    def __init__(self, position, controller, chaser = False):
        self.startingPosition = position
        self.controller = controller
        self.chaser = chaser

    def start(self):
        self.create_vehicle(self.entity, self.startingPosition, self.chaser)
        self.dead = False
        self.killed_by = None
        self.vehicle.lock().set_active(False)

        self.entity.register_handler(GameStarted, self.start_game)
        self.entity.register_handler(ChangeSurface, self.change_surface)
        self.entity.register_handler(ChangeSpeed, self.change_speed)

    def change_surface(self, event):
        v = self.vehicle.lock()
        if v:
            v.set_friction(event.friction)

    def change_speed(self, event):
        v = self.vehicle.lock()
        if v:
            v.omega *= event.mod
            v.torque *= event.mod

    def start_game(self, event):
        v = self.vehicle.lock()
        if v:
            v.set_active(True)

    def create_vehicle(self, entity, position, chaser = False):
        c = vehicle.Configuration()

        dims = physics.PxVec3(6, 2, 12)

        c.position = physics.PxVec3(position.x, position.y, position.z)
        c.chassis_dimensions = dims
        c.wheel_radius = 0.5
        c.wheel_width = 0.4
        c.wheel_mass = 10

        c.chassis_mass = 1000
        c.chassis_moi = physics.PxVec3(c.chassis_mass, c.chassis_mass / 2, c.chassis_mass)
        c.chassis_offset = physics.PxVec3(0, -dims.y, 0)
        if chaser:
            c.wheel_moi = 20
            c.torque = 4000
            c.max_speed = 1.0
            c.steer_angle = math.pi * .16
            c.max_omega = 1900
        else:
            c.wheel_moi = 20
            c.torque = 3000
            c.max_speed = 1.0
            c.steer_angle = math.pi * .15
            c.max_omega = 1600

        self.vehicle = entity.add_component(vehicle.Vehicle(self.physics, self.controller, c))
