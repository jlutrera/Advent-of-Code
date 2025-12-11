def evolve_secret(secret):
    MODULO = 16777216

    # Step 1: Multiply by 64, mix, and prune
    secret = (secret ^ (secret * 64)) % MODULO

    # Step 2: Divide by 32, round down, mix, and prune
    secret = (secret ^ (secret // 32)) % MODULO

    # Step 3: Multiply by 2048, mix, and prune
    secret = (secret ^ (secret * 2048)) % MODULO

    return secret

def simulate_buyers_secrets(initial_secrets, iterations):
    total_sum = 0

    for initial_secret in initial_secrets:
        secret = initial_secret

        for _ in range(iterations):
            secret = evolve_secret(secret)

        total_sum += secret

    return total_sum

def read_file(filename):
	with open(filename, "r") as file:
		return [int(line) for line in file]

# Example input
initial_secrets = read_file("data.txt")
iterations = 2000

# Calculate the sum of the 2000th secret numbers
result = simulate_buyers_secrets(initial_secrets, iterations)
print("Sum of the 2000th secret numbers:", result)
