from itertools import product

def evaluate_expression(nums, ops):
    """
    Evaluate the expression with given numbers and operators in left-to-right order.
    """
    result = nums[0]
    for i in range(len(ops)):
        if ops[i] == '+':
            result += nums[i + 1]
        elif ops[i] == '*':
            result *= nums[i + 1]
    return result

def parse_and_solve(file_path):
    """
    Parse the input data from a file and calculate the total calibration result.
    """
    total_calibration_result = 0

    with open(file_path, 'r') as file:
        for line in file:
            target, numbers = line.split(": ")
            target = int(target)
            nums = list(map(int, numbers.split()))
            
            # Generate all combinations of operators
            num_operators = len(nums) - 1
            operator_combinations = product("+*", repeat=num_operators)
            
            # Check if any combination produces the target
            valid = False
            for ops in operator_combinations:
                if evaluate_expression(nums, ops) == target:
                    print(f"{nums} {ops} = {target}")
                    valid = True
                    break
            
            if valid:
                total_calibration_result += target
    
    return total_calibration_result

# Solve using the provided file
file_path = "data.txt"
print(parse_and_solve(file_path))
