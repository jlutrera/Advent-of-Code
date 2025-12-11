from collections import deque

def bfs(map, start, end):
    """
    Encuentra el camino más corto en el mapa.
    """
    map_size = len(map)
    # Cola para BFS: (posición actual, número de pasos)
    queue = deque([(start, 0)])
    visited = set()
    visited.add(start)  # Registrar posición
    directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    while queue:
        (x, y), steps = queue.popleft()

        # Si alcanzamos el final, devolvemos el resultado
        if (x, y) == end:
            return steps

        # Explorar vecinos
        for dx, dy in directions:
            nx, ny = x + dx, y + dy

            # Verificar si la nueva posición es válida y transitable
            if 0 <= nx < map_size and 0 <= ny < map_size:
                # Asegurarnos de que no hemos visitado esta posición con este estado de bytes
                if map[nx][ny] != '#' and (nx, ny) not in visited:
                    visited.add((nx, ny))
                    queue.append(((nx, ny), steps + 1))

    # Si no hay camino posible, devolver 0
    return 0

def find_S_E(map):
    """
    Encuentra las posiciones de inicio y final en el mapa.
    """
    start = end = None
    for i in range(len(map)):
        for j in range(len(map[i])):
            if map[i][j] == 'S':
                start = (i, j)
            elif map[i][j] == 'E':
                end = (i, j)
            if start and end:
                break
        if start and end:
            break

    return start, end

def read_map(filename):
    """
    Read the map from the file.
    """
    with open(filename, 'r') as file:
        map_lines = file.read().strip().split('\n')
    return [list(line) for line in map_lines]

def find_cheats(map, start, end, normal_time, min_time_saved):
    """
    This function finds all possible cheats and their corresponding time savings.
    """
    cheats = []
    size = len(map)

    # For each possible position, simulate a cheat by skipping a wall
    for x in range(size):
        for y in range(size):
            if map[x][y] == '#':  # Obstacle found
                # Temporarily cheat through the two consecutive walls
                map[x][y] = '.'
                         
                # Calculate the time with the cheat
                cheat_time = bfs(map, start, end)
                            
                # Time saved
                time_saved = normal_time - cheat_time
                if time_saved >= min_time_saved:
                    cheats.append(cheat_time)
                            
                # Restore the map after simulating the cheat
                map[x][y] = '#'
    
    return cheats

def main():
    # Read the map from input.txt
    map = read_map("data.txt")
    
    # Find the start (S) and end (E) positions
    start, end = find_S_E(map)

    # Find the shortest path time without any cheating
    normal_time = bfs(map, start, end)
    print("Normal time:", normal_time)

    # Try finding cheats and save time
    min_time_saved = 100
    cheats = find_cheats(map, start, end, normal_time, min_time_saved)

    # Count cheats
    # for i in range(1, normal_time):
    #     c = sum(1 for cheat in cheats if cheat == i)
    #     if c > 0:
    #         print(f"There are {c} cheats that save {normal_time-i} picoseconds.")
    print(f"Total cheats: {len(cheats)}")

if __name__ == "__main__":
    main()
