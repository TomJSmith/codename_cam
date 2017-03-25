from physics import *
import events
import controller
import aicontroller
import vehicle
import math
import navmesh
import sys
import os

# there might be a better way to do this... needed a_star in path but dont know where to tell Visual Studio that
sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")
from a_star import *

v = None

time = 0
_controller = None
reachedGoal = False
backUp = False

currPos = None
prevPos = None

map = None
astar = None

closestPathNodes = None
closeNodeSelf = None
closeNodeTarget = None

runner_e = None
frame_count = -1
stuck = False
stuck_flag = False

def destroyed(event):
    pass
    global v
    v = None
    print "chaser ai collided"


def runnercreated(event):
    pass
    global runner_e
    global targetNodeXZ
    runner_e = event.get_runner()
    targetNodeXZ = astar.findCurrentNode((runner_e.transform().global_position().x, runner_e.transform().global_position().z))


def init(self):
    global v
    global _controller
    global map
    global astar
    global currentNodeXZ
    _controller = aicontroller.aiController()
    config = vehicle.Configuration()
    map = create_nav_mesh()
    astar = A_star(map)

    dims = PxVec3(3, 1, 5)
    config.position = PxVec3(-5, 5, 60)
    config.rotation = PxQuat(0, 1, 0, 0)
    config.chassis_dimensions = dims
    config.steer_angle = math.pi * .05
    config.torque = 10000
    config.wheel_radius = 0.5
    config.wheel_width = 0.4
    config.wheel_mass = 10
    config.omega = 100
    config.chassis_mass = 1000
    config.wheel_moi = 20
    config.chassis_moi = PxVec3(config.chassis_mass, config.chassis_mass / 10, config.chassis_mass)
    config.chassis_offset = PxVec3(0, -dims.y, 0)

    v = vehicle.Vehicle(self.physics(), _controller, config)
    self.entity().add_component(v)
    self.entity().register_runnercreated_handler(runnercreated)
    # self.entity().register_destroyed_handler(destroyed)

    currentNodeXZ = astar.findCurrentNode(
        (self.entity().transform().global_position().x, self.entity().transform().global_position().z))





def create_nav_mesh():
    _navmesh = navmesh.NavMesh('nav_mesh.fbx')
    graph = _navmesh.getSimpleGraph()
    map = {}
    for node in graph:
        map[(node[0], node[1])] = Node(node[0], node[1], node[3])
    for node in graph:
        for neighbor in node[2]:
            map[(node[0], node[1])].add_neighbor(map[neighbor])
    return map


def checkStuck(self):
    global prevPos
    currPos = self.entity().transform().global_position()

    if prevPos is not None:
        if (math.sqrt((currPos.x - prevPos.x)**2.0 + (currPos.z - prevPos.z)**2.0) < .1):
            prevPos = currPos
            return True
        else:
            prevPos = currPos
            return False

    prevPos = currPos
    return False

def getNodePathToVec(closestPathNodes):
    arrayVecs = []
    for node in closestPathNodes:
        vectorToAdd = Vec3(node.x, 0, node.z)
        arrayVecs.append(vectorToAdd)

    return arrayVecs


def getCurrentCoor(ent):
    return (ent.transform().global_position().x, ent.transform().global_position().z)


def drive(self):
    global _controller
    global reachedGoal
    global currentNodeIndex
    global currentPath
    global frame_count

    if not reachedGoal:
        currTarget = currentPath[currentNodeIndex]
        currPosition = self.entity().transform().global_position()
        direction = currTarget - currPosition  # transform().position
        forward = self.entity().transform().forward()
        right = self.entity().transform().right()
        test = self.entity().transform().position

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
        if frame_count%120 == 0:
            print("dot = " + str(dot))
            #print("Dx = " + str(direction.x))
            #print("Dx = " + str(direction.z))

        if stuck:
            if dot < -.5:
                _controller.setLeft(1)
                _controller.setRight(0)
            elif dot > .5:
                _controller.setRight(1)
                _controller.setLeft(0)
            else:
                _controller.setRight(0)

            _controller.setBrake(0)
            _controller.setAccel(1)
            _controller.setReverse(1)
        else:
            #print("Goal : " + str((currTarget.x, currTarget.z)))
            #print("Chaser Position : " + str((self.entity().transform().global_position().x, self.entity().transform().global_position().z)))
            _controller.setReverse(0)
            if astar.map[(currTarget.x, currTarget.z)].inNode((currPosition.x, currPosition.z)):
                print("Reached Node " + str(currentNodeIndex))
                #print "distance to goal : ", distanceToGoal
                #print "size of closestPathVectors : ", len(closestPathVectors)
                #print "size of targets : ", len(targets)
                #print "target index : ", target
                # reachedGoal = False
                # _controller.setBrake(1)
                # print "We're there!"
                _controller.setAccel(0)
                # _controller.setLeft(0)
                # _controller.setRight(0)
                currentNodeIndex += 1
                if currentNodeIndex >= len(currentPath):
                    currentNodeIndex == len(currentPath) - 1
                    print "Reached the Target"
                    reachedGoal = True
            else:
                _controller.setAccel(1)
                _controller.setBrake(0)

                if dot < -.37:
                    _controller.setLeft(1)
                    _controller.setRight(0)
                elif dot > .37:
                    _controller.setRight(1)
                    _controller.setLeft(0)
                else:
                    _controller.setRight(0)
                    _controller.setLeft(0)

    else:
        _controller.setAccel(0)
        _controller.setBrake(1)

def update(self, dt):
    global currentNodeXZ
    global targetNodeXZ
    global frame_count
    global currentNodeIndex
    global currentPath
    global reachedGoal
    global stuck
    global stuck_flag

    runnerPos = (runner_e.transform().global_position().x, runner_e.transform().global_position().z)
    chaserPos = (self.entity().transform().global_position().x, self.entity().transform().global_position().z)
    if frame_count%30 == 0 or frame_count == -1:
        if not map[currentNodeXZ].inNode(chaserPos):
            currentNodeXZ = astar.findNextNode(map[currentNodeXZ], chaserPos)

        if not map[targetNodeXZ].inNode(runnerPos):
            targetNodeXZ = astar.findNextNode(map[targetNodeXZ],runnerPos)

    if frame_count%120 == 0:
        if stuck:
            print("Stuck!")
        if stuck_flag:
            stuck_flag = False
        else:
            stuck = checkStuck(self)
            stuck_flag = stuck

    if frame_count % 600 == 0 or frame_count == -1:
        currentNodeIndex = 0
        currentPath = getNodePathToVec(astar.find_path(map[targetNodeXZ], map[currentNodeXZ]))
        reachedGoal = False
        print("Updated Path")
        #print("Path: ")
        #for path in currentPath:
        #    print(path.x, path.z)

    drive(self)

    frame_count += 1
    if frame_count >= 60000:
        frame_count = 0

    """if closestPathNodes is None or currNodeTarget >= len(closestPathNodes):
        # targetPosNode = (targets[target].x, targets[target].z)
        # selfPos = self.entity().transform().global_position()
        # selfPosNode = (selfPos.x, selfPos.z)
        # closeNodeTarget = astar.findClosestNode(map, targetPosNode)
        # closeNodeSelf = astar.findClosestNode(map, selfPosNode)
        targetPosNode = (runner_e.transform().global_position().x, runner_e.transform().global_position().z)
        selfPos = self.entity().transform().global_position()
        selfPosNode = (selfPos.x, selfPos.z)
        closeNodeTarget = astar.findNextNode(map, targetPosNode)
        closeNodeSelf = astar.findClosestNode(map, selfPosNode)

        drive_at(self, map[closeNodeTarget], map[closeNodeSelf])"""
