from collections import defaultdict

# Read input from file
with open("data.txt", "r") as file:
    input_data = file.read().strip()

# Build the adjacency list
graph = defaultdict(set)
for line in input_data.splitlines():
    a, b = line.split('-')
    graph[a].add(b)
    graph[b].add(a)

# Bron–Kerbosch algorithm to find all maximal cliques
def bron_kerbosch(r, p, x, graph, cliques):
    if not p and not x:  # Base case: no candidates left
        cliques.append(r)
        return
    for v in list(p):
        bron_kerbosch(r | {v}, p & graph[v], x & graph[v], graph, cliques)
        p.remove(v)
        x.add(v)

# Find all maximal cliques
def find_maximal_cliques(graph):
    cliques = []
    nodes = set(graph.keys())
    bron_kerbosch(set(), nodes, set(), graph, cliques)
    return cliques

# Get all cliques and find the largest one
cliques = find_maximal_cliques(graph)
largest_clique = max(cliques, key=len)

# Generate the password
password = ",".join(sorted(largest_clique))

# Output the result
print(f"The password to get into the LAN party is: {password}")
