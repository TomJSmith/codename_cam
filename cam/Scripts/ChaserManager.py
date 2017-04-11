import navmesh
import sys
import os
from physics import *
from events import *

sys.path.insert(0, os.getcwd() + "\\Scripts")
from a_star import *
class ChaserManager:
    def __init__(self):
        self.runner_e = []
        self.targetNodeXZ = []
        self.runnerPos = []
        self.map = self.create_nav_mesh()
        self.astar = A_star(self.map)
        self.frame_count = -1

    def start(self):
        self.entity.register_runnercreated_handler(self.runnercreated)
        self.entity.register_handler(RunnerDestroyed, self.runnerdestroyed)

    def runnerdestroyed(self, event):
        print("Removing runner")
        self.remove_runner(event.getother())
        event.getother().fire_event(Revived())

    def remove_runner(self, other):
        for i in range(len(self.runner_e)):
            if self.runner_e[i].id == other.id:
                self.runner_e.pop(i)
                self.targetNodeXZ.pop(i)
                self.runnerPos.pop(i)
                break

        print("Infected runner. Runners Left: " + str(len(self.runner_e)))

    def runnercreated(self, event):
        self.runner_e.append(event.get_runner())
        self.targetNodeXZ.append(self.astar.findCurrentNode(
            (event.get_runner().transform().global_position().x, event.get_runner().transform().global_position().z)))
        self.runnerPos.append(
            (event.get_runner().transform().global_position().x, event.get_runner().transform().global_position().z))


    def create_nav_mesh(self):
        _navmesh = navmesh.NavMesh('nav_mesh.fbx', Vec3(2.0, 2.0, 2.0))
        graph = _navmesh.getSimpleGraph()
        self.map = {}
        for node in graph:
            self.map[(node[0], node[1])] = Node(node[0], node[1], node[3])
        for node in graph:
            for neighbor in node[2]:
                self.map[(node[0], node[1])].add_neighbor(self.map[neighbor])
        return self.map

    def update(self, dt):
        for i in range(0, len(self.runnerPos)):
            self.runnerPos[i] = (self.runner_e[i].transform().global_position().x, self.runner_e[i].transform().global_position().z)

        if self.frame_count % 30 == 0 or self.frame_count == -1:
            for i in range(0, len(self.targetNodeXZ)):
                if not self.map[self.targetNodeXZ[i]].inNode(self.runnerPos[i]):
                    self.targetNodeXZ[i] = self.astar.findNextNode(self.map[self.targetNodeXZ[i]], self.runnerPos[i])

        self.frame_count += 1
        if self.frame_count > 60000:
            self.frame_count = 0