import csv

def procesar_csv(nombre_archivo):
	# Inicializar la variable para almacenar la suma de las multiplicaciones
	suma_multiplicaciones = 0

	# Leer el archivo CSV
	with open(nombre_archivo, newline='') as archivo:
		lector = csv.reader(archivo, delimiter=' ')

		# Convertir la lectura del CSV en una lista de tuplas
		datos = [tuple(map(int, fila)) for fila in lector]

	# Iterar a través de todos los números de la primera columna	
	for num1, _ in datos:
		# Contar cuántas veces aparece el valor de num2 en la segunda columna
		veces = sum(1 for _, n in datos if n == num1)
        
		# Multiplicar num1 por el número de veces que aparece num2 en la segunda columna
		multiplicacion = num1 * veces
		if veces > 1:
			print(f"{num1} * {veces} = {multiplicacion}")

		# Sumar la multiplicación al total
		suma_multiplicaciones += multiplicacion

	# Mostrar el resultado final
	print("La suma de todas las multiplicaciones es:", suma_multiplicaciones)

# Usar la función con el nombre del archivo CSV
procesar_csv('data.csv')
