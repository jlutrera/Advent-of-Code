from collections import deque

def parse_input(file_path):
    """Parse the input file into a list of corrupted byte positions."""
    with open(file_path, 'r') as f:
        return [tuple(map(int, line.split(','))) for line in f.read().strip().split('\n')]

def simulate_memory(grid_size, corrupted_positions, bytes_fallen):
    """Simulate the memory corruption after num_bytes have fallen."""
    grid = [["." for _ in range(grid_size)] for _ in range(grid_size)]

    for x, y in corrupted_positions[:bytes_fallen]:
        grid[y][x] = "#"  # Mark corrupted positions

    return grid

def find_shortest_path(grid):
    """Encuentra el camino más corto en el mapa."""
    grid_size = len(grid)
    start = (0, 0)
    end = (grid_size - 1, grid_size - 1)

    # Direcciones posibles: derecha, abajo, izquierda, arriba
    directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]

    # Cola para BFS: (posición actual, número de pasos, bytes encontrados, camino)
    queue = deque([(start, 0, [start])])
    visited = set()
    visited.add(start)  # Registrar posición

    while queue:
        (x, y), steps, path = queue.popleft()

        # Si alcanzamos el final, devolvemos el resultado
        if (x, y) == end:
            return steps, path

        # Explorar vecinos
        for dx, dy in directions:
            nx, ny = x + dx, y + dy

            # Verificar si la nueva posición es válida y transitable
            if 0 <= nx < grid_size and 0 <= ny < grid_size:
                # Asegurarnos de que no hemos visitado esta posición con este estado de bytes
                if grid[ny][nx] != '#' and (nx, ny) not in visited:
                    visited.add((nx, ny))
                    queue.append(((nx, ny), steps + 1, path + [(nx, ny)]))

    # Si no hay camino posible, devolver -1
    return -1, []

def print_grid_with_path(grid, path):
    """Muestra la cuadrícula con el camino marcado."""
    for y in range(len(grid)):
        row = ""
        for x in range(len(grid[y])):
            if (x, y) in path:
                row += "O"  # Marcar el camino con 'O'
            else:
                row += grid[y][x]
        print(row)


def print_grid(grid):
    """Print the grid."""
    for row in grid:
       print("".join(row))
       
# Example usage
file_path = 'data.txt'  # Path to the input file

# Parse input
data = parse_input(file_path)

# Define grid size (7x7 for the example, 71x71 for the full problem)
grid_size = 71
bytes_fallen = 1024

# Simulate memory corruption
grid = simulate_memory(grid_size, data, bytes_fallen)

# Print the grid
print_grid(grid)

steps, path = find_shortest_path(grid)

# Mostrar resultados
if steps == -1:
    print("No se encontró un camino dentro del límite de bytes.")
else:
    print(f"El camino más corto tiene {steps} pasos.")
    print("Mapa con el camino:")
    print_grid_with_path(grid, path)

