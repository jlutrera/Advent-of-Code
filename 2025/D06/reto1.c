#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char op;              // '+' o '*'
    unsigned long long *numbers;
    int count;
} Problem;

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("Error abriendo input.txt");
        return 1;
    }

    // Leer todas las líneas dinámicamente
    char **grid = NULL;
    int rows = 0;
    int cols = 0;
    char buf[10000];

    while (fgets(buf, sizeof(buf), fp)) {
        size_t len = strlen(buf);
        if (len && buf[len-1] == '\n') buf[len-1] = '\0';
        if ((int)len > cols) cols = (int)len;

        grid = realloc(grid, (rows+1) * sizeof(char*));
        grid[rows] = malloc(len+1);
        strcpy(grid[rows], buf);
        rows++;
    }
    fclose(fp);

    // Buscar la fila de operadores (última que contiene + o *)
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

    // Detectar bloques de problemas
    Problem *problems = NULL;
    int problem_count = 0;

    int c = 0;
    while (c < cols) {
        // Saltar columnas vacías
        int all_space;
        do {
            all_space = 1;
            for (int r = 0; r <= op_row; r++) {
                if (c < (int)strlen(grid[r]) && grid[r][c] != ' ' && grid[r][c] != '\0') {
                    all_space = 0;
                    break;
                }
            }
            if (all_space) c++;
        } while (all_space && c < cols);
        if (c >= cols) break;

        int start = c;
        // Avanzar hasta fin del bloque
        while (c < cols) {
            all_space = 1;
            for (int r = 0; r <= op_row; r++) {
                if (c < (int)strlen(grid[r]) && grid[r][c] != ' ' && grid[r][c] != '\0') {
                    all_space = 0;
                    break;
                }
            }
            if (all_space) break;
            c++;
        }
        int end = c;

        // Extraer operador
        char op = '?';
        for (int cc = start; cc < end; cc++) {
            if (cc < (int)strlen(grid[op_row])) {
                char ch = grid[op_row][cc];
                if (ch == '+' || ch == '*') { op = ch; break; }
            }
        }
        if (op == '?') continue;

        Problem p;
        p.op = op;
        p.count = 0;
        p.numbers = NULL;

        // Extraer números de filas anteriores
        for (int r = 0; r < op_row; r++) {
            char numbuf[64];
            int bi = 0;
            for (int cc = start; cc < end; cc++) {
                if (cc < (int)strlen(grid[r]) && isdigit((unsigned char)grid[r][cc])) {
                    numbuf[bi++] = grid[r][cc];
                }
            }
            numbuf[bi] = '\0';
            if (bi > 0) {
                p.numbers = realloc(p.numbers, (p.count+1)*sizeof(unsigned long long));
                p.numbers[p.count++] = strtoull(numbuf, NULL, 10);
            }
        }

        problems = realloc(problems, (problem_count+1)*sizeof(Problem));
        problems[problem_count++] = p;
    }

    // Evaluar problemas
    __int128 grand_total = 0;
    for (int i = 0; i < problem_count; i++) {
        Problem *p = &problems[i];
        __int128 result = (p->op == '+') ? 0 : 1;
        for (int j = 0; j < p->count; j++) {
            if (p->op == '+') result += p->numbers[j];
            else result *= p->numbers[j];
        }
        grand_total += result;
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

    // Liberar memoria
    for (int r = 0; r < rows; r++) free(grid[r]);
    free(grid);
    for (int i = 0; i < problem_count; i++) free(problems[i].numbers);
    free(problems);

    return 0;
}
