from controller import *
from oil_slick_powerup import *

powerup = None
ctrl = None

def pickup_oil_slick(event):
    global powerup
    powerup = "oil_slick"
    print "picked up oil slick"
    event.component.entity().destroy()

def update(self, dt):
    global powerup
    global ctrl

    print "updating powerup manager, ctrl.select " + str(ctrl.select)
    ctrl.update()
    if ctrl.secondary and powerup:
        print "using powerup " + powerup
        self.entity().add_component(ScriptComponent(powerup, self.physics()))
        powerup = None

def init(self):
    global ctrl
    ctrl = Controller(0)
    self.entity().register_handler(PickedUpOilSlick, pickup_oil_slick)
