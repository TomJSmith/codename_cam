import physics
import vehicle
import types
import controller

def init(self):
    _controller = controller.Controller()
    c = vehicle.Configuration()

    dims = physics.PxVec3(2, 2, 5)


    c.position = physics.PxVec3(0, 2, -20)
    c.torque = 2000
    c.chassis_dimensions = dims

    # straight from the physx examples
    # I don't know why these values are chosen or even really what the moment of inertia is
    # but they say this is cool so I guess it's cool

    c.chassis_moi = physics.PxVec3((dims.y ** 2 + dims.z ** 2) * c.chassis_mass / 12,
                                 (dims.x ** 2 + dims.z ** 2) * .8 * c.chassis_mass / 12,
                                 (dims.x ** 2 + dims.y ** 2) * c.chassis_mass / 12)


    c.chassis_offset = physics.PxVec3(0, -dims.y, .25)

    v = vehicle.Vehicle(self.physics(),_controller, c)
    self.entity().add_component(v)
