def parse_disk_map(disk_map):
    """Parse the disk map string into a list of file and free space lengths."""
    segments = []
    for i, char in enumerate(disk_map):
        if i % 2 == 0:  # File length
            segments.append((int(char), True))  # (length, is_file)
        else:  # Free space length
            segments.append((int(char), False))  # (length, is_file)
    return segments


def generate_compacted_disk_map(segments):
    """Generate the compacted disk map."""
    # Step 1: Build the initial representation of the disk
    blocks = []
    file_id = 0
    for length, is_file in segments:
        if is_file:
            blocks.extend([file_id] * length)
            file_id += 1
        else:
            blocks.extend(["."] * length)

    # Step 2: Compact the blocks (move one block at a time from end to leftmost free space)
    n = len(blocks)
    for i in range(n - 1, -1, -1):  # Start from the rightmost block
        if blocks[i] != ".":  # If it's a file block
            # Find the first free space from the left
            for j in range(n):
                if blocks[j] == ".":  # Found free space
                    # Move the block to the free space
                    blocks[j] = blocks[i]
                    blocks[i] = "."  # Leave a free space behind
                    break  # Stop searching and move to the next block
    # Ensure there are no leading empty spaces (if needed)
    while blocks[0] == ".":
        blocks.pop(0)
        blocks.append(".")

    return blocks


def calculate_checksum(compacted_blocks):
    """Calculate the checksum for the compacted disk."""
    checksum = 0
    for position, block in enumerate(compacted_blocks):
        if block != ".":  # Only consider file blocks
            checksum += position * block
    return checksum


def process_disk_map(file_path):
    """Process the disk map from the input file."""
    with open(file_path, 'r') as file:
        disk_map = file.read().strip()  # Read and strip any extra whitespace
    segments = parse_disk_map(disk_map)
    compacted_blocks = generate_compacted_disk_map(segments)
    checksum = calculate_checksum(compacted_blocks)
    return checksum


# Ejecución principal
if __name__ == "__main__":
    file_path = "data.txt"  # Nombre del archivo de entrada
    checksum = process_disk_map(file_path)
    print(f"Checksum: {checksum}")
