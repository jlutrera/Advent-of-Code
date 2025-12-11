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


def main():
    file_path = "data.txt"  # Ruta al archivo
    dependencies, sequences = parse_file(file_path)
    sum = 0
    for _, sequence in enumerate(sequences, 1):
        if validate_sequence(sequence, dependencies):
            central_value = get_central_value(sequence)
            sum += central_value
            print(f"{central_value} ", end="")
    print()
    print(f"Result = {sum}")

if __name__ == "__main__":
    main()
