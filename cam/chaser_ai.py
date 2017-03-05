from physics import *
import controller
import aicontroller
import vehicle
import math

v = None

time = 0
_controller = None

targets = [Vec3(50, 0, -50), Vec3(10, 0, -10), Vec3(-10, 0, -10), Vec3(-10, 0, 10)]
target = 0

def init(self):

    global v
    global _controller

    _controller = aicontroller.aiController()
    config = vehicle.Configuration()

    dims = PxVec3(3, 1, 5)
    config.position = PxVec3(0, 2, -10)
    config.chassis_dimensions = dims
    config.steer_angle = math.pi * .05
    config.torque = 10000
    config.wheel_radius = 0.5
    config.wheel_width = 0.4
    config.wheel_mass = 10
    config.omega = 100
    config.chassis_mass = 1000
    config.wheel_moi = 20
    config.chassis_moi = PxVec3(config.chassis_mass, config.chassis_mass / 10, config.chassis_mass)
    config.chassis_offset = PxVec3(0, -dims.y, 0)

    # config.position = PxVec3(5, 2, 10)
    # config.torque = 2000
    # config.chassis_dimensions = dims

    # config.chassis_moi = PxVec3((dims.y ** 2 + dims.z ** 2) * config.chassis_mass / 12,
    #                             (dims.x ** 2 + dims.z ** 2) * .8 * config.chassis_mass / 12,
    #                             (dims.x ** 2 + dims.y ** 2) * config.chassis_mass / 12)

    # config.chassis_offset = PxVec3(0, -dims.y, .25)

	
    v = vehicle.Vehicle(self.physics(), _controller, config)
    self.entity().add_component(v)

def drive_at(self, target):
	global v
	global _controller

	direction = target - self.entity().transform().position
	forward = self.entity().transform().forward()
	right = self.entity().transform().right()
	test = self.entity().transform().position
	#print test.x
	#print test.y
	#print test.z
	direction.y = 0
	forward.y = 0

	#direction = Vec3(-direction.z, 0, direction.x)
	dot = Vec3.dot( direction, right)
	
	
	#dot = dot / (direction.length() + right.length())
	#print dot
	#print angle
	#print target.x 
	#print target.y 
	#print target.z

	#print forward.x 
	#print forward.y 
	#print forward.z
	_controller.setRight(0)
	_controller.setLeft(0)
	if dot > 0.1:
		_controller.setLeft(1)
		_controller.setBrake(1)
	else:
		_controller.setLeft(0)
		_controller.setBrake(0)

	if dot < -0.1:
		_controller.setRight(1)
		_controller.setBrake(1)
       # v.input().set_steer_left(1)
	else:
		_controller.setRight(0)
		_controller.setBrake(0)

       # v.input().set_steer_left(0)

	if direction.length() < 0.5:
		#print "We're there!"
		_controller.setBrake(1)
       # v.input().set_brake(True)
       # v.input().set_acceleration(False)
	else:
		#print "Not there yet, accelerating..."
		_controller.setBrake(0)
       # v.input().set_acceleration(True)
       # v.input().set_brake(False)
	
    # cosine = Vec3.dot(direction, forward) / (direction.length() + forward.length())
    # angle = math.acos(cosine)

def update(self, dt): 
    global time
    global target
    global v

    time += dt
    if time > 5:
       #target = (target + 1) % len(targets)
        time = 0
        target = 0
    drive_at(self, targets[target])
