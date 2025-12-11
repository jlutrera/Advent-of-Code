from collections import defaultdict

def transform_stone(stone):
    """Transforma una piedra según las reglas especificadas."""
    if stone == 0:
        return [1]
    
    # Convierte el número a cadena y verifica si el número tiene un número par de dígitos
    stone_str = str(stone)
    if len(stone_str) % 2 == 0:
        mid = len(stone_str) // 2
        left_half = int(stone_str[:mid])
        right_half = int(stone_str[mid:])
        return [left_half, right_half]
    
    # Si ninguna de las condiciones anteriores aplica, multiplica por 2024
    return [stone * 2024]

def blink(stone_counts):
    """Realiza una iteración de transformación de piedras usando un diccionario de cuentas."""
    new_counts = defaultdict(int)
    
    # Procesa cada piedra en el contador
    for stone, count in stone_counts.items():
        # Transforma la piedra y actualiza el contador de nuevas piedras
        new_stones = transform_stone(stone)
        for new_stone in new_stones:
            new_counts[new_stone] += count
    
    return new_counts

# Inicialización de las piedras
initial_stones = [0,89741,316108,7641,756,9,7832357,91]
iterations = 75

# Contador de piedras (diccionario de ocurrencias)
stone_counts = defaultdict(int)
for stone in initial_stones:
    stone_counts[stone] += 1

# Realizar las iteraciones de blink
for _ in range(iterations):
    stone_counts = blink(stone_counts)

# Contar el total de piedras después de las iteraciones
total_stones = sum(stone_counts.values())
print(total_stones)
