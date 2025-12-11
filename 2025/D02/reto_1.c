#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 10000

// Function to check if a number is invalid (digits repeated twice)
int is_invalid_id(long long num) {
    char buf[32];
    sprintf(buf, "%lld", num);
    int len = strlen(buf);

    // Must have even length
    if (len % 2 != 0) return 0;

    int half = len / 2;
    // Compare first half and second half
    for (int i = 0; i < half; i++) {
        if (buf[i] != buf[i + half]) return 0;
    }
    return 1;
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

    printf("Sum of invalid IDs: %lld\n", total);
    return 0;
}
