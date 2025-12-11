def parse_input(input_data):
    robots = []
    for line in input_data.strip().split("\n"):
        position, velocity = line.split(" v=")
        p_x, p_y = map(int, position[2:].split(","))
        v_x, v_y = map(int, velocity.split(","))
        robots.append(((p_x, p_y), (v_x, v_y)))
    return robots

def simulate_robots_step(robots):
    # Update positions by one second
    new_positions = []
    for (p_x, p_y), (v_x, v_y) in robots:
        new_x = (p_x + v_x) % 101
        new_y = (p_y + v_y) % 103
        new_positions.append((new_x, new_y))
    return [(pos, vel) for pos, vel in zip(new_positions, [vel for _, vel in robots])]

def visualize_positions(positions, min_x, max_x, min_y, max_y):
    grid = [["." for _ in range(min_x, max_x + 1)] for _ in range(min_y, max_y + 1)]
    for x, y in positions:
        grid[y - min_y][x - min_x] = "#"
    return "\n".join("".join(row) for row in grid)

def all_positions_unique(positions):
    # Check if all positions are unique
    return len(positions) == len(set(positions))

# Read input data from file
with open("data.txt", "r") as file:
    input_data = file.read()

# Parse input
robots = parse_input(input_data)

# Find the moment when all positions are unique
seconds = 0

while True:
    # Extract positions
    positions = [pos for pos, _ in robots]

    # Check if all positions are unique
    if all_positions_unique(positions):
        break

    # Move robots forward by one second
    robots = simulate_robots_step(robots)
    seconds += 1

# Visualize the final positions
area_min_x, area_max_x, area_min_y, area_max_y = 0, 100, 0, 102
print(f"Fewest seconds: {seconds}")
print(visualize_positions(positions, area_min_x, area_max_x, area_min_y, area_max_y))
