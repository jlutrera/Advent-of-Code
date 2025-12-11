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

def dfs_count_paths(map_data, x, y, current_height, memo):
    rows, cols = len(map_data), len(map_data[0])

    # Memoization to prevent redundant calculations
    if (x, y, current_height) in memo:
        return memo[(x, y, current_height)]

    # If we reach height 9, this is a valid path
    if map_data[x][y] == 9:
        return 1

    # Initialize count of distinct paths
    count = 0

    # Explore all four possible directions (up, down, left, right)
    for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
        nx, ny = x + dx, y + dy

        # Check if the new position is valid
        if 0 <= nx < rows and 0 <= ny < cols and map_data[nx][ny] == current_height + 1:
            count += dfs_count_paths(map_data, nx, ny, map_data[nx][ny], memo)

    # Store result in memoization dictionary
    memo[(x, y, current_height)] = count
    return count

def calculate_trailhead_ratings(file_path):
    # Read the map
    topographic_map = read_map(file_path)

    # Find all trailheads
    trailheads = find_trailheads(topographic_map)

    # Calculate ratings for all trailheads
    total_rating = 0
    for trailhead in trailheads:
        x, y = trailhead
        memo = {}
        total_rating += dfs_count_paths(topographic_map, x, y, 0, memo)

    return total_rating

if __name__ == "__main__":
    file_name = "data.txt"
    total_rating = calculate_trailhead_ratings(file_name)
    print(f"The sum of the ratings of all trailheads is: {total_rating}")
