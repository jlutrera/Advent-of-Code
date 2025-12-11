def count_xmas(grid):
    # Dimensions of the grid
    rows = len(grid)
    cols = len(grid[0])
    
    # Word to search
    word = "XMAS"
    word_len = len(word)
    
    # Directions to check (8 directions: horizontal, vertical, diagonal)
    directions = [
        (0, 1),   # Right
        (1, 0),   # Down
        (1, 1),   # Down-right diagonal
        (-1, 0),  # Up
        (0, -1),  # Left
        (-1, -1), # Up-left diagonal
        (1, -1),  # Down-left diagonal
        (-1, 1)   # Up-right diagonal
    ]
    
    # Helper function to check if a word is found starting from (r, c) in direction (dr, dc)
    def is_word_found(r, c, dr, dc):
        for i in range(word_len):
            nr, nc = r + i * dr, c + i * dc
            if nr < 0 or nc < 0 or nr >= rows or nc >= cols or grid[nr][nc] != word[i]:
                return False
        return True
    
    # Count occurrences of the word
    count = 0
    for r in range(rows):
        for c in range(cols):
            for dr, dc in directions:
                if is_word_found(r, c, dr, dc):
                    count += 1
    
    return count


# Read the grid from the file data.txt
def read_grid_from_file(filename):
    with open(filename, 'r') as file:
        grid = [line.strip() for line in file.readlines()]
    return grid


# Main program
if __name__ == "__main__":
    filename = "data.txt"  # File containing the grid
    grid = read_grid_from_file(filename)  # Read the grid
    result = count_xmas(grid)  # Count occurrences of "XMAS"
    print(f"The word 'XMAS' appears {result} times.")
