#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum size for the manifold
#define MAX_ROWS 200 
#define MAX_COLS 200

// Global State
int total_splits = 0;
char manifold[MAX_ROWS][MAX_COLS];
int R; // Number of rows (determined from input)
int C; // Number of columns (determined from input)

// State tracking array for memoization.
int is_split_point[MAX_ROWS][MAX_COLS] = {0}; 
int visited_start[MAX_ROWS][MAX_COLS] = {0};

/**
 * @brief Simulates the downward movement of a single tachyon beam.
 *
 * @param r The starting row for the downward search.
 * @param c The column the beam is traveling in.
 */
void simulate_beam(int r, int c) {
    if (r >= R || c < 0 || c >= C) return;

    if (visited_start[r][c]) return;
    visited_start[r][c] = 1;

   for (int next_r = r; next_r < R; next_r++) {
        if (c < 0 || c >= C) return;
        char current_cell = manifold[next_r][c];

        if (current_cell == '^') {
            if (is_split_point[next_r][c]) return;
            is_split_point[next_r][c] = 1;
            total_splits++;
            simulate_beam(next_r + 1, c - 1);
            simulate_beam(next_r + 1, c + 1);
            
            return;
        }
    }
}

/**
 * @brief Reads the manifold from the specified file. (Unchanged)
 */
int read_manifold(const char *filename, int *start_r, int *start_c) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'.\n", filename);
        return 0;
    }

    char line[MAX_COLS + 2]; 
    int row = 0;
    
    while (row < MAX_ROWS && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        if (row == 0) {
            C = strlen(line);
        } else if (strlen(line) != C) {
            fprintf(stderr, "Error: Manifold rows have inconsistent length in file '%s'.\n", filename);
            fclose(file);
            return 0;
        }

        strncpy(manifold[row], line, C);
        for (int col = 0; col < C; col++) {
            if (manifold[row][col] == 'S') {
                *start_r = row;
                *start_c = col;
            }
        }
        
        row++;
    }
    
    R = row;
    fclose(file);

    if (*start_r == -1) {
        fprintf(stderr, "Error: Starting position 'S' not found in the manifold.\n");
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    int start_r = -1;
    int start_c = -1;
    const char *filename = "input.txt";

    if (argc > 1) {
        filename = argv[1];
    }

    if (!read_manifold(filename, &start_r, &start_c)) {
        return 1;
    }

    // The beam starts its simulation at the cell immediately below 'S'.
    simulate_beam(start_r + 1, start_c);

    printf("The total number of times the tachyon beam will be split is: %d\n", total_splits);

    return 0;
}