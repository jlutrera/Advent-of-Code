def count_unique_antinodes_from_file(file_path):
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

    antinodes = set()

    # Calcular antinodos solo para antenas iguales
    for freq, positions in antennas.items():
        for (x1, y1), (x2, y2) in itertools.combinations(positions, 2):
            dx, dy = x2 - x1, y2 - y1  # Dirección de la línea

            # Calcular los antinodos fuera de las antenas
            candidate_antinodes = [
                (x1 - dx, y1 - dy),  # Antinodo detrás de la primera antena
                (x2 + dx, y2 + dy),  # Antinodo más allá de la segunda antena
            ]

            # Añadir los antinodos calculados al conjunto
            antinodes.update(candidate_antinodes)

    # Mostrar los antinodos calculados antes de cualquier filtrado
    # print("Raw antinodes:", sorted(antinodes))

    # Eliminar los antinodos fuera de los límites del mapa
    max_x = len(map_input[0])
    max_y = len(map_input)
    antinodes = {
        (x, y)
        for x, y in antinodes
        if 0 <= x < max_x and 0 <= y < max_y
    }

    # Mostrar los antinodos después de eliminar los que están fuera de los límites
    # print("Filtered antinodes (in bounds):", sorted(antinodes))

    # Devolver el número de antinodos únicos válidos
    return len(antinodes)

# Ejemplo de uso
file_path = 'data.txt'
print("Total unique antinodes:", count_unique_antinodes_from_file(file_path))
