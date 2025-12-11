def parse_schematic(schematic):
    """Parses a schematic and returns a list of pin heights."""
    heights = []
    for i in range(len(schematic[0])):
        height = -1
        for row in schematic:
            if row[i] == '#':
                height += 1
        heights.append(height)
    return heights

def check_compatibility(lock, key):
    """Checks if a lock and key are compatible."""
    for lock_height, key_height in zip(lock, key):
        if lock_height + key_height > 5:
            return False
    return True

def count_compatible_pairs(locks, keys):
    """Counts the number of compatible lock-key pairs."""
    count = 0
    for lock in locks:
        for key in keys:
            if check_compatibility(lock, key):
                count += 1
    return count

def read_input(filename):
    """Reads schematics from the input file."""
    with open(filename, 'r') as f:
        lines = f.readlines()

    locks = []
    keys = []
    current_schematic = []
    for line in lines:
        line = line.strip()
        if line:
            current_schematic.append(line)
        else:
            if current_schematic:
                if current_schematic[0][0] == '.':  # Check if it's a key
                    keys.append(parse_schematic(current_schematic))
                else:
                    locks.append(parse_schematic(current_schematic))
            current_schematic = []
    if current_schematic:  # Handle the last schematic
        if current_schematic[0][0] == '.':
            keys.append(parse_schematic(current_schematic))
        else:
            locks.append(parse_schematic(current_schematic))

    return locks, keys

if __name__ == "__main__":
    locks, keys = read_input("data.txt")
    # print("lock pins:", locks)
    # print("key pins:", keys)
    num_pairs = count_compatible_pairs(locks, keys)
    print("Number of compatible lock-key pairs:", num_pairs)