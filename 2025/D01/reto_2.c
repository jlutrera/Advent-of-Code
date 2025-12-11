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
    long long zero_count = 0;  // count how many times dial points at 0

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        char direction = line[0];        // 'L' or 'R'
        int distance = atoi(line + 1);   // number after L/R

        if (direction == 'L') {
            // Each click moves left (decreasing position)
            for (int i = 0; i < distance; i++) {
                position--;
                if (position < 0) position = 99;
                if (position == 0) zero_count++;
            }
        } else if (direction == 'R') {
            // Each click moves right (increasing position)
            for (int i = 0; i < distance; i++) {
                position++;
                if (position > 99) position = 0;
                if (position == 0) zero_count++;
            }
        } else {
            fprintf(stderr, "Invalid instruction: %s\n", line);
            continue;
        }
    }

    fclose(fp);

    printf("Password (method 0x434C49434B): %lld\n", zero_count);
    return 0;
}
