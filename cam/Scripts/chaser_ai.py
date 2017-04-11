from physics import *
from component import *
from events import *
import controller
import aicontroller
import vehicle
import math
import navmesh
import sys
import os

# there might be a better way to do this... needed a_star in path but dont know where to tell Visual Studio that
sys.path.insert(0, os.getcwd() + "\\Scripts")
from start_game import *
from a_star import *
from vehicle_script import *
from chaser import *
import runner

class ChaserAi:
    def __init__(self, manager, start=False):
        self.started = start
        self.manager = manager
        self.controller = aicontroller.aiController(4)

    def start_game(self, event):
        print "chaser started"
        self.started = True

    def runnerdestroyed(self, event):
        print "runner destroyed"
        self.targetRunner = self.closestRunner()

    def start(self):
        self.entity.add_component(Chaser(), self.physics)
        self.startingPosition = self.entity.global_position
        self.reachedGoal = False
        self.backUp = False

        self.prevPos = None

        self.map = None

        self.closeNodeSelf = None
        self.closeNodeTarget = None

        self.targetRunner = 0
        self.frame_count = -1
        self.stuck = False
        self.stuck_flag = False

        self.map = self.create_nav_mesh()
        self.astar = A_star(self.map)

        self.entity.add_component(VehicleScript(self.startingPosition, self.controller), self.physics)
        self.entity.register_handler(GameStarted, self.start_game)
        self.entity.register_handler(runner.RunnerDestroyed, self.runnerdestroyed)
        self.currentNodeXZ = self.astar.findCurrentNode(
            (self.entity.transform().global_position().x, self.entity.transform().global_position().z))
        self.manager.chaserXZ.append(self.currentNodeXZ)
        self.manager.chaserPos.append((self.entity.transform().global_position().x, self.entity.transform().global_position().z))
        self.manager.chaser_e.append(self.entity)

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

    def checkStuck(self):
        currPos = self.entity.transform().global_position()

        if self.prevPos is not None:
            if (math.sqrt((currPos.x - self.prevPos.x) ** 2.0 + (currPos.z - self.prevPos.z) ** 2.0) < .3):
                self.prevPos = currPos
                self.stuck_flag = True
                return True
            else:
                self.prevPos = currPos
                return False

        self.prevPos = currPos
        return False

    def closestRunner(self):
        currPos = self.entity.transform().global_position()
        minDistance = 9999999
        closest = 0
        for i in range(0, len(self.manager.runnerPos)):
            currDistance = math.sqrt((currPos.x - self.manager.runnerPos[i][0]) ** 2.0 + (currPos.z - self.manager.runnerPos[i][1]) ** 2.0)
            if currDistance < minDistance:
                minDistance = currDistance
                closest = i
        return closest


    def getNodePathToVec(self, closestPathNodes):
        arrayVecs = []
        for node in closestPathNodes:
            vectorToAdd = Vec3(node.x, 0, node.z)
            arrayVecs.append(vectorToAdd)

        return arrayVecs

    def drive(self):
        if not self.reachedGoal:
            currTarget = self.currentPath[self.currentNodeIndex]
        else:
            currTarget = self.manager.runner_e[self.targetRunner].transform().global_position()
        self.currPosition = self.entity.transform().global_position()
        direction = currTarget - self.currPosition  # transform().position
        forward = self.entity.transform().forward()
        right = self.entity.transform().right()
        test = self.entity.transform().position

        test.y = 0
        direction.y = 0
        right.y = 0
        forward.y = 0
        distanceToGoal = direction.length()

        if direction.length() == 0:
            return

        direction.x = direction.x / direction.length()
        direction.y = direction.y / direction.length()
        direction.z = direction.z / direction.length()

        forward.x = forward.x / forward.length()
        forward.y = forward.y / forward.length()
        forward.z = forward.z / forward.length()

        right.x = right.x / right.length()
        right.y = right.y / right.length()
        right.z = right.z / right.length()

        dot = Vec3.dot(right, direction)
        if dot < -1:
            dot = -1
        if dot > 1:
            dot = 1

        if self.stuck:
            self.controller.setDirection(-.80 * dot)
            self.controller.setBrake(0)
            self.controller.setReverse(1)
            self.controller.setAccel(1)
        else:
            self.controller.setReverse(0)
            if self.reachedGoal or not self.astar.map[(currTarget.x, currTarget.z)].inNode((self.currPosition.x, self.currPosition.z)):
                self.controller.setAccel(1)
                self.controller.setBrake(0)
                self.controller.setDirection(dot)
            else:
                self.controller.setDirection(0)
                self.controller.setBrake(1)
                self.controller.setAccel(0)

                self.currentNodeIndex += 1
                if self.currentNodeIndex >= len(self.currentPath):
                    self.currentNodeIndex = len(self.currentPath) - 1
                    self.reachedGoal = True

    def update(self, dt):
        if not self.started:
            return

        if len(self.manager.runner_e) > 0:
            chaserPos = (self.entity.transform().global_position().x, self.entity.transform().global_position().z)
            if self.frame_count % 30 == 0 or self.frame_count == -1:
                if not self.map[self.currentNodeXZ].inNode(chaserPos):
                    self.currentNodeXZ = self.astar.findNextNode(self.map[self.currentNodeXZ], chaserPos)

            if (self.frame_count + 50) % 360 == 0:
                if self.stuck_flag:
                    self.stuck = True
                    self.stuck_flag = False
                else:
                    self.stuck = self.checkStuck()

            if self.frame_count % 1200 == 0 or self.frame_count == -1:
                self.targetRunner = self.closestRunner()
                self.currentNodeIndex = 0
                self.currentPath = self.getNodePathToVec(self.astar.find_path(self.map[self.manager.runnerXZ[self.targetRunner]], self.map[self.currentNodeXZ]))
                self.reachedGoal = False

            self.drive()

            self.frame_count += 1
            if self.frame_count > 60000:
                self.frame_count = 0
