from physics import *
import events
import controller
import aicontroller
import vehicle
import math
import navmesh
import sys
import os
import runner

# there might be a better way to do this... needed a_star in path but dont know where to tell Visual Studio that
sys.path.insert(0, os.getcwd() + "\..\..\cam\Scripts")
from a_star import *

# self.controller = None
# self.reachedGoal = False
# self.backUp = False

# self.map = None
# self.astar = None


# self.runner_e = None
# self.frame_count = -1
# self.stuck = Falseself.count = 0


class RunnerAi:
    def runnercreated(self, event):
	# global self.runner_e
	# global self.targetNodeXZ
	self.runner_e = event.get_runner()
	self.targetNodeXZ = self.astar.findCurrentNode(
		(self.runner_e.transform().global_position().x, self.runner_e.transform().global_position().z))

    def start(self):
	# global v
	# global self.controller
	# global self.map
	# global self.astar
	# global self.currentNodeXZ
	self.controller = aicontroller.aiController(5)
	config = vehicle.Configuration()
	self.map = self.create_nav_mesh()
	self.astar = A_star(self.map)
        self.count = 0
        self.reachedGoal = False
        self.backUp = False
        self.frame_count = -1
        self.count = 0
        self.stuck = False

        self.currPos = None
        self.prevPos = None

        self.closestPathNodes = None
        self.closeNodeSelf = None
        self.closeNodeTarget = None
        self.runner_e = None

	dims = PxVec3(3, 1, 5)
	config.position = PxVec3(-15, 2, 60)
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

	self.vehicle = vehicle.Vehicle(self.physics, self.controller, config)
	r = runner.Runner()
	self.entity.add_component(self.vehicle)
	self.entity.add_component(r)
	# self.entity().register_destroyed_handler(destroyed)

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
	# global self.prevPos
	self.currPos = self.entity.transform().global_position()

	if self.prevPos is not None:
	    if (math.sqrt((self.currPos.x - self.prevPos.x) ** 2.0 + (self.currPos.z - self.prevPos.z) ** 2.0) < 1):
		self.prevPos = self.currPos
                return True
	    else:
		self.prevPos = self.currPos
		return False

	self.prevPos = self.currPos
	return False


    def getNodePathToVec(self, closestPathNodes):
	arrayVecs = []
	for node in closestPathNodes:
		vectorToAdd = Vec3(node.x, 0, node.z)
		arrayVecs.append(vectorToAdd)

	return arrayVecs

    def getCurrentCoor(self, ent):
	return (ent.transform().global_position().x, ent.transform().global_position().z)

    def drive(self):
	# global self.controller
	# global self.reachedGoal
	# global currentNodeIndex
	# global currentPath
	# global self.currentNodeXZ
	# global self.targetNodeXZ
	# globalself.count
	# self.controller.setAccel(1)
	# self.controller.setRight(1)
	
	currTarget = Vec3(self.targetNodeXZ[0], 0, self.targetNodeXZ[1])

	
	"""
	self.controller.setAccel(1)
	self.controller.setRight(0)
	self.controller.setLeft(0)
	dot = 0.0
	if dot < -.3:
		self.controller.setLeft(1)
		self.controller.setRight(0)
	else:
		self.controller.setLeft(0)
		self.controller.setBrake(0)

	if dot > .3:
		self.controller.setRight(1)
		self.controller.setLeft(0)
	else:
		self.controller.setRight(0)
		self.controller.setBrake(0)

	"""
	if not self.reachedGoal:
		#currTarget = currentPath[currentNodeIndex]
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

		if self.stuck:
			self.controller.setBrake(0)
			self.controller.setRight(0)
			self.controller.setLeft(0)
			self.controller.setReverse(1)
			self.controller.setAccel(1)

		else:
			#print("Goal : " + str((currTarget.x, currTarget.z)))
			#print("Chaser Position : " + str((self.entity().transform().global_position().x, self.entity().transform().global_position().z)))
			self.controller.setReverse(0)
			if self.astar.map[(currTarget.x, currTarget.z)].inNode((self.currPosition.x, self.currPosition.z)):
				# print("Reached Node " + str(currentNodeIndex))
				#print "distance to goal : ", distanceToGoal
				#print "size of closestPathVectors : ", len(closestPathVectors)
				#print "size of targets : ", len(targets)
				#print "target index : ", target
				# self.reachedGoal = False
				# self.controller.setBrake(1)
				# print "We're there!"
			  #  self.controller.setAccel(0)
				# self.controller.setLeft(0)
				# self.controller.setRight(0)
				"""currentNodeIndex += 1
				if currentNodeIndex >= len(currentPath):
					currentNodeIndex == len(currentPath) - 1
				   # currTarget.x = self.runner_e.transform().global_position().x
				   # currTarget.z = self.runner_e.transform().global_position().z"""
				print "Reached the Target"
			        self.count +=1

				#self.reachedGoal = True
			else:
				self.controller.setAccel(1)
				self.controller.setRight(0)
				self.controller.setLeft(0)

				if dot < -.3:
					self.controller.setLeft(1)
					self.controller.setRight(0)
				else:
					self.controller.setLeft(0)
					self.controller.setBrake(0)

				if dot > .3:
					self.controller.setRight(1)
					self.controller.setLeft(0)
				else:
					self.controller.setRight(0)
					self.controller.setBrake(0)
	
	# else:
	#   self.controller.setAccel(0)
	#     self.controller.setBrake(1)


    def update(self, dt):
	# global self.currentNodeXZ
	# global self.targetNodeXZ
	# global self.frame_count
	# global currentNodeIndex
	# global currentPath
	# global self.reachedGoal
	# global self.stuck
	# global self.runner_e
	# globalself.count
	# global self.map
	
	
	currentNode = self.map[self.currentNodeXZ]
        self.count %= len(currentNode.neighbors)
	#ifself.count % len(currentNode.neighbors) == 0:
		# self.count = 0
	targetNode = currentNode.neighbors[self.count]
	myPos = (self.entity.transform().global_position().x, self.entity.transform().global_position().z) 
	self.targetNodeXZ = self.astar.findNextNode(currentNode, (targetNode.x, targetNode.z))
	
	self.count += 1
	
	"""
	if self.runner_e is not None:
		runnerPos = (self.runner_e.transform().global_position().x, self.runner_e.transform().global_position().z)

		chaserPos = (self.entity().transform().global_position().x, self.entity().transform().global_position().z)
		if self.frame_count%30 == 0 or self.frame_count == -1:
			if not self.map[self.currentNodeXZ].inNode(chaserPos):
				self.currentNodeXZ = self.astar.findNextNode(self.map[self.currentNodeXZ], chaserPos)

			if not self.map[self.targetNodeXZ].inNode(runnerPos):
				self.targetNodeXZ = self.astar.findNextNode(self.map[self.targetNodeXZ],runnerPos)"""
	if self.frame_count%60 == 0:
		self.stuck = self.checkStuck()
	"""if self.frame_count > 360 or self.frame_count == -1:
		self.frame_count = -1
		currentNodeIndex = 0
		currentPath = getNodePathToVec(self.astar.find_path(self.map[self.targetNodeXZ], self.map[self.currentNodeXZ]))
		self.reachedGoal = False



		# print("Updated Path")
	"""
	self.drive()

	self.frame_count += 1
	self.frame_count %= 360
	"""if self.closestPathNodes is None or currNodeTarget >= len(self.closestPathNodes):
		# targetPosNode = (targets[target].x, targets[target].z)
		# selfPos = self.entity().transform().global_position()
		# selfPosNode = (selfPos.x, selfPos.z)
		# self.closeNodeTarget = self.astar.findClosestNode(self.map, targetPosNode)
		# self.closeNodeSelf = self.astar.findClosestNode(self.map, selfPosNode)
		targetPosNode = (self.runner_e.transform().global_position().x, self.runner_e.transform().global_position().z)
		selfPos = self.entity().transform().global_position()
		selfPosNode = (selfPos.x, selfPos.z)
		self.closeNodeTarget = self.astar.findNextNode(self.map, targetPosNode)
		self.closeNodeSelf = self.astar.findClosestNode(self.map, selfPosNode)

		drive_at(self, self.map[self.closeNodeTarget], self.map[self.closeNodeSelf])"""
