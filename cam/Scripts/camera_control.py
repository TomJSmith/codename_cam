from camera import *
from physics import *

class CameraControl:
    def __init__(self, target):
        self.target = target

    def start(self):
        self.offset = self.entity.global_position - self.target.global_position
        self.entity.add_component(Camera(self.physics))

    def update(self, dt):
        origin = self.target.global_position
        offset = self.target.global_rotation * self.offset
        direction = offset.normal()
        distance = offset.length()
        hit = self.physics.raycast(origin, direction, distance)

        if hit:
            position = Vec3(hit.position.x, hit.position.y, hit.position.z)
            offset = position - origin
            offset.y = self.offset.y

        target = Vec3(self.target.global_position.x, self.target.global_position.y + offset.y, self.target.global_position.z)

        self.entity.global_position = self.target.global_position + offset
        self.entity.global_rotation = Quaternion.look_at(self.entity.global_position, target, Vec3(0.0, 1.0, 0.0))
