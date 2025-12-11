
YELLOW = '\033[93m'
CYAN = '\033[96m'
END = '\033[0m'

# Function to parse the input file and extract the relevant data
def parse_input_file(filename):
    prizes = []
    with open(filename, 'r') as file:
        lines = file.readlines()
        for i in range(0, len(lines), 4):  # Each machine's data takes 4 lines
            Ax, Ay = map(int, lines[i].strip().split('X+')[1].split(', Y+'))
            Bx, By = map(int, lines[i+1].strip().split('X+')[1].split(', Y+'))
            Px, Py = map(int, lines[i+2].strip().split('X=')[1].split(', Y='))
            prizes.append((Ax, Ay, Bx, By, Px, Py))
    return prizes

import math

# Algoritmo de Euclides para calcular MDC(a,b) = g, alpha = x, beta = y
def egcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x1, y1 = egcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return g, x, y

# Si P no es múltiplo del MCD(A,B), entonces NO hay solución
def has_solution(A, B, P):
	mcd, _, _ = egcd(A, B)
	return P % mcd == 0

# Función para resolver el sistema de ecuaciones
def solve_diophantine(A, B, P):
    mcd, alpha, beta = egcd(A, B)
    if P % mcd != 0:
        return None

	# Ecuaciones de la forma:
    #    x = x0 + Bg * t
    #    y = y0 - Ag * t
    k = P // mcd
    x0 = alpha * k
    y0 = beta * k
    Bg = B // mcd
    Ag = A // mcd

    return x0, Bg, y0, Ag

def solve_equation(prize):
	Ax, Ay, Bx, By, Px, Py = prize
	Px += 10**13
	Py += 10**13
	print(YELLOW, end="")
	print(f"{Ax}X + {Bx}Y = {Px}")
	print(f"{Ay}X + {By}Y = {Py}", end="")
	print(END)
	if (not has_solution(Ax, Bx, Px)) or (not has_solution(Ay, By, Py)):
		print("  Solucion = NONE")
		print()
		return 0

	# Resuelvo la primera ecuación y obtengo x0, Bg, y0, Ag
	x0, Bg, y0, Ag = solve_diophantine(Ax, Bx, Px)
    
	# Sustituimos en la segunda ecuación para obtener x, y
	numerador = Py - Ay * x0 - By * y0
	denominador = Ay * Bg - By * Ag
	t = numerador // denominador
	x = x0 + Bg * t
	y = y0 - Ag * t
      
	# Comprobamos que la solución es correcta
	if (Ax*x + Bx*y != Px) or (Ay*x + By*y != Py):
		print("  Solucion = NONE")
		print()
		return 0

    # Si es correcta, calculamos el coste y lo devolvemos
	print(f"  Solucion = ({x}, {y})")
	coste = 3*x + y	
	print (f"  Coste = {coste}")
	print()
	return coste

# Main function
def main():
	prizes = parse_input_file('data.txt')
	cost = 0
	for prize in prizes:
		cost += solve_equation(prize)
            
	print(f"{CYAN}Total cost: {END}{cost}")
	print()

main()
