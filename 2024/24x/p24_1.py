# Simulación de un sistema de puertas lógicas

def parse_input(file_name):
    """Parses the input data from a file into initial wire values and gate connections."""
    initial_values = {}
    gates = []

    with open(file_name, 'r') as file:
        for line in file:
            if ":" in line:
                # Parse initial values
                wire, value = line.split(":")
                initial_values[wire.strip()] = int(value.strip())
            elif "->" in line:
                # Parse gates
                parts = line.split("->")
                output_wire = parts[1].strip()
                operation = parts[0].strip().split()
                gates.append((operation, output_wire))

    return initial_values, gates

def evaluate_gate(operation, wire_values):
    """Evaluates a logic gate operation."""
    if len(operation) == 3:
        input1, gate, input2 = operation
        val1 = wire_values.get(input1, None)
        val2 = wire_values.get(input2, None)

        if val1 is None or val2 is None:
            return None  # Wait for all inputs to be available

        if gate == "AND":
            return val1 & val2
        elif gate == "OR":
            return val1 | val2
        elif gate == "XOR":
            return val1 ^ val2
    return None

def simulate_circuit(initial_values, gates):
    """Simulates the circuit and calculates wire values."""
    wire_values = initial_values.copy()
    pending_gates = gates.copy()

    while pending_gates:
        remaining_gates = []
        for operation, output_wire in pending_gates:
            result = evaluate_gate(operation, wire_values)
            if result is not None:
                wire_values[output_wire] = result
                print(f"Wire {output_wire} set to {result}")  # Debugging output
            else:
                remaining_gates.append((operation, output_wire))
        pending_gates = remaining_gates

    return wire_values

def calculate_output(wire_values):
    """Calculates the decimal number from the binary values on wires starting with 'z'."""
    z_wires = {wire: value for wire, value in wire_values.items() if wire.startswith("z")}
    sorted_z_wires = dict(sorted(z_wires.items(), key=lambda x: int(x[0][1:])))
    binary_number = "".join(str(sorted_z_wires[wire]) for wire in reversed(sorted_z_wires))
    print("Binary output (reversed):", binary_number)  # Debugging output
    return int(binary_number, 2)

# Input file
file_name = "data.txt"

# Simulate the circuit
initial_values, gates = parse_input(file_name)
wire_values = simulate_circuit(initial_values, gates)
output = calculate_output(wire_values)

print("Decimal output:", output)
