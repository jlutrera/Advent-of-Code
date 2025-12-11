from heapq import heappop, heappush

def parse_maze_from_file(filename):
    with open(filename, 'r') as f:
        maze = f.read()
    grid = []
    start = end = None
    for y, line in enumerate(maze.strip().split("\n")):
        row = list(line)
        for x, cell in enumerate(row):
            if cell == 'S':
                start = (x, y)
            elif cell == 'E':
                end = (x, y)
        grid.append(row)
    return grid, start, end

def neighbors(x, y, direction):
    """Returns possible movements: forward, rotate clockwise, rotate counterclockwise."""
    # Directions: 0 = East, 1 = North, 2 = West, 3 = South
    directions = [(1, 0), (0, -1), (-1, 0), (0, 1)]
    forward_dx, forward_dy = directions[direction]
    
    # Move forward
    yield (x + forward_dx, y + forward_dy, direction, 1)  # Cost = 1 point

    # Rotate clockwise
    yield (x, y, (direction + 1) % 4, 1000)  # Cost = 1000 points

    # Rotate counterclockwise
    yield (x, y, (direction - 1) % 4, 1000)  # Cost = 1000 points

def solve_maze(filename):
    grid, start, end = parse_maze_from_file(filename)
    
    # Priority queue for A* search: (score, x, y, direction)
    pq = [(0, start[0], start[1], 0)]  # Start facing East (direction = 0)
    visited = set()
    
    while pq:
        score, x, y, direction = heappop(pq)
        
        # If we reach the end, return the score
        if (x, y) == end:
            return score

        # Mark state as visited
        if (x, y, direction) in visited:
            continue
        visited.add((x, y, direction))

        # Explore neighbors
        for nx, ny, nd, cost in neighbors(x, y, direction):
            if 0 <= ny < len(grid) and 0 <= nx < len(grid[0]) and grid[ny][nx] != '#':
                heappush(pq, (score + cost, nx, ny, nd))

# Solve maze from file
if __name__ == "__main__":
    filename = "data.txt"
    print("Minimum Score:", solve_maze(filename))
