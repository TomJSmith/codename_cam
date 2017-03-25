import camera
import physics
import vehicle
import types
import controller
import math
import runner


def init(self):
	_controller = controller.Controller(0)
	c = vehicle.Configuration()

	dims = physics.PxVec3(3, 1, 5)

	c.position = physics.PxVec3(20, 2, -40)
	c.chassis_dimensions = dims
	c.steer_angle = math.pi * .05
	c.torque = 100000
	c.wheel_radius = 0.5
	c.wheel_width = 0.4
	c.wheel_mass = 10
	c.omega = 1000
	c.chassis_mass = 1000
	# c.suspension_damping = 1000

	# straight from the physx examples
	# I don't know why these values are chosen or even really what the moment of inertia is
	# but they say this is cool so I guess it's cool

	# c.chassis_moi = physics.PxVec3((dims.y ** 2 + dims.z ** 2) * c.chassis_mass / 12,
	#                              (dims.x ** 2 + dims.z ** 2) * .8 * c.chassis_mass / 12,
	#                              (dims.x ** 2 + dims.y ** 2) * c.chassis_mass / 12)

	c.wheel_moi = 20
	c.chassis_moi = physics.PxVec3(c.chassis_mass, c.chassis_mass / 2, c.chassis_mass)
	c.chassis_offset = physics.PxVec3(0, -dims.y, 0)
	# c.chassis_offset = physics.PxVec3(0, 0, 0)

	v = vehicle.Vehicle(self.physics(), _controller, c)
	cam = camera.Camera(v)
	r = runner.Runner()
	self.entity().add_component(v)
	self.entity().add_component(cam)
	self.entity().add_component(r)
