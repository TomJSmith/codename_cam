from physics import *
import events
import controller
import aicontroller
import vehicle
import math
v = None

time = 0
_controller = None
reachedGoal = True

targets = [Vec3(-0, 2, 40), Vec3(10, 0, -10), Vec3(-10, 0, -10), Vec3(-10, 0, 10)]
target = 0

def destroyed(event):
    pass
    global v
    v = None
    print "chaser ai collided"

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
    # self.entity().register_destroyed_handler(destroyed)

def drive_at(self, target):
	global v
	global _controller
	global reachedGoal

	direction = target - self.entity().transform().global_position() #transform().position
	forward = self.entity().transform().forward()
	right = self.entity().transform().right()
	test = self.entity().transform().position
	
	target.y = 0
	test.y = 0
	direction.y = 0
	right.y = 0
	forward.y = 0
	distanceToGoal = direction.length()

	print "CurrLoc X : ", test.x
	print "currLoc Y : ", test.y
	print "currLoc Z : ", test.z

	print "Target X : ", target.x
	print "Target Y : ", target.y
	print "Target : ", target.z

	#print "Direction X : ", direction.x
	#print "Direction Y : ", direction.y
	#print "Direction Z : ", direction.z

	direction.x = direction.x / direction.length()
	direction.y = direction.y / direction.length()
	direction.z = direction.z / direction.length()

	forward.x = forward.x / forward.length()
	forward.y = forward.y / forward.length()
	forward.z = forward.z / forward.length()
	
	right.x = right.x / right.length()
	right.y = right.y / right.length()
	right.z = right.z / right.length()
	#print "Direction X : ", direction.x
	#print "Direction Y : ", direction.y
	#print "Direction Z : ", direction.z

	#print "Target X : ", target.x
	#print "Target Y : ", target.y
	#print "Target : ", target.z

	#print "CurrLoc X : ", test.x
	#print "currLoc X : ", test.y
	#print "currLoc X : ", test.z
	#direction.y = 0
	#right.y = 0
	#forward.y = 0
	#direction = direction / direction.length()
	#forward = forward / forward.length()

	direction = Vec3(-direction.z, 0, direction.x)
	dot = Vec3.dot( right, direction)
	#print "Dot product : ", dot
	#print "Distance to goal : ", distanceToGoal
	#dot = dot / (direction.length() + right.length())
	#print dot
	#print angle
	#print target.x 
	#print target.y 
	#print target.z

	#print dot
	print distanceToGoal
	#print forward.x 
	#print forward.y 
	#print forward.z
	
	if reachedGoal:
		
		if distanceToGoal < 10.0:
			reachedGoal = False
			print "We're there!"
			_controller.setAccel(0)
			_controller.setBrake(1)
		else:
			_controller.setAccel(1)
			_controller.setRight(0)
			_controller.setLeft(0)


			if dot < -0.1:
				_controller.setLeft(1)
				#_controller.setBrake(1)
			else:
				_controller.setLeft(0)
				_controller.setBrake(0)

			if dot > 0.1:
				_controller.setRight(1)
				#_controller.setBrake(1)
			   # v.input().set_steer_left(1)
			else:
				_controller.setRight(0)
				_controller.setBrake(0)

		   # v.input().set_steer_left(0)

		
		   # v.input().set_brake(True)
		   # v.input().set_acceleration(False)
		#else:
			#print "Not there yet, accelerating..."
		#	_controller.setBrake(0)
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
