def count_xmas(grid):
    # Dimensiones del tablero
    rows = len(grid)
    cols = len(grid[0])
    
    # Función para verificar si hay un X-MAS en el centro (r, c)
    def is_xmas(r, c):
        # Verificar límites para las diagonales
        if r - 1 < 0 or r + 1 >= rows or c - 1 < 0 or c + 1 >= cols:
            return False
        
        # Validar las dos diagonales con ambas direcciones
        # Diagonal top-left -> bottom-right
        diagonal1 = [grid[r - 1][c - 1], grid[r][c], grid[r + 1][c + 1]]
        # Diagonal top-right -> bottom-left
        diagonal2 = [grid[r - 1][c + 1], grid[r][c], grid[r + 1][c - 1]]
        
        # Revisar si ambas diagonales son "MAS" o "SAM"
        valid_mas_sam = ["MAS", "SAM"]
        return "".join(diagonal1) in valid_mas_sam and "".join(diagonal2) in valid_mas_sam

    # Contar todas las X-MAS válidas
    count = 0
    for r in range(1, rows - 1):  # Saltar los bordes
        for c in range(1, cols - 1):  # Saltar los bordes
            if is_xmas(r, c):
                count += 1
    
    return count


# Leer la cuadrícula desde el archivo data.txt
def read_grid_from_file(filename):
    with open(filename, 'r') as file:
        grid = [line.strip() for line in file.readlines()]
    return grid


# Programa principal
if __name__ == "__main__":
    filename = "data.txt"  # Archivo que contiene la cuadrícula
    grid = read_grid_from_file(filename)  # Leer la cuadrícula
    result = count_xmas(grid)  # Contar las X-MAS
    print(f"The X-MAS appears {result} times.")
