import re

def extract_and_compute(file_path: str) -> int:
    # Read the content of the file
    with open(file_path, 'r') as file:
        memory = file.read()
    
    # Define the pattern to match valid mul(X,Y) instructions
    pattern = r"mul\(\d{1,3},\d{1,3}\)"
    
    # Find all matches
    matches = re.findall(pattern, memory)
    
    # Initialize the total sum
    total = 0
    
    # Process each match
    for match in matches:
        # Extract the numbers from the match
        numbers = re.findall(r"\d+", match)
        x, y = map(int, numbers)
        total += x * y
    
    return total

# Specify the file path
file_path = 'data.txt'

# Compute the result
result = extract_and_compute(file_path)

print("The sum of all valid multiplications is:", result)
