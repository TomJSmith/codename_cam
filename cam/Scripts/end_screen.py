from component import *
from controller import *
from physics import *
from ui import *
from entity import *
import sys
import os
import time

sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")

from player import *
from runner_ai import *
from camera_control import *
from chaser_ai import *
from main_menu import *
from vehicle_script import *
from ChaserManager import *
import runner

class GameEnded:
    pass

class EndScreen:
    def __init__(self, chaserManager, renderer):
        self.chaserManager = chaserManager
        self.gameStarted = False
        self.gameEnded = False
        self.runnerTime = 0
        self.renderer = renderer

    def start(self):
        self.startTime = time.time()
        self.control = Controller(0)
        self.background = None
        self.entity.register_handler(runner.RunnerDestroyed, self.runnerInfected)

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
            self.exitButton = Text("Press X to return to the menu", "GROBOLD", Vec2(-0.8, -0.3), 8)
            self.entity.add_component(self.background)
            self.entity.add_component(self.topLine)
            self.entity.add_component(self.bottomLine)
            self.entity.add_component(self.timeScore)
            self.entity.add_component(self.exitButton)
            self.gameEnded = True
            self.selected = self.control.select
            self.entity.get_parent().broadcast_event(GameEnded())

        self.control.update()

        if self.gameEnded and self.control.select:
            if not self.selected:
                p = self.entity.get_parent()
                p.destroy()
                menu = Entity.create(p).lock()
                menu.add_component(MainMenu(self.renderer), self.physics)

        if not self.control.select:
            self.selected = False

    def runnerInfected(self, event):
        print('runner infected!!!!!!!!!!!')
        if (event.player):
            self.runnerTime = time.time() - self.startTime

