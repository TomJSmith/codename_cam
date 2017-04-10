import camera
import physics
import vehicle
import types
import controller
import math
import runner
from start_game import *
from entity import *
from component import *
from oil_slick import *
from camera_control import *

class VehicleScript:
    def start(self):
        self.controller = controller.Controller(0)
        self.create_vehicle(self.entity)
        self.dead = False
        self.killed_by = None
        self.vehicle.set_active(False)

        self.entity.register_handler(GameStarted, self.start_game)
        self.entity.register_handler(Infected, self.infected)
        self.entity.register_handler(ChangeSurface, self.change_surface)

    def update(self, dt):
        if self.dead:
            pass

    def change_surface(self, event):
        self.vehicle.set_friction(event.friction)

    def start_game(self, event):
        self.vehicle.set_active(True)

    def infected(self, event):
        thisEvent = RunnerDestroyed()
        thisEvent.other = self.entity
        event.getother().fire_event(thisEvent)
        print("infected me")
        self.killed_by = event.getother()
        e = self.entity
        while e.get_parent():
            e = e.get_parent()

        self.entity.destroy()
        e = Entity.create(e).lock()
        self.create_vehicle(e, True)
        chaser = ScriptComponent("chaser", self.physics)
        e.add_component(chaser)

    def runnerdestroyed(self, event):
        thisEvent = RunnerDestroyed()
        thisEvent.other = event.getother()
        self.killed_by.fire_event(thisEvent)

    def create_vehicle(self, entity, chaser = False):
        c = vehicle.Configuration()

        dims = physics.PxVec3(3, 1, 5)

        c.position = physics.PxVec3(-20, 5, -90)
        c.chassis_dimensions = dims
        c.steer_angle = math.pi * .10
        c.torque = 10000
        c.wheel_radius = 0.5
        c.wheel_width = 0.4
        c.wheel_mass = 10
        c.omega = 100
        c.chassis_mass = 1000
        c.wheel_moi = 20
        c.chassis_moi = physics.PxVec3(c.chassis_mass, c.chassis_mass / 2, c.chassis_mass)
        c.chassis_offset = physics.PxVec3(0, -dims.y, 0)

        self.vehicle = vehicle.Vehicle(self.physics, self.controller, c)
        # cam = camera.Camera(self.physics)
        mesh = Mesh(ModelShader("chaser_texture.jpg" if chaser else "runner_texture.jpg"), "chaser_mesh.fbx" if chaser else "runner_mesh.fbx", physics.Vec3(0.1, 0.1, 0.6), physics.Vec3(1, 1, 1), 4)
        entity.add_component(self.vehicle)
        # entity.add_component(cam)
        entity.add_component(mesh)

        # cam = Entity.create(self.entity.get_parent()).lock()
        # cam.global_position = self.entity.global_position + physics.Vec3(0.0, 10.0, -30)
        # cam.add_component(CameraControl(self.entity), self.physics)

        if chaser:
            entity.register_handler(RunnerDestroyed, self.runnerdestroyed)
        else:
            r = runner.Runner()
            entity.add_component(r)
