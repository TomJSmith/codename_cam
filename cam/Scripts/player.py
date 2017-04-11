from component import *
from controller import *
from entity import *
from physics import *

import sys
import os
sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")

from vehicle_script import *
from powerup_manager import *
from camera_control import *
# from runner import Runner
import runner
from chaser import *

CAMERA_OFFSET = Vec3(0.0, 10.0, -30.0)

class Player:
    def __init__(self, manager, r = True):
        self.manager = manager
        self.controller = Controller(0)
        self.runner = r

    def start(self):
        print "starting player..."
        self.entity.add_component(VehicleScript(Vec3(-20.0, 2.0, -90.0), self.controller), self.physics)
        self.entity.add_component(PowerupManager(), self.physics)
        if self.runner:
            self.entity.add_component(runner.Runner(self.manager, self.controller, True), self.physics)
        else:
            self.entity.add_component(Chaser(), self.physics)

        cam = Entity.create(self.entity.get_parent()).lock()
        cam.global_position = self.entity.global_position + CAMERA_OFFSET
        cam.add_component(CameraControl(self.entity), self.physics)
