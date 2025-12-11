def is_design_possible(towel_patterns, design):
    """
    Determine if the design can be constructed using the available towel patterns.
    """
    def backtrack(remaining_design):
        # Base case: If the remaining design is empty, it is possible
        if not remaining_design:
            return True

        # Try all towel patterns
        for pattern in towel_patterns:
            if remaining_design.startswith(pattern):
                # Recursively check the remainder of the design
                if backtrack(remaining_design[len(pattern):]):
                    return True

        # If no pattern matches, return False
        return False

    return backtrack(design)


def count_possible_designs(towel_patterns, designs):
    """
    Count how many designs can be constructed using the available towel patterns.
    """
    count = 0
    for design in designs:
        if is_design_possible(towel_patterns, design):
            count += 1
    return count

# Read data from files
with open("patterns.txt", "r") as patterns_file:
    towel_patterns = [pattern.strip() for pattern in patterns_file.readline().strip().split(",")]

with open("designs.txt", "r") as designs_file:
    designs = [line.strip() for line in designs_file]

# Debugging: Print parsed data
# print("Towel Patterns:", towel_patterns)
# print("Designs:", designs)

# Calculate the result
possible_designs_count = count_possible_designs(towel_patterns, designs)
print(f"Number of possible designs: {possible_designs_count}")
