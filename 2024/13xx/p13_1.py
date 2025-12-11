from itertools import product
import re

def find_min_tokens(a, b, c, d, px, py):
    min_cost = float('inf')
    solution = None

    # Iterate over all possible values of x_A and x_B (both up to 100 presses)
    for x_A, x_B in product(range(101), repeat=2):
        if a * x_A + b * x_B == px and c * x_A + d * x_B == py:
            cost = 3 * x_A + x_B
            if cost < min_cost:
                min_cost = cost
                solution = (x_A, x_B)

    return min_cost if solution else None

def solve_claw_machines(machines):
    total_cost = 0
    prizes_won = 0

    for machine in machines:
        a, b, c, d, px, py = machine
        result = find_min_tokens(a, b, c, d, px, py)

        if result is not None:
            prizes_won += 1
            total_cost += result

    return prizes_won, total_cost

def read_machines_from_file(filename):
    machines = []
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]

    # Ensure number of lines is a multiple of 3
    if len(lines) % 3 != 0:
        raise ValueError("Input file does not have complete sets of 3 lines per machine.")

    for i in range(0, len(lines), 3):
        try:
            button_a_line = lines[i]
            button_b_line = lines[i + 1]
            prize_line = lines[i + 2]

            # Extract values using regular expressions
            a_match = re.search(r"X\+(\d+)", button_a_line)
            c_match = re.search(r"Y\+(\d+)", button_a_line)
            b_match = re.search(r"X\+(\d+)", button_b_line)
            d_match = re.search(r"Y\+(\d+)", button_b_line)
            px_match = re.search(r"X=(\d+)", prize_line)
            py_match = re.search(r"Y=(\d+)", prize_line)

            if not (a_match and c_match and b_match and d_match and px_match and py_match):
                raise ValueError("Missing or invalid values in one of the lines.")

            a = int(a_match.group(1))
            c = int(c_match.group(1))
            b = int(b_match.group(1))
            d = int(d_match.group(1))
            px = int(px_match.group(1))
            py = int(py_match.group(1))

            machines.append((a, b, c, d, px, py))
        except Exception as e:
            raise ValueError(f"Invalid line format in input file around lines {i}-{i+2}: {e}")

    return machines


# Input data from file
machines = read_machines_from_file('input.txt')

# Solve
prizes_won, total_cost = solve_claw_machines(machines)

print(f"Prizes won: {prizes_won}")
print(f"Total cost: {total_cost}")
