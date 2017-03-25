from physics import *
import controller
import vehicle
import math
import types

v = None

time = 0

targets = [Vec3(10, 0, 10), Vec3(10, 0, -10), Vec3(-10, 0, -10), Vec3(-10, 0, 10)]
target = 0


def init(self):
	global v

	_controller = controller.aiController()
	config = vehicle.Configuration()

	dims = PxVec3(0, 2, -20)
	config.position = Px
	config.torque = 2000
	config.chassis_dimensisions = dims

	config.chassis_moi = physics.PxVec3((dims.y ** 2 + dims.z ** 2) * c.chassis_mass / 12,
										(dims.x ** 2 + dims.z ** 2) * .8 * c.chassis_mass / 12,
										(dims.x ** 2 + dims.y ** 2) * c.chassis_mass / 12)

	config.chassis_offset = physics.PxVec3(0, -dims.y, .25)
	v = vehicle.Vehicle(self.physics(), _controller, config)
	self.entity().add_component(v)
