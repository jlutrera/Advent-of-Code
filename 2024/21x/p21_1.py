from collections import deque

YELLOW = '\033[93m'
CYAN = '\033[96m'
END = '\033[0m'

# Matriz del teclado
keypad = [
    ['7', '8', '9'],
    ['4', '5', '6'],
    ['1', '2', '3'],
    [None, '0', 'A'] 
]

cursors = [
    [None, '^', 'A'],
    ['<', 'v', '>']
]

# Movimientos posibles (arriba, abajo, izquierda, derecha)
movimientos = [(-1, 0, '^'), (0, 1, '>'), (1, 0, 'v'), (0, -1, '<')]

# Función que devuelve las coordenadas de cada valor en la matriz
def find_coordinates(value, mapa):
    for i, row in enumerate(mapa):
        for j, cell in enumerate(row):
            if cell == value:
                return (i, j)
    return None

# Función para realizar la búsqueda de los caminos más cortos usando BFS
def bfs(start, goal, mapa):
    """
    Encuentra todos los caminos más cortos entre un punto inicial y final en un mapa.

    Args:
        start: Tupla (fila, columna) representando el punto de inicio.
        goal: Tupla (fila, columna) representando el punto final.
        mapa: Lista de listas representando el mapa, donde '.' son espacios válidos y '#' son obstáculos.

    Returns:
        Lista de listas, donde cada lista interna representa un camino más corto.
    """

    stack = [(start[0], start[1], [], set(), 0)]  # (fila, columna, camino, visitados, longitud)
    resultados = []
    min_length = float('inf')  # Inicializamos la longitud mínima a infinito

    while stack:
        x, y, path, visited, length = stack.pop()

        if (x, y) == goal:
            if length < min_length:
                resultados = [path]  # Reemplazamos los resultados si encontramos un camino más corto
                min_length = length
            elif length == min_length:
                resultados.append(path)  # Agregamos el camino si tiene la misma longitud mínima
            continue

        for dx, dy, direction in movimientos:
            nx, ny = x + dx, y + dy
            if 0 <= nx < len(mapa) and 0 <= ny < len(mapa[nx]) and mapa[nx][ny] is not None and (nx, ny) not in visited:
                new_visited = visited.copy()
                new_visited.add((nx, ny))
                stack.append((nx, ny, path + [direction], new_visited, length + 1))

    return resultados

def generate_combinations(paths):
    def helper(index, current):
        # Caso base: si hemos procesado todos los elementos de paths
        if index == len(paths):
            results.append(current)
            return
        
        # Recorremos cada sublista en paths[index]
        for sublist in paths[index]:
            # Concatenamos la representación del sublist con "A" y seguimos con el siguiente índice
            helper(index + 1, current + ''.join(sublist) + 'A')

    # Generar todas las combinaciones
    results = []
    helper(0, "")

    # Encontrar la longitud mínima
    min_length = min(len(combination) for combination in results)

    # Filtrar las combinaciones más cortas
    shortest_combinations = [combination for combination in results if len(combination) == min_length]
    
    return shortest_combinations

# Función principal que calcula todas las secuencias para el input
def find_shortest_paths(codigo, mapa):
    # Encontramos las coordenadas de cada carácter en el código
    positions = []
    for char in codigo:
        positions.append(find_coordinates(char, mapa))
    
    # Para almacenar todas las secuencias de movimientos más cortos
    all_paths = []

    # Iterar por el código y encontrar los caminos más cortos entre caracteres sucesivos
    for i in range(len(positions) - 1):
        start = positions[i]
        goal = positions[i + 1]
        paths = bfs(start, goal, mapa)
        all_paths.append(paths)

    return all_paths

def options(combinations0):
    min_length1 = 10**10
    combinations1 = []
    for combination0 in combinations0:
        paths1 = find_shortest_paths( "A" + combination0, cursors)
        combi = generate_combinations(paths1)
        combinations1 += combi
        min_length = min(len(combination) for combination in combinations1)
        if min_length < min_length1:
            min_length1 = min_length
    combinations1 = [combination for combination in combinations1 if len(combination) == min_length1]
    return min_length1, combinations1

# Ejemplo de uso
codigos = ['413A', '480A', '682A', '879A', '083A']

print("Complejidad")
complejidad_total = 0
directional_keypad_robots = 2

for codigo in codigos:
    paths0 = find_shortest_paths("A" + codigo, keypad)
    combinations0 = generate_combinations(paths0)

    for _ in range(directional_keypad_robots):
        min_length, combinations = options(combinations0)
        combinations0 = combinations

    print(YELLOW, codigo, END, end=" ")
    numeric_part = int(codigo[:3])
    print(f" = {min_length} * {numeric_part}")
    complejidad_total += numeric_part * min_length

print(CYAN, "Total: ", END, complejidad_total)

