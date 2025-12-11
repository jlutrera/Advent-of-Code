from itertools import combinations
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

# Find all triangles
triangles = []
for a, b, c in combinations(graph.keys(), 3):
    if b in graph[a] and c in graph[a] and c in graph[b]:
        triangles.append({a, b, c})

# Filter triangles containing at least one node starting with 't'
triangles_with_t = [triangle for triangle in triangles if any(node.startswith('t') for node in triangle)]

# Output the results
print(f"Total triangles with at least one 't': {len(triangles_with_t)}")
print("Triangles:", triangles_with_t)
