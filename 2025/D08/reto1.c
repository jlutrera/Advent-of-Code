#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of junction boxes (nodes) in the puzzle input
#define MAX_NODES 1000

// Number of shortest connections to attempt
// Use 10 for the example; set to 1000 for the real input.
#define MAX_CONNECTIONS 1000

typedef struct {
    long x, y, z;
} Point;

typedef struct {
    int u, v;
    long long dist_sq;
} Edge;

int parent[MAX_NODES];
int size[MAX_NODES];

long long dist_sq(Point p1, Point p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    long long dz = p1.z - p2.z;
    return dx * dx + dy * dy + dz * dz;
}

int compare_edges(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    if (ea->dist_sq < eb->dist_sq) return -1;
    if (ea->dist_sq > eb->dist_sq) return 1;
    // Optional deterministic tie-breaker:
    if (ea->u != eb->u) return (ea->u < eb->u) ? -1 : 1;
    if (ea->v != eb->v) return (ea->v < eb->v) ? -1 : 1;
    return 0;
}

void dsu_init(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
}

int dsu_find(int i) {
    if (parent[i] != i) parent[i] = dsu_find(parent[i]);
    return parent[i];
}

int dsu_union(int i, int j) {
    int ri = dsu_find(i);
    int rj = dsu_find(j);
    if (ri == rj) return 0;
    if (size[ri] < size[rj]) {
        parent[ri] = rj;
        size[rj] += size[ri];
    } else {
        parent[rj] = ri;
        size[ri] += size[rj];
    }
    return 1;
}

void solve(FILE *fp) {
    Point nodes[MAX_NODES];
    int num_nodes = 0;

    while (fscanf(fp, "%ld,%ld,%ld", &nodes[num_nodes].x, &nodes[num_nodes].y, &nodes[num_nodes].z) == 3) {
        num_nodes++;
        if (num_nodes >= MAX_NODES) break;
    }

    if (num_nodes < 2) {
        printf("Error: Need at least 2 nodes.\n");
        return;
    }

    int max_edges = num_nodes * (num_nodes - 1) / 2;
    Edge *edges = (Edge *)malloc(max_edges * sizeof(Edge));
    if (!edges) { perror("edges malloc"); return; }

    int edge_count = 0;
    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            edges[edge_count].u = i;
            edges[edge_count].v = j;
            edges[edge_count].dist_sq = dist_sq(nodes[i], nodes[j]);
            edge_count++;
        }
    }

    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    dsu_init(num_nodes);

    // Process exactly the first K edges (attempts), even if union is a no-op.
    int attempts = (edge_count < MAX_CONNECTIONS) ? edge_count : MAX_CONNECTIONS;
    int unions_done = 0;
    for (int i = 0; i < attempts; i++) {
        unions_done += dsu_union(edges[i].u, edges[i].v);
    }

    free(edges);

    long long top[3] = {0, 0, 0};
    for (int i = 0; i < num_nodes; i++) {
        if (parent[i] == i) {
            long long s = size[i];
            if (s > top[0]) { top[2] = top[1]; top[1] = top[0]; top[0] = s; }
            else if (s > top[1]) { top[2] = top[1]; top[1] = s; }
            else if (s > top[2]) { top[2] = s; }
        }
    }

    long long result = top[0] * top[1] * top[2];

    printf("Total nodes: %d\n", num_nodes);
    printf("Connections attempted (shortest edges): %d\n", attempts);
    printf("Successful unions: %d\n", unions_done);
    printf("Sizes of the three largest circuits: %lld, %lld, %lld\n", top[0], top[1], top[2]);
    printf("Result (product of the three largest sizes): %lld\n", result);
}
 
int main() {
    solve(stdin);
    return 0;
}
