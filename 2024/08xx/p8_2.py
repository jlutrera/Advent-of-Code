def count_propagated_antinodes(file_path):
    import itertools

    # Leer el mapa del archivo
    with open(file_path, 'r') as file:
        map_input = [line.strip() for line in file.readlines()]

    # Parsear las posiciones de las antenas
    antennas = {}
    for y, row in enumerate(map_input):
        for x, char in enumerate(row):
            if char != '.':
                antennas.setdefault(char, []).append((x, y))

    max_x = len(map_input[0])
    max_y = len(map_input)

    # Función para verificar si un punto está dentro del mapa
    def is_within_bounds(x, y):
        return 0 <= x < max_x and 0 <= y < max_y

    # Conjunto para almacenar todas las posiciones de antinodos generados
    propagated_positions = set()

    # Agregar las posiciones iniciales de las antenas
    for freq, positions in antennas.items():
        for pos in positions:
            propagated_positions.add(pos)

    # Función para propagar antinodos en una línea
    def propagate_line(x1, y1, x2, y2, freq):
        dx, dy = x2 - x1, y2 - y1  # Dirección de la línea
        for direction in [-1, 1]:  # Propagar en ambas direcciones
            curr_x, curr_y = x1 + direction * dx, y1 + direction * dy
            while is_within_bounds(curr_x, curr_y):
                if (curr_x, curr_y) not in propagated_positions:
                    propagated_positions.add((curr_x, curr_y))
                curr_x += direction * dx
                curr_y += direction * dy

    # Propagar desde cada par de antenas iniciales
    for freq, positions in antennas.items():
        for (x1, y1), (x2, y2) in itertools.combinations(positions, 2):
            propagate_line(x1, y1, x2, y2, freq)

    # Contar todas las posiciones únicas de antinodos
    return len(propagated_positions)

# Ejemplo de uso
file_path = 'data.txt'
print("Total unique antinodes:", count_propagated_antinodes(file_path))
