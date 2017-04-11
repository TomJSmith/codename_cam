# from events import *
from component import *
from vehicle_script import *
from chaser import *
from player import *
from chaser_ai import *

class RunnerCreated:
    def __init__(self, entity):
        self.runner = entity

class RunnerDestroyed:
    def __init__(self, entity):
        self.runner = entity

class Runner:
    def __init__(self, manager, controller, player = False):
        self.manager = manager
        self.controller = controller
        self.player = player

    def start(self):
        self.entity.add_component(Mesh(ModelShader("runner_texture_green.jpg"), "runner_mesh.fbx", Vec3(1.0, 0.84, 0.0), Vec3(1, 1, 1), 4))
        self.entity.get_parent().broadcast_event(RunnerCreated(self.entity))

    def infected(self, event):
        # thisEvent = RunnerDestroyed()
        # thisEvent.other = self.entity
        event.getother().fire_event(RunnerDestroyed(self.entity))
        print("infected me")
        # self.killed_by = event.getother()
        e = self.entity
        while e.get_parent():
            e = e.get_parent()

        e = Entity.create(e).lock()
        e.add_component(VehicleScript(Vec3(-20.0, 2.0, -90.0), self.controller), self.physics)
        e.add_component(Chaser(), self.physics)

        if self.player:
            e.add_component(Player(), self.physics)
        else:
            e.add_component(ChaserAi(self.manager), self.physics)
