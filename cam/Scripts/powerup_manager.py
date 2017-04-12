from physics import *
from entity import *
from controller import *
from oil_slick_powerup import *
from oil_slick import *
from ui import *

class PowerupManager:
    def __init__(self, controller, player = True):
        self.controller = controller
        self.player = player

    def pickup(self, event):
        self.powerup = event.powerup
        event.entity.destroy()

        if self.image and self.image.lock():
            self.image.lock().destroy()

        if self.player:
            self.image = Entity.create(self.entity)
            self.image.lock().add_component(Image(event.ui, Vec2(-1.0, 0.8), Vec2(0.2, 0.2), 1))

    def update(self, dt):
        self.controller.update()
        if self.controller.secondary and self.powerup:
            entity = Entity.create(self.entity.get_parent()).lock()

            entity.transform().position = self.entity.transform().position
            entity.transform().position.y = 0.01
            entity.transform().position -= self.entity.transform().forward() * 25.0

            entity.add_component(self.powerup, self.physics)
            self.powerup = None
            if self.image and self.image.lock():
                self.image.lock().destroy()

    def start(self):
        self.powerup = None
        self.image = None
        self.entity.register_handler(PickedUpPowerup, self.pickup)
