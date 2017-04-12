from component import *
from controller import *
from physics import *
from ui import *
from entity import *
import sys
import os

sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")

from chaser import *
from runner import *
from player import *
from runner_ai import *
from camera_control import *
from chaser_ai import *
from vehicle_script import *
from ChaserManager import *


class EndScreen:
    def start(self):
        self.control = Controller(0)
        self.background = Image("background.png", Vec2(-1.0, -1.0), Vec2(2.0, 2.0), 3)
        self.gameOver = Text("Game Over", "GROBOLD", Vec2(-0.3, 0.0), 12)
        self.entity.add_component(self.background)
        self.entity.add_component(self.gameOver)

    def update(self, dt):
        self.control.update()
