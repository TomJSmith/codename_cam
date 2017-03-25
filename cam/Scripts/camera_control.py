import physics
import math

initialized = False


def init(self):
	global initialized

	self.entity().transform().position = physics.Vec3(0, 5, -20)
	self.entity().transform().rotation = physics.Quaternion.axis_angle(math.pi, physics.Vec3(0, 1, 0))

	initialized = True


def update(self, dt):
	global initialized

	if not initialized:
		init(self)
