from controller import *
from oil_slick_powerup import *

class PowerupManager:
    def pickup_oil_slick(self, event):
        self.powerup = "oil_slick"
        print "picked up oil slick"
        event.entity.destroy()

    def update(self, dt):
        self.ctrl.update()
        if self.ctrl.secondary and self.powerup:
            print "using powerup " + self.powerup
            self.entity.add_component(ScriptComponent(self.powerup, self.physics))
            self.powerup = None

    def start(self):
        self.ctrl = Controller(0)
        self.powerup = None
        self.entity.register_handler(PickedUpOilSlick, self.pickup_oil_slick)
