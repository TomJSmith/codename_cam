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
from a_star import *
from start_game import *

class RunnerAi:
    def __init__(self, position):
        self.startingPosition = position

    def runnercreated(self, event):
        self.runner_e = event.get_runner()
        self.targetNodeXZ = self.astar.findCurrentNode(
            (self.runner_e.transform().global_position().x, self.runner_e.transform().global_position().z))

    def infected(self, event):
        thisEvent = RunnerDestroyed()
        thisEvent.other = self.entity
        event.getother().fire_event(thisEvent)
        print("infected yo")
        self.entity.destroy()


    def start_game(self, event):
        self.started = True

    def start(self):
        self.dead = False
        self.started = False
        self.controller = aicontroller.aiController(5)
        config = vehicle.Configuration()
        self.map = self.create_nav_mesh()
        self.astar = A_star(self.map)
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
        self.runner_e = None

        dims = PxVec3(3, 1, 5)
        config.position = PxVec3(self.startingPosition.x, 2, self.startingPosition.z)
        config.rotation = PxQuat(0, 1, 0, 0)
        config.chassis_dimensions = dims
        config.steer_angle = math.pi * .12
        config.torque = 10000
        config.wheel_radius = 0.5
        config.wheel_width = 0.4
        config.wheel_mass = 10
        config.omega = 100
        config.chassis_mass = 1000
        config.wheel_moi = 20
        config.chassis_moi = PxVec3(config.chassis_mass, config.chassis_mass / 10, config.chassis_mass)
        config.chassis_offset = PxVec3(0, -dims.y, 0)

        self.vehicle = vehicle.Vehicle(self.physics, self.controller, config)
        r = runner.Runner()
        self.entity.add_component(self.vehicle)
        self.entity.add_component(r)
        self.entity.register_handler(Infected, self.infected)
        self.entity.register_handler(GameStarted, self.start_game)

        self.currentNodeXZ = self.astar.findCurrentNode(
            (self.entity.transform().global_position().x, self.entity.transform().global_position().z))


    def create_nav_mesh(self):
        _navmesh = navmesh.NavMesh('nav_mesh.fbx', Vec3(2.0, 2.0, 2.0))
        graph = _navmesh.getSimpleGraph()
        map = {}
        for node in graph:
            map[(node[0], node[1])] = Node(node[0], node[1], node[3])
        for node in graph:
            for neighbor in node[2]:
                map[(node[0], node[1])].add_neighbor(map[neighbor])
        return map

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
        # currTarget = Vec3(self.targetNodeXZ[0], 0, self.targetNodeXZ[1])
        if len(self.currentPath) > self.currentNodeIndex:
            currTarget = self.currentPath[self.currentNodeIndex]
        else:
            currTarget = Vec3(self.currentNodeXZ[0], 0, self.currentNodeXZ[1])
        self.currPosition = self.entity.transform().global_position()
        direction = currTarget - self.currPosition  # transform().position
        forward = self.entity.transform().forward()
        right = self.entity.transform().right()
        test = self.entity.transform().position

        # print "Current Target X: ", currTarget.x
        # print "Current Target Y: ", currTarget.y
        # print "Current Target Z: ", currTarget.z
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
        if dot < -1:
            dot = -1
        if dot > 1:
            dot = 1

        if self.stuck:
            self.controller.setDirection(-.8 * dot)
            self.controller.setBrake(0)
            self.controller.setReverse(1)
            self.controller.setAccel(1)

        else:
            #print("Goal : " + str((currTarget.x, currTarget.z)))
            #print("Chaser Position : " + str((self.entity().transform().global_position().x, self.entity().transform().global_position().z)))
            self.controller.setReverse(0)
            if self.astar.map[(currTarget.x, currTarget.z)].inNode((self.currPosition.x, self.currPosition.z)):
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
            if not self.map[self.currentNodeXZ].inNode(myPos):
                self.currentNodeXZ = self.astar.findNextNode(self.map[self.currentNodeXZ], myPos)

        if (self.frame_count + self.seed) % 4800 == 0 or self.reachedGoal:
            if self.reachedGoal:
                self.frame_count = self.seed + 1
            rndNodes = list(self.map.keys())
            rndNodes.remove(self.currentNodeXZ)
            randomTarget = random.choice(rndNodes)
            while self.map[randomTarget] in self.map[self.currentNodeXZ].neighbors:
                randomTarget = random.choice(rndNodes)
            self.currentPath = self.getNodePathToVec(
            self.astar.find_path(self.map[self.currentNodeXZ], self.map[randomTarget]))
            self.reachedGoal = False

        if (self.frame_count) % 360 == 0:
            if self.stuck_flag:
                self.stuck = True
                self.stuck_flag = False
            else:
                self.stuck = self.checkStuck()

        self.drive()
        self.frame_count += 1
        self.frame_count %= 60000
