from collections import deque

def calculate_total_price(map_grid):
    rows = len(map_grid)
    cols = len(map_grid[0])
    visited = [[False] * cols for _ in range(rows)]

    def is_valid(r, c, char):
        return 0 <= r < rows and 0 <= c < cols and not visited[r][c] and map_grid[r][c] == char

    def bfs(r, c):
        # Start BFS to find the region's area and perimeter
        queue = deque([(r, c)])
        visited[r][c] = True
        char = map_grid[r][c]
        area = 0
        perimeter = 0

        while queue:
            cr, cc = queue.popleft()
            area += 1

            # Check all four directions
            for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                nr, nc = cr + dr, cc + dc
                if is_valid(nr, nc, char):
                    visited[nr][nc] = True
                    queue.append((nr, nc))
                elif not (0 <= nr < rows and 0 <= nc < cols) or map_grid[nr][nc] != char:
                    # Count edges that are at the boundary or next to a different type
                    perimeter += 1

        return area, perimeter

    total_price = 0

    for r in range(rows):
        for c in range(cols):
            if not visited[r][c]:
                area, perimeter = bfs(r, c)
                print(f"Region at ({r}, {c}): Area = {area}, Perimeter = {perimeter}")
                total_price += area * perimeter

    return total_price

# Read the map from input.txt
with open('input.txt', 'r') as file:
    map_grid = [list(line.strip()) for line in file]

# Calculate and print the total price
total_price = calculate_total_price(map_grid)
print(f"Total Price: {total_price}")
