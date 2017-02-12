from physics import *
import controller
import vehicle
import math

v = None

time = 0

targets = [Vec3(10, 0, 10), Vec3(10, 0, -10), Vec3(-10, 0, -10), Vec3(-10, 0, 10)]
target = 0

def init(self):
    global v

    _controller = controller.Controller()
    config = vehicle.Configuration()

    dims = PxVec3(2, 2, 5)

    config.position = PxVec3(5, 2, 10)
    config.torque = 2000
    config.chassis_dimensions = dims

    config.chassis_moi = PxVec3((dims.y ** 2 + dims.z ** 2) * config.chassis_mass / 12,
                                (dims.x ** 2 + dims.z ** 2) * .8 * config.chassis_mass / 12,
                                (dims.x ** 2 + dims.y ** 2) * config.chassis_mass / 12)

    config.chassis_offset = PxVec3(0, -dims.y, .25)

    v = vehicle.Vehicle(self.physics(), _controller, config)
    self.entity().add_component(v)

def drive_at(self, target):
    global v

    direction = target - self.entity().transform().global_position()
    forward = self.entity().transform().forward()

    direction.y = 0
    forward.y = 0

    direction = Vec3(-direction.z, 0, direction.x)
    dot = Vec3.dot(forward, direction)

    if dot > 0.01:
        v.input().set_steer_right(1)
    else:
        v.input().set_steer_right(0)

    if dot < -0.01:
        v.input().set_steer_left(1)
    else:
        v.input().set_steer_left(0)

    if direction.length() < 0.5:
        print "We're there!"
        v.input().set_brake(True)
        v.input().set_acceleration(False)
    else:
        print "Not there yet, accelerating..."
        v.input().set_acceleration(True)
        v.input().set_brake(False)

    # cosine = Vec3.dot(direction, forward) / (direction.length() + forward.length())
    # angle = math.acos(cosine)

def update(self, dt):
    global time
    global target
    global v

    time += dt
    if time > 5:
        target = (target + 1) % len(targets)
        time = 0

    drive_at(self, targets[target])
