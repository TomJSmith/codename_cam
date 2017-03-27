from physics import *
from entity import *
from controller import *
from oil_slick_powerup import *
from oil_slick import *
from ui import *

class PowerupManager:
    def pickup_oil_slick(self, event):
        self.powerup = OilSlick
        print "picked up oil slick"
        event.entity.destroy()

        if self.image and self.image.lock():
            self.image.lock().destroy()

        self.image = Entity.create(self.entity)
        self.image.lock().add_component(Image("oil_slick.png", Vec2(-1.0, 0.8), Vec2(0.2, 0.2), 1))

    def update(self, dt):
        self.ctrl.update()
        if self.ctrl.secondary and self.powerup:
            print "using powerup " + self.powerup.__name__
            entity = Entity.create(self.entity.get_parent()).lock()

            entity.transform().position = self.entity.transform().position
            entity.transform().position.y = 0.01

            entity.add_component(self.powerup(), self.physics)
            self.powerup = None
            if self.image and self.image.lock():
                self.image.lock().destroy()

    def start(self):
        self.ctrl = Controller(0)
        self.powerup = None
        self.image = None
        self.entity.register_handler(PickedUpOilSlick, self.pickup_oil_slick)
