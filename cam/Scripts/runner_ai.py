from physics import *
from events import *
import controller
import aicontroller
import vehicle
import math
import navmesh
import sys
import os
import runner
import random
from entity import *
from component import *

# there might be a better way to do this... needed a_star in path but dont know where to tell Visual Studio that
sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")
import positionSetter
from chaser_ai import *
from a_star import *
from start_game import *
from runner import *
from chaser import *
from powerup_manager import *

class RunnerAi:
    def __init__(self, manager):
        self.manager = manager
        self.controller = aicontroller.aiController(5)

    def start_game(self, event):
        self.started = True

    def start(self):
        self.startingPosition = self.entity.global_position
        self.dead = False
        self.started = False
        self.count = 0
        self.reachedGoal = True
        self.backUp = False
        self.count = 0
        self.stuck = False
        self.stuck_flag = False
        self.currentPath = []
        self.seed = random.randint(0, 4800)
        self.frame_count = 0

        self.prevPos = None

        self.closestPathNodes = None
        self.closeNodeSelf = None
        self.closeNodeTarget = None
        self.currentNodeIndex = 0

        self.entity.add_component(Runner(self.manager, self.controller), self.physics)
        self.entity.add_component(PowerupManager(self.controller, False), self.physics)

        self.vehicle = self.entity.add_component(VehicleScript(self.startingPosition, self.controller), self.physics)
        self.entity.register_handler(GameStarted, self.start_game)

        self.currentNodeXZ = self.manager.astar.findCurrentNode(
            (self.entity.transform().global_position().x, self.entity.transform().global_position().z))


    def checkStuck(self):
        currPos = self.entity.transform().global_position()

        if self.prevPos is not None:
            if (math.sqrt((currPos.x - self.prevPos.x) ** 2.0 + (currPos.z - self.prevPos.z) ** 2.0) < 1):
                self.prevPos = currPos
                self.stuck_flag = True
                return True
            else:
                self.prevPos = currPos
                return False

        self.prevPos = currPos
        return False


    def getNodePathToVec(self, closestPathNodes):
        arrayVecs = []
        for node in closestPathNodes:
            vectorToAdd = Vec3(node.x, 0, node.z)
            arrayVecs.append(vectorToAdd)

        return arrayVecs

    def drive(self):
        if len(self.currentPath) > self.currentNodeIndex:
            currTarget = self.currentPath[self.currentNodeIndex]
        else:
            currTarget = Vec3(self.currentNodeXZ[0], 0, self.currentNodeXZ[1])
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
        dot = dot * .9
        if dot < -1:
            dot = -1
        if dot > 1:
            dot = 1

        if self.stuck:
            self.controller.setDirection(-.7 * dot)
            self.controller.setBrake(0)
            self.controller.setReverse(1)
            self.controller.setAccel(1)

        else:
            self.controller.setReverse(0)
            if self.manager.map[(currTarget.x, currTarget.z)].inNode((self.currPosition.x, self.currPosition.z)):
                if (self.currentNodeIndex+1) >= len(self.currentPath):
                    self.reachedGoal = True
                else:
                    self.currentNodeIndex += 1
            else:
                self.controller.setAccel(1)
                self.controller.setDirection(dot)

    def update(self, dt):
        if not self.started:
            return


        myPos = (self.entity.transform().global_position().x, self.entity.transform().global_position().z)
        if self.frame_count % 30 == 0 or self.frame_count == -1:
            if not self.manager.map[self.currentNodeXZ].inNode(myPos):
                self.currentNodeXZ = self.manager.astar.findNextNode(self.manager.map[self.currentNodeXZ], myPos)

        if (self.frame_count + self.seed) % 3600 == 0 or self.reachedGoal:
            if self.reachedGoal:
                self.frame_count = self.seed + 1
            rndNodes = list(self.manager.map.keys())
            for chaserXZ in self.manager.chaserXZ:
                if chaserXZ in rndNodes:
                    rndNodes.remove(chaserXZ)
                for neighbor in self.manager.map[chaserXZ].neighbors:
                    node = (neighbor.x, neighbor.z)
                    if node in rndNodes:
                        rndNodes.remove(node)
                    for neighbor2 in neighbor.neighbors:
                        node2 = (neighbor2.x, neighbor2.z)
                        if node2 in rndNodes:
                            rndNodes.remove(node2)
                        for neighbor3 in neighbor2.neighbors:
                            node3 = (neighbor3.x, neighbor3.z)
                            if node3 in rndNodes:
                                rndNodes.remove(node3)

            if self.currentNodeXZ in rndNodes:
                rndNodes.remove(self.currentNodeXZ)

            for neighbor in self.manager.map[self.currentNodeXZ].neighbors:
                node = (neighbor.x, neighbor.z)
                if node in rndNodes:
                    rndNodes.remove(node)

            randomTarget = random.choice(rndNodes)

            self.currentPath = self.getNodePathToVec(
            self.manager.astar.find_path(self.manager.map[self.currentNodeXZ], self.manager.map[randomTarget]))
            self.reachedGoal = False

        if (self.frame_count + 50) % 360 == 0:
            if self.stuck_flag:
                self.stuck = True
                self.stuck_flag = False
            else:
                self.stuck = self.checkStuck()

        self.drive()
        self.frame_count += 1
        self.frame_count %= 60000
