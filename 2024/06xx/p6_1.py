def read_grid_from_file(filename):
    """
    Read the grid from a file and return it as a 2D list (list of lists).
    :param filename: The name of the file containing the grid
    :return: 2D list representing the map/grid
    """
    with open(filename, 'r') as file:
        grid = [list(line.strip()) for line in file.readlines()]
    return grid

def find_start_position(grid):
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j] == '^':
                return (i, j)
    return None

def turn_right(direction):
    if direction == (-1, 0): # Up
        return (0, 1) # Right
    
    if direction == (0, 1): # Right
        return (1, 0) # Down
    
    if direction == (1, 0):	# Down
        return (0, -1) # Left

    return (-1, 0) # Uo

def move(position, direction):
    return (position[0] + direction[0], position[1] + direction[1])

def is_within_bounds(position, grid):
    x, y = position
    return 0 <= y < len(grid) and 0 <= x < len(grid[0])

def find_guard_path(grid, guard_start):
    direction = (-1, 0)  # Start moving up
    position = guard_start

    visited_positions = set()
    visited_positions.add(position)
    
    while True:
        next_position = move(position, direction)
        if is_within_bounds(next_position, grid):
            if grid[next_position[0]][next_position[1]] != '#':
                position = next_position
                visited_positions.add(position)
            else:
                direction = turn_right(direction)
        else:
            break

    return len(visited_positions)

if __name__ == "__main__":
    # Read the map from the file
    grid = read_grid_from_file('data.txt')
    guard_start = find_start_position(grid)
    # Simulate the guard and print the result
    positions = find_guard_path(grid, guard_start)
    print("Number of distinct positions visited:", positions)
