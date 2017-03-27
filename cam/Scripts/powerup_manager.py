from physics import *
from entity import *
from controller import *
from oil_slick_powerup import *
from ui import *

class PowerupManager:
    def pickup_oil_slick(self, event):
        self.powerup = "oil_slick"
        print "picked up oil slick"
        event.entity.destroy()

        if self.image and self.image.lock():
            self.image.lock().destroy()

        self.image = Entity.create(self.entity)
        self.image.lock().add_component(Image("oil_slick.png", Vec2(-1.0, 0.8), Vec2(0.2, 0.2), 1))

    def update(self, dt):
        self.ctrl.update()
        if self.ctrl.secondary and self.powerup:
            print "using powerup " + self.powerup
            self.entity.add_component(ScriptComponent(self.powerup, self.physics))
            self.powerup = None
            if self.image and self.image.lock():
                self.image.lock().destroy()

    def start(self):
        self.ctrl = Controller(0)
        self.powerup = None
        self.image = None
        self.entity.register_handler(PickedUpOilSlick, self.pickup_oil_slick)
