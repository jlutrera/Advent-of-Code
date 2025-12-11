#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 10000

// Check if string s is repetition of a substring
int is_repetition(const char *s) {
    int len = strlen(s);
    for (int k = 1; k <= len / 2; k++) {
        if (len % k != 0) continue;  // must divide evenly
        int repeats = len / k;
        int valid = 1;
        for (int i = k; i < len; i++) {
            if (s[i] != s[i % k]) {
                valid = 0;
                break;
            }
        }
        if (valid && repeats >= 2) return 1;
    }
    return 0;
}

// Check if number is invalid ID
int is_invalid_id(long long num) {
    char buf[32];
    sprintf(buf, "%lld", num);
    return is_repetition(buf);
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), fp)) {
        perror("Error reading file");
        return 1;
    }
    fclose(fp);

    long long total = 0;

    // Split ranges by comma
    char *range = strtok(line, ",");
    while (range) {
        long long start, end;
        if (sscanf(range, "%lld-%lld", &start, &end) == 2) {
            for (long long id = start; id <= end; id++) {
                if (is_invalid_id(id)) {
                    total += id;
                }
            }
        }
        range = strtok(NULL, ",");
    }

    printf("Sum of invalid IDs (Part Two): %lld\n", total);
    return 0;
}
