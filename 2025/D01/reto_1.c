#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100

int main() {
    FILE *fp = fopen("input.txt", "r");  // puzzle input file
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE];
    int position = 50;   // dial starts at 50
    int zero_count = 0;  // count how many times dial points at 0

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline if present
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) continue;  // skip empty lines

        char direction = line[0];        // 'L' or 'R'
        int distance = atoi(line + 1);   // number after L/R

        if (direction == 'L') {
            position = (position - distance) % 100;
            if (position < 0) position += 100;  // wrap around
        } else if (direction == 'R') {
            position = (position + distance) % 100;
        } else {
            fprintf(stderr, "Invalid instruction: %s\n", line);
            continue;
        }

        if (position == 0) {
            zero_count++;
        }
    }

    fclose(fp);

    printf("Password: %d\n", zero_count);
    return 0;
}
