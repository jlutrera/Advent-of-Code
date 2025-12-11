#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TILES 5000   // adjust if needed

typedef struct {
    int x, y;
} Tile;

/**
 * Compute rectangle area given two opposite corners.
 * Formula uses inclusive coordinates: (|x1 - x2| + 1) * (|y1 - y2| + 1)
 */
long long rect_area(Tile a, Tile b) {
    long long width  = llabs(a.x - b.x) + 1;
    long long height = llabs(a.y - b.y) + 1;
    return width * height;
}

void solve(FILE *fp) {
    Tile tiles[MAX_TILES];
    int n = 0;

    // Read coordinates: "x,y" per line
    while (fscanf(fp, "%d,%d", &tiles[n].x, &tiles[n].y) == 2) {
        n++;
        if (n >= MAX_TILES) break;
    }

    if (n < 2) {
        printf("Error: need at least 2 red tiles.\n");
        return;
    }

    long long max_area = 0;
    Tile bestA, bestB;

    // Check all pairs
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long area = rect_area(tiles[i], tiles[j]);
            if (area > max_area) {
                max_area = area;
                bestA = tiles[i];
                bestB = tiles[j];
            }
        }
    }

    printf("Total red tiles: %d\n", n);
    printf("Largest rectangle area: %lld\n", max_area);
    printf("Corners used: (%d,%d) and (%d,%d)\n",
           bestA.x, bestA.y, bestB.x, bestB.y);
}

int main() {
    // Run solver on stdin
    solve(stdin);
    return 0;
}
