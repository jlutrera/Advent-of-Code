def parse_input(input_data):
    robots = []
    for line in input_data.strip().split("\n"):
        position, velocity = line.split(" v=")
        p_x, p_y = map(int, position[2:].split(","))
        v_x, v_y = map(int, velocity.split(","))
        robots.append(((p_x, p_y), (v_x, v_y)))
    return robots

def simulate_robots(robots, width, height, seconds):
    final_positions = []
    for (p_x, p_y), (v_x, v_y) in robots:
        # Update position after `seconds` seconds
        new_x = (p_x + v_x * seconds) % width
        new_y = (p_y + v_y * seconds) % height
        final_positions.append((new_x, new_y))
    return final_positions

def count_quadrants(positions, width, height):
    mid_x = width // 2
    mid_y = height // 2

    quadrant_counts = [0, 0, 0, 0]  # Top-left, Top-right, Bottom-left, Bottom-right

    for x, y in positions:
        # Skip robots exactly on the middle lines
        if x == mid_x or y == mid_y:
            continue

        # Assign robots to quadrants
        if x < mid_x and y < mid_y:
            quadrant_counts[0] += 1  # Top-left
        elif x > mid_x and y < mid_y:
            quadrant_counts[1] += 1  # Top-right
        elif x < mid_x and y > mid_y:
            quadrant_counts[2] += 1  # Bottom-left
        elif x > mid_x and y > mid_y:
            quadrant_counts[3] += 1  # Bottom-right

    return quadrant_counts

def calculate_safety_factor(quadrant_counts):
    factor = 1
    for count in quadrant_counts:
        factor *= count
    return factor

# Read input data from file
with open("data.txt", "r") as file:
    input_data = file.read()

# Parameters
width = 101
height = 103
seconds = 100

# Parse input
robots = parse_input(input_data)

# Simulate robot movement
final_positions = simulate_robots(robots, width, height, seconds)

# Count robots in each quadrant
quadrant_counts = count_quadrants(final_positions, width, height)

# Calculate safety factor
safety_factor = calculate_safety_factor(quadrant_counts)

print("Quadrant Counts:", quadrant_counts)
print("Safety Factor:", safety_factor)
