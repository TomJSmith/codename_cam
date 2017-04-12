from events import *
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

class UpdateRunner:
    def __init__(self, entity):
        self.runner = entity

class Runner:
    def __init__(self, manager, controller, player = False):
        self.manager = manager
        self.controller = controller
        self.player = player
        self.needs_revive = False

    def start(self):
        self.entity.add_component(Mesh(ModelShader("runner_texture_green.jpg"), "runner_mesh.fbx", Vec3(1.0, 0.84, 0.0), Vec3(1, 1, 1), 4))
        self.entity.register_infected_handler(self.infected)
        self.entity.register_handler(Infected, self.infected)
        self.entity.register_handler(Revived, self.revived)
        # self.entity.register_infected_handler(self.infected)
        self.entity.get_parent().broadcast_event(RunnerCreated(self.entity))

    def infected(self, event):
        e = self.entity
        while e.get_parent():
            e = e.get_parent()

        e.broadcast_event(RunnerDestroyed(self.entity))

    def update(self, dt):
        if self.needs_revive:
            self.needs_revive = False
            print("infected me")
            e = self.entity
            while e.get_parent():
                e = e.get_parent()

            e = Entity.create(e).lock()
            e.global_position = Vec3(self.entity.global_position.x, 15.0, self.entity.global_position.z)

            if self.player:
                e.add_component(Player(self.manager, False), self.physics)
            else:
                e.global_position += Vec3(0.0, 2.0, 0.0)
                e.add_component(ChaserAi(self.manager), self.physics)

            e.fire_event(GameStarted())

            self.entity.destroy()

    def revived(self, event):
        self.needs_revive = True
        # self.needs_revive = True
        # e = self.entity
        # while e.get_parent():
        #     e = e.get_parent()

        # chaser = Entity.create(e).lock()
        # chaser.add_component(VehicleScript(Vec3(-20.0, 2.0, -90.0), self.controller), self.physics)
        # if self.player:
        #     chaser.add_component(Player(self.manager, False), self.physics)
        # else:
        #     chaser.add_component(ChaserAi(self.manager), self.physics)

        # self.entity.destroy()
