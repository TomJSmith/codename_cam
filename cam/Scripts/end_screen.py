from component import *
from controller import *
from physics import *
from ui import *
from entity import *
import sys
import os
import time

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
    def __init__(self, chaserManager):
        self.chaserManager = chaserManager
        self.gameStarted = False
        self.runnerTime = 0

    def start(self):
        self.startTime = time.time()
        self.control = Controller(0)
        self.background = None
        self.entity.register_handler(RunnerDestroyed, self.runnerInfected)

    def update(self, dt):
        if len(self.chaserManager.runner_e) > 0:
            self.gameStarted = True
        # print(len(self.chaserManager.runner_e))
        # print(self.background)
        # print(self.gameStarted)
        # print(len(self.chaserManager.runner_e) < 1 and self.background is None and self.gameStarted)
        if len(self.chaserManager.runner_e) < 1 and self.background is None and self.gameStarted:
            print('end game')
            self.background = Image("background.png", Vec2(-1.0, -1.0), Vec2(2.0, 2.0), 3)
            self.topLine = Text("All runners infected", "GROBOLD", Vec2(-0.5, 0.0), 8)
            self.bottomLine = Text("Game Over", "GROBOLD", Vec2(-0.3, -0.1), 8)
            self.timeScore = Text("You survived %0.1f seconds!" % (self.runnerTime,), "GROBOLD", Vec2(-0.7, -0.2), 8)
            self.entity.add_component(self.background)
            self.entity.add_component(self.topLine)
            self.entity.add_component(self.bottomLine)
            self.entity.add_component(self.timeScore)

        self.control.update()

    def runnerInfected(self, event):
        print('runner infected!!!!!!!!!!!')
        if (event.player):
            self.runnerTime = time.time() - self.startTime

