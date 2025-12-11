#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 2000
#define MAX_COLS 2000

char manifold[MAX_ROWS][MAX_COLS];
int R, C;

// Memoization table: -1 means not computed yet
long long memo[MAX_ROWS][MAX_COLS];

// Safe access: treat missing columns as space
char cell_at(int r, int c) {
    if (r < 0 || r >= R || c < 0 || c >= C) return ' ';
    return manifold[r][c];
}

// Recursive function: number of timelines starting at (row,col)
long long timelines(int row, int col) {
    if (row >= R || col < 0 || col >= C) return 1; // beam exits manifold

    if (memo[row][col] != -1) return memo[row][col];

    // Scan downward until first splitter
    for (int r = row; r < R; r++) {
        char ch = cell_at(r, col);
        if (ch == '^') {
            long long left = timelines(r + 1, col - 1);
            long long right = timelines(r + 1, col + 1);
            memo[row][col] = left + right;
            return memo[row][col];
        }
    }

    // No splitter found below → one terminal timeline
    memo[row][col] = 1;
    return 1;
}

int main(int argc, char *argv[]) {
    const char *filename = "input.txt";
    if (argc > 1) filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open %s\n", filename);
        return 1;
    }

    char line[MAX_COLS+2];
    int row = 0;
    int start_r = -1, start_c = -1;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        int len = strlen(line);
        if (len > C) C = len;
        strncpy(manifold[row], line, len);
        for (int col = 0; col < len; col++) {
            if (line[col] == 'S') {
                start_r = row;
                start_c = col;
            }
        }
        row++;
    }
    fclose(file);
    R = row;

    if (start_r == -1) {
        fprintf(stderr, "Error: no S found\n");
        return 1;
    }

    // Initialize memo table
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            memo[i][j] = -1;

    long long total = timelines(start_r + 1, start_c);
    printf("Total timelines: %lld\n", total);
    return 0;
}
