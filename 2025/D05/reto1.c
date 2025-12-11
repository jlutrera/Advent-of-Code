#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 10000
#define MAX_IDS    10000

typedef struct {
    long long start;
    long long end;
} Range;

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error opening input.txt");
        return 1;
    }

    Range ranges[MAX_RANGES];
    int range_count = 0;

    long long ids[MAX_IDS];
    int id_count = 0;

    char line[256];
    int reading_ranges = 1;

    while (fgets(line, sizeof(line), fp)) {
        // Strip newline
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            // Blank line: switch to reading IDs
            reading_ranges = 0;
            continue;
        }

        if (reading_ranges) {
            long long start, end;
            if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
                ranges[range_count].start = start;
                ranges[range_count].end = end;
                range_count++;
            }
        } else {
            long long id;
            if (sscanf(line, "%lld", &id) == 1) {
                ids[id_count++] = id;
            }
        }
    }
    fclose(fp);

    int fresh_count = 0;

    for (int i = 0; i < id_count; i++) {
        long long id = ids[i];
        int fresh = 0;
        for (int j = 0; j < range_count; j++) {
            if (id >= ranges[j].start && id <= ranges[j].end) {
                fresh = 1;
                break;
            }
        }
        if (fresh) fresh_count++;
    }

    printf("Number of fresh ingredient IDs: %d\n", fresh_count);
    return 0;
}
