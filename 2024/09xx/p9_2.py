def parse_disk_map(disk_map):
    """Parse the disk map string into a list of file and free space lengths."""
    segments = []
    for i, char in enumerate(disk_map):
        if i % 2 == 0:  # File length
            segments.append((int(char), True))  # (length, is_file)
        else:  # Free space length
            segments.append((int(char), False))  # (length, is_file)
    return segments


def measure_free_spaces(blocks):
    """Identify and measure free spaces in the disk map."""
    free_spaces = []
    start = None
    for i, block in enumerate(blocks):
        if block == ".":
            if start is None:
                start = i
        else:
            if start is not None:
                free_spaces.append((start, i - start))  # (start, length)
                start = None
    if start is not None:  # Handle trailing free space
        free_spaces.append((start, len(blocks) - start))
    return free_spaces

def print_disk(blocks):
	"""Print the disk map in a human-readable format."""
	for i, block in enumerate(blocks):
		print(block, end="")
	print("\n")

def compact_disk(segments):
    """Compact the disk by moving whole files to the first available free space."""
    # Step 1: Build the initial representation of the disk
    blocks = []
    file_positions = []  # Track start and end of each file
    file_id = 0
    for length, is_file in segments:
        if is_file:
            blocks.extend([file_id] * length)
            file_positions.append((file_id, len(blocks) - length, len(blocks) - 1))
            file_id += 1
        else:
            blocks.extend(["."] * length)

    # print("I = ", end=" ")
    # print_disk(blocks)   
    # Step 2: Process files in decreasing order of file ID
    for file_id, start, end in sorted(file_positions, key=lambda x: -x[0]):
        file_length = end - start + 1
        free_spaces = measure_free_spaces(blocks)  # Measure free spaces dynamically
        moved = False
        for free_start, free_length in free_spaces:
            if free_length >= file_length and free_start < start:
                # Move the file to this free space
                blocks[start:end + 1] = ["."] * file_length  # Clear original position
                blocks[free_start:free_start + file_length] = [file_id] * file_length
                moved = True
                break  # Stop searching for this file
        if not moved:
            continue  # File stays in its original position if no space is found
        # print(f"{file_id} = ", end="")
        # print_disk(blocks)
    return blocks


def calculate_checksum(blocks):
    """Calculate the checksum for the compacted disk."""
    checksum = 0
    for position, block in enumerate(blocks):
        if block != ".":
            checksum += position * block
    return checksum


def process_disk_map(file_path):
    """Process the disk map from the input file."""
    with open(file_path, 'r') as file:
        disk_map = file.read().strip()
    segments = parse_disk_map(disk_map)
    compacted_blocks = compact_disk(segments)
    checksum = calculate_checksum(compacted_blocks)
    return checksum


# Ejecución principal
if __name__ == "__main__":
    file_path = "data.txt"  # Nombre del archivo de entrada
    checksum = process_disk_map(file_path)
    print(f"Checksum: {checksum}")
