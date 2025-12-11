#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 2000   // adjust if needed

typedef struct {
    long x, y, z;
} Point;

typedef struct {
    int u, v;
    long long dist_sq;
} Edge;

int parent[MAX_NODES];
int size[MAX_NODES];

// --- Union-Find (Disjoint Set Union) ---
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

// --- Utility ---
long long dist_sq(Point p1, Point p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    long long dz = p1.z - p2.z;
    return dx*dx + dy*dy + dz*dz;
}

int compare_edges(const void *a, const void *b) {
    const Edge *ea = (const Edge*)a;
    const Edge *eb = (const Edge*)b;
    if (ea->dist_sq < eb->dist_sq) return -1;
    if (ea->dist_sq > eb->dist_sq) return 1;
    // optional tie-breaker
    if (ea->u != eb->u) return (ea->u < eb->u) ? -1 : 1;
    if (ea->v != eb->v) return (ea->v < eb->v) ? -1 : 1;
    return 0;
}

// --- Solver ---
void solve(FILE *fp) {
    Point nodes[MAX_NODES];
    int num_nodes = 0;

    while (fscanf(fp, "%ld,%ld,%ld", &nodes[num_nodes].x,
                  &nodes[num_nodes].y,
                  &nodes[num_nodes].z) == 3) {
        num_nodes++;
        if (num_nodes >= MAX_NODES) break;
    }

    if (num_nodes < 2) {
        printf("Error: need at least 2 nodes.\n");
        return;
    }

    int max_edges = num_nodes * (num_nodes - 1) / 2;
    Edge *edges = (Edge*)malloc(max_edges * sizeof(Edge));
    if (!edges) { perror("malloc edges"); return; }

    int edge_count = 0;
    for (int i = 0; i < num_nodes; i++) {
        for (int j = i+1; j < num_nodes; j++) {
            edges[edge_count].u = i;
            edges[edge_count].v = j;
            edges[edge_count].dist_sq = dist_sq(nodes[i], nodes[j]);
            edge_count++;
        }
    }

    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    dsu_init(num_nodes);
    int components = num_nodes;
    int last_u = -1, last_v = -1;

    for (int i = 0; i < edge_count; i++) {
        if (components == 1) break;
        if (dsu_union(edges[i].u, edges[i].v)) {
            components--;
            last_u = edges[i].u;
            last_v = edges[i].v;
        }
    }

    free(edges);

    if (last_u == -1 || last_v == -1) {
        printf("Error: no final connection found.\n");
        return;
    }

    long long product = (long long)nodes[last_u].x * (long long)nodes[last_v].x;

    printf("Total nodes: %d\n", num_nodes);
    printf("Final connection between (%ld,%ld,%ld) and (%ld,%ld,%ld)\n",
           nodes[last_u].x, nodes[last_u].y, nodes[last_u].z,
           nodes[last_v].x, nodes[last_v].y, nodes[last_v].z);
    printf("Product of X coordinates: %lld\n", product);
}

int main() {
    solve(stdin);
    return 0;
}
