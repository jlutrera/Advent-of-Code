#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000

// Function to compute max joltage for one bank
int max_joltage(const char *bank) {
    int len = strlen(bank);
    int max_val = 0;

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            int d1 = bank[i] - '0';
            int d2 = bank[j] - '0';
            int val = d1 * 10 + d2;
            if (val > max_val) {
                max_val = val;
            }
        }
    }
    return max_val;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE];
    long long total = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        int max_val = max_joltage(line);
        total += max_val;
    }

    fclose(fp);

    printf("Total output joltage: %lld\n", total);
    return 0;
}
