import re

def extract_and_compute(file_path: str) -> int:
    # Read the content of the file
    with open(file_path, 'r') as file:
        memory = file.read()
    
    # Remove newline characters to handle multi-line input
    memory = memory.replace('\n', '')
    
    # Define patterns for instructions
    instruction_pattern = r"do\(\)|don't\(\)|mul\(\d{1,3},\d{1,3}\)"
    
    # Find all instructions in sequence
    instructions = re.findall(instruction_pattern, memory)
    
    # Initialize state and total sum
    enabled = True
    total = 0
    
    # Process each instruction
    for instruction in instructions:
        if instruction == "do()":
            enabled = True  # Enable mul instructions
        elif instruction == "don't()":
            enabled = False  # Disable mul instructions
        elif "mul(" in instruction and enabled:
            # Extract numbers and compute the product
            numbers = re.findall(r"\d+", instruction)
            x, y = map(int, numbers)
            total += x * y
    
    return total

# Specify the file path
file_path = 'data.txt'

# Compute the result
result = extract_and_compute(file_path)

print("The sum of all valid and enabled multiplications is:", result)
