#include <stdio.h>
#include <string.h>

#define MAX_ROWS 2000
#define MAX_COLS 2000

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error opening input.txt");
        return 1;
    }

    char grid[MAX_ROWS][MAX_COLS + 1];
    int rows = 0, cols = -1;

    while (rows < MAX_ROWS && fgets(grid[rows], sizeof(grid[rows]), fp)) {
        size_t len = strlen(grid[rows]);
        if (len > 0 && grid[rows][len - 1] == '\n') grid[rows][len - 1] = '\0';
        if (cols == -1) cols = (int)strlen(grid[rows]);
        rows++;
    }
    fclose(fp);

    if (rows == 0 || cols <= 0) {
        fprintf(stderr, "No input grid provided.\n");
        return 1;
    }

    int dx[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

    long long total_removed = 0;
    int changed = 1;

    while (changed) {
        changed = 0;
        char to_remove[MAX_ROWS][MAX_COLS];   // much smaller footprint
        memset(to_remove, 0, sizeof(to_remove));

        // Find accessible rolls
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] != '@') continue;

                int adj = 0;
                for (int k = 0; k < 8; k++) {
                    int nr = r + dx[k], nc = c + dy[k];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (grid[nr][nc] == '@') adj++;
                    }
                }
                if (adj < 4) {
                    to_remove[r][c] = 1;
                }
            }
        }

        // Remove marked rolls
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (to_remove[r][c]) {
                    grid[r][c] = '.';
                    total_removed++;
                    changed = 1;
                }
            }
        }
    }

    printf("Total rolls removed: %lld\n", total_removed);
    return 0;
}
