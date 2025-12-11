def run_program(initial_a, initial_b, initial_c, program):
    # Initialize registers
    A, B, C = initial_a, initial_b, initial_c
    output = []
    ip = 0  # Instruction pointer

    # Helper function to evaluate combo operands
    def get_combo_value(operand):
        if operand == 4:
            return A
        elif operand == 5:
            return B
        elif operand == 6:
            return C
        elif operand < 4:
            return operand
        else:
            raise ValueError("Invalid combo operand")

    # Main execution loop
    while ip < len(program):
        opcode = program[ip]
        operand = program[ip + 1] if ip + 1 < len(program) else None

        if opcode == 0:  # adv: A //= 2^operand
            denom = 2 ** get_combo_value(operand)
            A //= denom
        elif opcode == 1:  # bxl: B ^= literal operand
            B ^= operand
        elif opcode == 2:  # bst: B = combo operand % 8
            B = get_combo_value(operand) % 8
        elif opcode == 3:  # jnz: if A != 0, jump to literal operand
            if A != 0:
                ip = operand
                continue
        elif opcode == 4:  # bxc: B ^= C (operand ignored)
            B ^= C
        elif opcode == 5:  # out: output combo operand % 8
            output.append(get_combo_value(operand) % 8)
        elif opcode == 6:  # bdv: B = A // 2^operand
            denom = 2 ** get_combo_value(operand)
            B = A // denom
        elif opcode == 7:  # cdv: C = A // 2^operand
            denom = 2 ** get_combo_value(operand)
            C = A // denom
        else:
            raise ValueError(f"Invalid opcode: {opcode}")

        # Move to the next instruction
        ip += 2

    # Return the collected output as a comma-separated string
    return ",".join(map(str, output))

# Example input
initial_a = 17323786
initial_b = 0
initial_c = 0
program = [2,4,1,1,7,5,1,5,4,1,5,5,0,3,3,0]

# Run the program
result = run_program(initial_a, initial_b, initial_c, program)
print(result)
