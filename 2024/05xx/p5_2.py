def parse_file(file_path):
    with open(file_path, 'r') as file:
        # Leer y dividir el contenido en líneas
        lines = file.readlines()
    
    # Separar dependencias y secuencias
    dependencies = []
    sequences = []
    for line in lines:
        if '|' in line:
            dependencies.append(tuple(map(int, line.strip().split('|'))))
        elif ',' in line:
            sequences.append(list(map(int, line.strip().split(','))))
    
    return dependencies, sequences


def validate_sequence(sequence, dependencies):
    for a, b in dependencies:
        # Verificar que 'a' aparezca antes que 'b' en la secuencia
        if a in sequence and b in sequence:
            if sequence.index(a) > sequence.index(b):
                return False
    return True


def get_central_value(sequence):
    # Calcula el índice central
    if sequence:
        central_index = len(sequence) // 2
        return sequence[central_index]
    return None


def sort_sequence(sequence, dependencies):
    # Crear un grafo de dependencias
    from collections import defaultdict, deque

    graph = defaultdict(list)
    in_degree = {num: 0 for num in sequence}

    # Construir grafo de dependencias
    for a, b in dependencies:
        if a in sequence and b in sequence:
            graph[a].append(b)
            in_degree[b] += 1

    # Ordenación topológica
    queue = deque([node for node in sequence if in_degree[node] == 0])
    sorted_sequence = []

    while queue:
        current = queue.popleft()
        sorted_sequence.append(current)
        for neighbor in graph[current]:
            in_degree[neighbor] -= 1
            if in_degree[neighbor] == 0:
                queue.append(neighbor)

    return sorted_sequence

def get_central_value(sequence):
    # Calcula el índice central
    if sequence:
        central_index = len(sequence) // 2
        return sequence[central_index]
    return None

def main():
    file_path = "data.txt"  # Ruta al archivo
    dependencies, sequences = parse_file(file_path)
    sum = 0
    for _, sequence in enumerate(sequences, 1):
        if not validate_sequence(sequence, dependencies):
            corrected_sequence = sort_sequence(sequence, dependencies)
            central_value = get_central_value(corrected_sequence)
            sum += central_value
            print(f"{sequence} --> {corrected_sequence} ==== {central_value}")

    print(f"Result = {sum}")

if __name__ == "__main__":
    main()
