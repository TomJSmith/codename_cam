from physics import *
from component import *
from vehicle import *

class ChangeSurface:
    def __init__(self, friction):
        self.friction = friction

class OilSlick:
    def triggered(self, event):
        print "#triggered"
        event.entity().fire_event(ChangeSurface(0.0))

    def untriggered(self, event):
        print "#untriggered"
        event.entity().fire_event(ChangeSurface(1.0))

    def start(self):
        self.entity.add_component(Trigger(self.physics, PxSphere(10.0), PxTransform(PxVec3(0.0, 0.0, 0.0))))
        self.entity.add_component(Mesh(ModelShader("oil_slick_texture.jpg"), "oil_slick_mesh.fbx", Vec3(0.5, 0.5, 0.5), Vec3(1.0, 1.0, 1.0), 4))
        self.entity.register_triggerenter_handler(self.triggered)
        self.entity.register_triggerexit_handler(self.untriggered)
