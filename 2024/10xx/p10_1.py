from collections import deque

def read_map(file_path):
    with open(file_path, 'r') as f:
        return [list(map(int, line.strip())) for line in f.readlines()]

def find_trailheads(topographic_map):
    trailheads = []
    for i, row in enumerate(topographic_map):
        for j, height in enumerate(row):
            if height == 0:
                trailheads.append((i, j))
    return trailheads

def calculate_score(map_data, trailhead):
    rows, cols = len(map_data), len(map_data[0])
    queue = deque([trailhead])
    visited = set()
    visited.add(trailhead)
    reachable_nines = set()

    while queue:
        x, y = queue.popleft()
        current_height = map_data[x][y]

        # Check neighbors (up, down, left, right)
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + dx, y + dy

            if 0 <= nx < rows and 0 <= ny < cols:
                next_height = map_data[nx][ny]

                # Valid hiking trail condition
                if (nx, ny) not in visited and next_height == current_height + 1:
                    queue.append((nx, ny))
                    visited.add((nx, ny))

                    # Check if it is a height 9
                    if next_height == 9:
                        reachable_nines.add((nx, ny))

    return len(reachable_nines)

def calculate_total_score(file_path):
    # Read the map
    topographic_map = read_map(file_path)

    # Find trailheads
    trailheads = find_trailheads(topographic_map)

    # Calculate scores for all trailheads
    total_score = 0
    for trailhead in trailheads:
        total_score += calculate_score(topographic_map, trailhead)

    return total_score

if __name__ == "__main__":
    file_name = "data.txt"
    total_score = calculate_total_score(file_name)
    print(f"The sum of the scores of all trailheads is: {total_score}")
