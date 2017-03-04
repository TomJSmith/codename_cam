class A_star():
    """
    Init and use a grid... probably use nav mesh? Use True values as passable terrain and false for walls/objects
    """
    def __init__(self, grid):
        self.grid = grid

    def calculate_h(self, x, y, targetX, targetY):
        H_score = 10 * (abs(x - targetX) + abs(y - targetY))
        return H_score

    def find_path(self, start, target):
        dx = [1, 1, 1, 0, -1, -1, -1, 0]
        dy = [-1, 0, 1, 1, 1, 0, -1, -1]

        open_set = [start]
        closed_set = []
        came_from = {}
        G_score = {}
        G_score[start] = 0
        H_score = {}
        H_score[start] = self.calculate_h(start[0], start[1], target[0], target[1])

        while open is not None:
            #Make the current node the lowest G_score in the open set(IE shortest neighbor from last current)
            current = open_set[0]
            for each in open_set:
                if G_score[each] < G_score[current]:
                    current = each
            if (current[0] == target[0]) and (current[1] == target[1]):
                return self.construct_path(start, target, came_from)

            # Add current node to closed, mark it has been evaluated
            open_set.remove(current)
            closed_set.append(current)


            neighbors = self.create_neighbors(current[0], current[1], grid)
            for neighbor in neighbors:
                if neighbor in closed_set:
                    continue
                if (neighbor[0] == current[0]) or (neighbor[1] == current[1]):
                    distance = 10
                else:
                    distance = 14
                tentative_G_score = G_score[current] + distance
                if neighbor not in open_set:
                    open_set.append(neighbor)
                elif tentative_G_score >= G_score[neighbor]:
                    continue

                came_from[neighbor] = current
                G_score[neighbor] = tentative_G_score
                H_score[neighbor] = G_score[neighbor] + self.calculate_h(neighbor[0], neighbor[1], target[0], target[1])

    def construct_path(self,start,  target, came_from):
        total_path = [target]
        next_node = target
        while next_node != (start):
            next_node = came_from[next_node]
            total_path.append(next_node)
        return (total_path)

    def create_neighbors(self, x, y, grid):
        dxy = [(1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1)]
        neighbors = []
        for XY in dxy:
            new_x = x + XY[0]
            new_y = y + XY[1]
            if (new_x < len(grid)) and (new_y < len(grid)) and (new_x >= 0) and (new_y >= 0):
                if grid[new_x][new_y]:
                    neighbors.append((new_x, new_y))
        return neighbors


# EXAMPLE... Run this python file seperatly to run the example

if __name__ == "__main__":
    grid = [[True for i in range(12)] for i in range(12)]
    walls = [(5, 10), (10, 7), (1, 0), (2, 4), (5, 2),
            (5, 8), (5, 2), (8, 7), (6, 11), (2, 9),
            (11, 0), (2, 10), (3, 4), (3, 5), (1, 5),
            (8, 1), (3, 1), (10, 10), (9, 3), (3, 7), (4,4), (5,4), (5,3)]
    for x, y in walls:
        grid[x][y] = False

    map = A_star(grid)
    fast_path = map.find_path((0,0), (11,11))

    draw_grid = [[" " for i in range(12)] for i in range(12)]
    for row in range(12):
        for column in range(12):
            if grid[row][column]:
                draw_grid[row][column] = " "
            else:
                draw_grid[row][column] = "X"
            if (row, column) in fast_path:
                draw_grid[row][column] = "O"

    for row in draw_grid:
        print(row)
