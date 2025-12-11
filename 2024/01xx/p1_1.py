import csv

def process_csv(file_path: str) -> int:
    column1 = []
    column2 = []
    
    # Leer el archivo y separar las columnas
    with open(file_path, 'r') as file:
        reader = csv.reader(file, delimiter=' ')
        for row in reader:
            column1.append(int(row[0]))
            column2.append(int(row[1]))
    
    # Ordenar cada columna
    column1.sort()
    column2.sort()
    
    # Calcular las restas y sumar
    differences_sum = sum(abs(c1 - c2) for c1, c2 in zip(column1, column2))
    
    return differences_sum

# Ejecución principal para probar
if __name__ == "__main__":
    # Ruta del archivo CSV
    file_path = "data.csv"
    
    try:
        result = process_csv(file_path)
        print(f"La suma total de las restas es: {result}")
    except FileNotFoundError:
        print("El archivo especificado no se encontró.")
    except ValueError:
        print("El archivo tiene un formato incorrecto.")
