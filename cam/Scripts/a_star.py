import sys
import math


class Node():
    def __init__(self, x, z, vertices):
        """
        Node Object. Represents a point on a graph
        :param x: X coordinate of node
        :param z: Z coordinate of node
        :param neighbors: List of Node objects reprsenting a nodes neighbors
        """
        self.x = x
        self.z = z
        self.vertices = vertices
        self.neighbors = []
        self.weight = 0

    def __hash__(self):
        return hash((self.x, self.z))

    def __eq__(self, other):
        return (self.x, self.z) == (other.x, other.z)

    def distance(self, node):
        """
        Calculate distance between this node and the node parameter. Both nodes must be neighbors.
        :param node: node to calculate distance of
        :return: Float distance
        """
        if node in self.neighbors:
            return 10 * math.sqrt((abs(self.x - node.x) ** 2) + (abs(self.z - node.z) ** 2))
        else:
            raise Exception("Node (%l, %l) is not a neighbor of node (%l, %l)" % (node.x, node.z, self.x, self.z))

    def add_neighbor(self, nodes):
        """
        Add either a single node or a list of nodes to be neighbors of a node
        :param nodes: Either a single Node object or a list of Nodes.
        :return: void
        """
        if isinstance(nodes, list):
            self.neighbors.extend(nodes)
        elif isinstance(nodes, Node):
            self.neighbors.append(nodes)
        else:
            raise Exception("Failure to add neighbor for node (%i, %i)" % (self.x, self.y))

    def inNode(self, coordinates):
        result = False
        j = len(self.vertices) -  1
        for i in range(0, len(self.vertices)):
            ix = self.vertices[i][0]
            iz = self.vertices[i][1]
            jx = self.vertices[j][0]
            jz = self.vertices[j][1]
            if ((iz>coordinates[1]) != (jz>coordinates[1]) and (coordinates[0] < (jx - ix) * (coordinates[1] - iz) / (jz - iz) + ix)):
                result = not result
            j = i
        return result




class A_star():
    """
    A * class with functions to calculate shortest path
    """
    def __init__(self, map):
        self.map = map

    def calculate_h(self, x, z, targetX, targetZ):
        """
        Calculate Heuristic(estimated) value between to points using manhatten method
        :param x:
        :param z:
        :param targetX:
        :param targetZ:
        :return:
        """
        H_score = 10 * (abs(x - targetX) + abs(z - targetZ))
        return H_score

    def findCurrentNode(self, position):
        """

        :param position:
        :return:
        """
        for node in self.map:
            if self.map[node].inNode(position):
                return node

        raise Exception("Node not in map?")

    def findNextNode(self, previousNode, currentPosition):
        """

        :param previousNode:
        :param currentPosition:
        :return:
        """
        for node in previousNode.neighbors:
            if(node.inNode(currentPosition)):
                return (node.x, node.z)

        return self.findCurrentNode(currentPosition)

    def findClosestNode(self, map, start):

        xCoor = 0
        zCoor = 0
        distToTest = 0
        closestDist = 10000

        closeNode = None
        for node in map:
            xCoor = node[0] - start[0]
            zCoor = node[1] - start[1]
            distToTest = math.sqrt((xCoor * xCoor) + (zCoor * zCoor))
            if (distToTest < closestDist):
                closeNode = node
                closestDist = distToTest

        return closeNode

    def find_path(self, start, target):
        """
        Find shortest path between 2 points.
        :param start: starting node
        :param target: target node
        :return: a list of nodes which represents the shortest path
        """
        open_set = [start]
        closed_set = []
        came_from = {}
        G_score = {}
        G_score[start] = 0
        H_score = {}
        H_score[start] = self.calculate_h(start.x, start.z, target.x, target.z)

        while open is not None:
            # Make the current node the lowest G_score in the open set(IE shortest neighbor from last current)
            current = open_set[0]
            for each in open_set:
                if G_score[each] < G_score[current]:
                    current = each
            if (current.x == target.x) and (current.z == target.z):
                return self.construct_path(start, target, came_from)

            # Add current node to closed, mark it has been evaluated
            open_set.remove(current)
            closed_set.append(current)

            neighbors = current.neighbors
            for neighbor in neighbors:
                if neighbor in closed_set:
                    continue
                distance = current.distance(neighbor)
                tentative_G_score = G_score[current] + distance + neighbor.weight
                if neighbor not in open_set:
                    open_set.append(neighbor)
                elif tentative_G_score >= G_score[neighbor]:
                    continue

                came_from[neighbor] = current
                G_score[neighbor] = tentative_G_score
                H_score[neighbor] = G_score[neighbor] + self.calculate_h(neighbor.x, neighbor.z, target.x, target.z)

    def construct_path(self, start, target, came_from):
        total_path = [target]
        next_node = target
        while next_node != (start):
            next_node = came_from[next_node]
            total_path.append(next_node)
        return (total_path)

