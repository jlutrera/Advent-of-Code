def read_grid_from_file(filename):
    """
    Read the grid from a file and return it as a 2D list (list of lists).
    :param filename: The name of the file containing the grid
    :return: 2D list representing the map/grid
    """
    with open(filename, 'r') as file:
        grid = [list(line.strip()) for line in file.readlines()]
    return grid

def find_guard_start(grid):
    """
    Find the guard's starting position, marked by '^'.
    :param grid: 2D list representing the map
    :return: Tuple of the guard's starting coordinates (x, y)
    """
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

def find_loop_path(grid, guard_start):
    """
    Simulate the movement of the guard and return the path the guard would take.
    :param grid: 2D list representing the map
    :param guard_start: Tuple (x, y) representing the guard's starting position
    :return: Set of positions visited by the guard
    directions = [
    	(-1,0)  # Up
        (1, 0) 	# Down
        (0,-1)  # Left
        (0, 1) 	# Right
    ]
    """
    direction = (-1, 0)  # Start moving up
    position = guard_start

    visited = set()
    
    while True:
        entry = (position, direction)
        visited.add(entry)
        next_position = move(position, direction)
        if is_within_bounds(next_position, grid):
            if grid[next_position[0]][next_position[1]] != '#' and grid[next_position[0]][next_position[1]] != 'O':
               position = next_position
            else:
               direction = turn_right(direction)
            entry = (position, direction)   
            if entry in visited:
                break
        else:
          return 0
    return 1

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

    return visited_positions

def find_valid_obstruction_positions(grid, guard_start):
    """
    Find all valid positions where placing an obstruction will create a loop.
    grid: 2D list representing the map
    guard_start: Tuple (x, y) representing the guard's starting position
    visited_positions: Set of positions visited by the guard = possible positions for obstruction
    return: Number of valid obstruction positions
    """
    valid_positions = 0
    visited_positions = find_guard_path(grid, guard_start)
    # Try placing an obstruction ('O') in every empty space ('.')
    for (i, j) in visited_positions:
       if grid[i][j] == '.' and (i, j) != guard_start:
          # Create a new grid with an obstruction placed
          new_grid = [row[:] for row in grid]
          new_grid[i][j] = 'O'
          # Simulate the guard's movement with the new obstruction
          find_loop = find_loop_path(new_grid, guard_start)
          # If the new position causes a loop, add it to the valid positions
          valid_positions += find_loop

    return valid_positions

# Main execution
if __name__ == "__main__":
    # Read the grid from the file
    grid = read_grid_from_file('data.txt')
    # Find the guard's starting position
    guard_start = find_guard_start(grid)
   #print(f"Guard starting position: {guard_start}")
    valid_positions = find_valid_obstruction_positions(grid, guard_start)
    print(f"  Result: {valid_positions}")
