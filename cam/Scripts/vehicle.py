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

v = None
dead = False

def change_surface(event):
    v.set_friction(event.friction)

def start_game(event):
    global v
    v.set_active(True)

def infected(event):
    global dead
    dead = True

def create_vehicle(e, phys):
    global v

    _controller = controller.Controller(0)
    c = vehicle.Configuration()

    dims = physics.PxVec3(3, 1, 5)

    c.position = physics.PxVec3(0, 2, 30)
    c.chassis_dimensions = dims
    c.steer_angle = math.pi * .05
    c.torque = 100000
    c.wheel_radius = 0.5
    c.wheel_width = 0.4
    c.wheel_mass = 10
    c.omega = 1000
    c.chassis_mass = 1000
    c.wheel_moi = 20
    c.chassis_moi = physics.PxVec3(c.chassis_mass, c.chassis_mass / 2, c.chassis_mass)
    c.chassis_offset = physics.PxVec3(0, -dims.y, 0)

    v = vehicle.Vehicle(phys, _controller, c)
    cam = camera.Camera(v)
    mesh = Mesh(ModelShader("runner_texture.jpg"), "runner_mesh.fbx", physics.Vec3(0.1, 0.1, 0.6), physics.Vec3(4.427, 2.426, 12.935), 4)
    # v.set_active(False)
    # r = runner.Runner()
    e.add_component(v)
    e.add_component(cam)
    e.add_component(mesh)
    # self.entity().add_component(r)

def init(self):
    global v

    # self.entity().register_start_game_handler(start_game)
    create_vehicle(self.entity(), self.physics())
    # self.entity().add_component(cam)
    v.set_active(False)
    self.entity().add_component(runner.Runner())
    self.entity().register_handler(StartGame, start_game)
    self.entity().register_handler(Infected, infected)
    self.entity().register_handler(ChangeSurface, change_surface)

def update(self, dt):
    global dead
    global v

    if dead:
        e = self.entity()
        while e.get_parent():
            e = e.get_parent()

        e = Entity.create(e).lock()
        create_vehicle(e, self.physics())
        chaser = ScriptComponent("chaser", self.physics())
        e.add_component(chaser)
        self.entity().destroy()
