from controller import *

global ctrl

def init(self):
    ctrl = Controller(0)

    self.entity().add_component(ScriptComponent("vehicle", self.physics()))
