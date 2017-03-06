import sys
import math

class Node():
    def __init__(self, x, z):
        """
        Node Object. Represents a point on a graph
        :param x: X coordinate of node
        :param z: Z coordinate of node
        :param neighbors: List of Node objects reprsenting a nodes neighbors
        """
        self.x = x
        self.z = z
        self.neighbors = []

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
            return 10 * math.sqrt((abs(self.x - node.x)**2) + (abs(self.z - node.z)**2))
        else:
            raise Exception("Node (%l, %l) is not a neighbor of node (%l, %l)"%(node.x, node.z, self.x, self.z))

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
            raise Exception("Failure to add neighbor for node (%i, %i)"%(self.x, self.y))


class A_star():
    """
    A * class with functions to calculate shortest path
    """
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
            #Make the current node the lowest G_score in the open set(IE shortest neighbor from last current)
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
                tentative_G_score = G_score[current] + distance
                if neighbor not in open_set:
                    open_set.append(neighbor)
                elif tentative_G_score >= G_score[neighbor]:
                    continue

                came_from[neighbor] = current
                G_score[neighbor] = tentative_G_score
                H_score[neighbor] = G_score[neighbor] + self.calculate_h(neighbor.x, neighbor.z, target.x, target.z)

    def construct_path(self,start,  target, came_from):
        total_path = [target]
        next_node = target
        while next_node != (start):
            next_node = came_from[next_node]
            total_path.append(next_node)
        return (total_path)

    def create_neighbors(self, x, y, grid):
        """
        OLD, POSSIBLY OF NO USE
        """
        dxy = [(1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1)]
        neighbors = []
        for XY in dxy:
            new_x = x + XY[0]
            new_y = y + XY[1]
            if (new_x < len(grid)) and (new_y < len(grid)) and (new_x >= 0) and (new_y >= 0):
                if grid[new_x][new_y]:
                    neighbors.append((new_x, new_y))
        return neighbors


# EXAMPLE... Run this python file separately to run the example

if __name__ == "__main__":
    node_a = Node(1, 1)
    node_b = Node(2.5, 1.5)
    node_c = Node(3, 3)
    node_d = Node(4, 4)
    node_e = Node(5, 5)
    node_f = Node(6, 6)
    node_g = Node(6, 1)
    node_h = Node(1, 5)
    node_i = Node(5, 5)
    node_j = Node(5, 1)
    node_k = Node(2.5, 5.5)
    node_l = Node(5, 2)
    node_m = Node(2, 5)

    node_a.add_neighbor([node_b, node_g, node_k])
    node_b.add_neighbor([node_c, node_a])
    node_c.add_neighbor([node_m, node_b])
    node_d.add_neighbor([node_e, node_l])
    node_e.add_neighbor([node_f, node_d])
    node_f.add_neighbor([node_e, node_j, node_k])
    node_g.add_neighbor([node_a, node_h])
    node_h.add_neighbor([node_g, node_i])
    node_i.add_neighbor([node_h, node_j])
    node_j.add_neighbor([node_f, node_i])
    node_k.add_neighbor([node_a, node_f])
    node_l.add_neighbor([node_d, node_m])
    node_m.add_neighbor([node_l, node_c])


    map = A_star()
    fast_path = map.find_path(node_a, node_f)

    for node in fast_path:
        print(str(node.x) + " " + str(node.z))

