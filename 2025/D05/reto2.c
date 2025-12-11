#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 10000

typedef struct {
    long long start;
    long long end;
} Range;

int cmp(const void *a, const void *b) {
    Range *r1 = (Range *)a;
    Range *r2 = (Range *)b;
    if (r1->start < r2->start) return -1;
    if (r1->start > r2->start) return 1;
    return 0;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error opening input.txt");
        return 1;
    }

    Range ranges[MAX_RANGES];
    int range_count = 0;
    char line[256];

    // Read ranges until blank line
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) break; // stop at blank line

        long long start, end;
        if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
            ranges[range_count].start = start;
            ranges[range_count].end = end;
            range_count++;
        }
    }
    fclose(fp);

    if (range_count == 0) {
        printf("No ranges found.\n");
        return 0;
    }

    // Sort ranges by start
    qsort(ranges, range_count, sizeof(Range), cmp);

    // Merge ranges
    long long total = 0;
    long long cur_start = ranges[0].start;
    long long cur_end = ranges[0].end;

    for (int i = 1; i < range_count; i++) {
        if (ranges[i].start <= cur_end + 1) {
            // Overlaps or touches current range
            if (ranges[i].end > cur_end) {
                cur_end = ranges[i].end;
            }
        } else {
            // Finish current range
            total += (cur_end - cur_start + 1);
            cur_start = ranges[i].start;
            cur_end = ranges[i].end;
        }
    }
    // Add last range
    total += (cur_end - cur_start + 1);

    printf("Total fresh ingredient IDs: %lld\n", total);
    return 0;
}
