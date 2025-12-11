#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000
#define TARGET_LEN 12

// Function to compute max joltage for one bank (largest subsequence of length 12)
void max_joltage(const char *bank, char *result) {
    int len = strlen(bank);
    int to_remove = len - TARGET_LEN;  // how many digits we can drop
    char stack[MAX_LINE];
    int top = 0;

    for (int i = 0; i < len; i++) {
        char c = bank[i];
        while (top > 0 && stack[top-1] < c && to_remove > 0) {
            top--;
            to_remove--;
        }
        stack[top++] = c;
    }

    // If we still need to remove digits, trim from the end
    top -= to_remove;

    // Copy first 12 digits into result
    for (int i = 0; i < TARGET_LEN; i++) {
        result[i] = stack[i];
    }
    result[TARGET_LEN] = '\0';
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
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        char result[TARGET_LEN+1];
        max_joltage(line, result);

        long long val = atoll(result);
        total += val;
    }

    fclose(fp);

    printf("Total output joltage (12 digits): %lld\n", total);
    return 0;
}
