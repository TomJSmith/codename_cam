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
sys.path.insert(0, os.getcwd() + "\..\..\cam")
from a_star import *

v = None

time = 0
_controller = None
reachedGoal = True
backUp = False

currPos = None
prevPos = None

targets = [Vec3(0, 2, 20), Vec3(-70, 0, 20), Vec3(-70, 0, -60), Vec3(-20, 2, -60), Vec3(0, 0, -20)]
target = 0
currNodeTarget = 0
map = None
astar = None

def destroyed(event):
    pass
    global v
    v = None
    print "chaser ai collided"

def init(self):

    global v
    global _controller
    global map
    global astar
    _controller = aicontroller.aiController()
    config = vehicle.Configuration()

    dims = PxVec3(3, 1, 5)
    config.position = PxVec3(0, 2, -10)
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
    # self.entity().register_destroyed_handler(destroyed)

    # THIS CREATES
    map = create_nav_mesh()
    astar = A_star()

    # Usage: astar.find_path(map[(startX, startZ)], map[(targetX, targetZ)])
    # returns a list of nodes

def create_nav_mesh():
    _navmesh = navmesh.NavMesh('nav_mesh.fbx')
    graph = _navmesh.getSimpleGraph()
    map = {}
    for node in graph:
        map[(node[0], node[1])] = Node(node[0], node[1])
    for node in graph:
        for neighbor in node[2]:
            map[(node[0], node[1])].add_neighbor(map[neighbor])
    return map

def checkStuck(self):
	global prevPos
	global currPos
	currPos = self.entity().transform().global_position()
	if prevPos is not None:
		if Vec3.dot(currPos,prevPos) == 0:
			prevPos = curPos
			return True;
		
	else:
		prevPos = currPos
		return False;

def getNodePathToVec(closestPathNodes):
	arrayVecs = []
	for node in closestPathNodes:
		vectorToAdd = Vec3(node.x, 0, node.z)
		arrayVecs.append(vectorToAdd)
		
	return arrayVecs
	
def drive_at(self, closeNodeTarget, closeNodeSelf):
	global v
	global _controller
	global reachedGoal
	global targets
	global target
	global currNodeTarget

	closestPathNodes = astar.find_path(closeNodeSelf, closeNodeTarget)
	closestPathVectors = getNodePathToVec(closestPathNodes)
	
	
	direction = closestPathVectors[currNodeTarget] - self.entity().transform().global_position() #transform().position
	forward = self.entity().transform().forward()
	right = self.entity().transform().right()
	test = self.entity().transform().position
	
	targets[target].y = 0
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

	dot = Vec3.dot( right, direction)
	
	if reachedGoal:
		if checkStuck(self):
			_controller.setReverse(1)
		else:
			_controller.setReverse(0)
			if distanceToGoal < 10.0:
				#reachedGoal = False
				#_controller.setBrake(1)
				print "We're there!"
				_controller.setAccel(0)
				#_controller.setLeft(0)
				#_controller.setRight(0)
				currNodeTarget += 1
				currNodeTarget = currNodeTarget % len(closestPathVectors)
			else:
				_controller.setAccel(1)
				_controller.setRight(0)
				_controller.setLeft(0)


				if dot < -0.1:
					_controller.setLeft(1)
					_controller.setRight(0)
				else:
					_controller.setLeft(0)
					_controller.setBrake(0)

				if dot > 0.1:
					_controller.setRight(1)
					_controller.setLeft(0)
				else:
					_controller.setRight(0)
					_controller.setBrake(0)


def update(self, dt): 
	global time
	global target
	global v
	global map

	time += dt
	if time > 5:
		time = 0
	targetPosNode = (targets[target].x, targets[target].z)
	selfPos = self.entity().transform().global_position()
	selfPosNode = (selfPos.x, selfPos.z)
	closeNodeTarget = astar.findClosestNode(map, targetPosNode)
	closeNodeSelf = astar.findClosestNode(map, selfPosNode)
	drive_at(self, map[closeNodeTarget], map[closeNodeSelf])
