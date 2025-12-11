#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char op;
    unsigned long long *numbers;
    int count;
} Problem;

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error abriendo input.txt");
        return 1;
    }

    // Leer todas las líneas
    char **grid = NULL;
    int rows = 0, cols = 0;
    char buf[10000];
    while (fgets(buf, sizeof(buf), fp)) {
        size_t len = strlen(buf);
        if (len && buf[len-1] == '\n') buf[len-1] = '\0';
        if ((int)len > cols) cols = (int)len;
        grid = realloc(grid, (rows+1)*sizeof(char*));
        grid[rows] = malloc(len+1);
        strcpy(grid[rows], buf);
        rows++;
    }
    fclose(fp);

    // Encontrar fila de operadores
    int op_row = -1;
    for (int r = rows-1; r >= 0; r--) {
        for (int c = 0; c < (int)strlen(grid[r]); c++) {
            if (grid[r][c] == '+' || grid[r][c] == '*') {
                op_row = r;
                break;
            }
        }
        if (op_row != -1) break;
    }
    if (op_row == -1) {
        fprintf(stderr, "No se encontró fila de operadores.\n");
        return 1;
    }

    __int128 grand_total = 0;

    // Recorrer de derecha a izquierda
    int c = cols-1;
    while (c >= 0) {
        // Saltar espacios
        int all_space;
        do {
            all_space = 1;
            for (int r = 0; r <= op_row; r++) {
                if (c < (int)strlen(grid[r]) && grid[r][c] != ' ' && grid[r][c] != '\0') {
                    all_space = 0;
                    break;
                }
            }
            if (all_space) c--;
        } while (all_space && c >= 0);
        if (c < 0) break;

        // Encontrar inicio del bloque
        int end = c;
        while (c >= 0) {
            all_space = 1;
            for (int r = 0; r <= op_row; r++) {
                if (c < (int)strlen(grid[r]) && grid[r][c] != ' ' && grid[r][c] != '\0') {
                    all_space = 0;
                    break;
                }
            }
            if (all_space) break;
            c--;
        }
        int start = c+1;

        // Extraer operador
        char op = '?';
        for (int cc = start; cc <= end; cc++) {
            if (cc < (int)strlen(grid[op_row])) {
                char ch = grid[op_row][cc];
                if (ch == '+' || ch == '*') { op = ch; break; }
            }
        }
        if (op == '?') continue;

        // Extraer números columna por columna
        unsigned long long *nums = NULL;
        int num_count = 0;
        for (int cc = start; cc <= end; cc++) {
            char numbuf[64];
            int bi = 0;
            for (int r = 0; r < op_row; r++) {
                if (cc < (int)strlen(grid[r]) && isdigit((unsigned char)grid[r][cc])) {
                    numbuf[bi++] = grid[r][cc];
                }
            }
            numbuf[bi] = '\0';
            if (bi > 0) {
                nums = realloc(nums, (num_count+1)*sizeof(unsigned long long));
                nums[num_count++] = strtoull(numbuf, NULL, 10);
            }
        }

        // Evaluar problema
        __int128 result = (op == '+') ? 0 : 1;
        for (int i = 0; i < num_count; i++) {
            if (op == '+') result += nums[i];
            else result *= nums[i];
        }
        grand_total += result;
        free(nums);
    }

    // Imprimir __int128
    if (grand_total == 0) {
        printf("Grand total: 0\n");
    } else {
        char out[256];
        int oi = 0;
        __int128 x = grand_total;
        char tmp[256];
        int ti = 0;
        while (x > 0) {
            tmp[ti++] = '0' + (int)(x % 10);
            x /= 10;
        }
        for (int i = ti-1; i >= 0; i--) out[oi++] = tmp[i];
        out[oi] = '\0';
        printf("Grand total: %s\n", out);
    }

    for (int r = 0; r < rows; r++) free(grid[r]);
    free(grid);

    return 0;
}
