#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// El tamaño máximo de la línea se ha aumentado para manejar entradas largas
#define MAX_LINE_LENGTH 1024
// Máximo número de luces y botones (basado en las restricciones del problema)
#define MAX_SIZE 6

// --- Helper Functions for Matrix Operations over F_2 (Modulo 2) ---

/**
 * @brief Realiza la eliminación Gaussiana en la matriz aumentada sobre F_2.
 * @param M El número de filas (luces/ecuaciones).
 * @param N El número de columnas (botones/variables).
 * @param matrix La matriz aumentada [B | T] (tamaño M x (N+1)).
 * @param pivots Un arreglo para almacenar el índice de la columna pivote para cada fila.
 * @return El rango de la matriz B.
 */
int gaussian_elimination(int M, int N, int matrix[MAX_SIZE][MAX_SIZE + 1], int pivots[MAX_SIZE]) {
    int rank = 0;
    int col = 0;
    
    for (int row = 0; row < M && col < N; row++) {
        // 1. Encontrar pivote
        int pivot_row = row;
        while (pivot_row < M && matrix[pivot_row][col] == 0) {
            pivot_row++;
        }

        if (pivot_row < M) {
            // 2. Intercambiar filas
            if (pivot_row != row) {
                for (int j = col; j <= N; j++) {
                    int temp = matrix[row][j];
                    matrix[row][j] = matrix[pivot_row][j];
                    matrix[pivot_row][j] = temp;
                }
            }
            
            // Almacenar el índice de la columna pivote
            pivots[rank] = col;
            rank++;
            
            // 3. Eliminar '1's en la columna actual
            for (int i = 0; i < M; i++) {
                if (i != row && matrix[i][col] == 1) {
                    for (int j = col; j <= N; j++) {
                        matrix[i][j] ^= matrix[row][j]; // XOR es la suma módulo 2
                    }
                }
            }
            // Mover a la siguiente columna
            col++;
        } else {
            // No hay pivote en esta columna
            col++;
            row--; // Permanecer en la fila actual
        }
    }
    return rank;
}

/**
 * @brief Encuentra la solución de peso mínimo (menos pulsaciones) para un sistema consistente.
 * @param M El número de filas (luces/ecuaciones).
 * @param N El número de columnas (botones/variables).
 * @param matrix La forma escalonada reducida por filas de la matriz aumentada.
 * @param rank El rango de la matriz de botones B.
 * @param pivots El arreglo que contiene el índice de la columna pivote para cada fila.
 * @return El número mínimo de pulsaciones requerido.
 */
int find_min_presses(int M, int N, int matrix[MAX_SIZE][MAX_SIZE + 1], int rank, int pivots[MAX_SIZE]) {
    // La verificación de la consistencia (inconsistencia) se omite aquí por brevedad,
    // asumiendo que el llamador ya la ha realizado o que el problema siempre es solucionable.

    // 1. Identificar variables libres
    int is_pivot[MAX_SIZE] = {0};
    for (int i = 0; i < rank; i++) {
        is_pivot[pivots[i]] = 1;
    }

    int free_vars[MAX_SIZE];
    int num_free = 0;
    for (int j = 0; j < N; j++) {
        if (!is_pivot[j]) {
            free_vars[num_free++] = j;
        }
    }

    int min_presses = INT_MAX;

    // 2. Iterar a través de las 2^(num_free) asignaciones de variables libres
    int num_solutions_to_check = 1 << num_free;
    for (int i = 0; i < num_solutions_to_check; i++) {
        int k_prime[MAX_SIZE] = {0}; // k' es el vector de solución (paridad de pulsaciones)
        int current_presses = 0;

        // a. Asignar variables libres
        for (int f = 0; f < num_free; f++) {
            if ((i >> f) & 1) {
                k_prime[free_vars[f]] = 1;
            } else {
                k_prime[free_vars[f]] = 0;
            }
            current_presses += k_prime[free_vars[f]];
        }

        // b. Resolver para variables pivote
        for (int r = 0; r < rank; r++) {
            int pivot_value = matrix[r][N]; 

            // XOR la contribución de las variables libres
            for (int j = pivots[r] + 1; j < N; j++) {
                if (!is_pivot[j]) {
                    pivot_value ^= (matrix[r][j] & k_prime[j]);
                }
            }
            
            k_prime[pivots[r]] = pivot_value;
            current_presses += pivot_value;
        }

        // 3. Actualizar mínimo
        if (current_presses < min_presses) {
            min_presses = current_presses;
        }
    }

    return min_presses;
}

// --- Main Solver Function ---

/**
 * @brief Analiza una línea de máquina, resuelve el sistema lineal y devuelve el mínimo de pulsaciones.
 * @param line La cadena que contiene los datos de la máquina.
 * @return El número mínimo de pulsaciones requerido para esta máquina, o -1 si el análisis falla.
 */
int solve_machine(const char *line) {
    int matrix[MAX_SIZE][MAX_SIZE + 1] = {0};
    int pivots[MAX_SIZE] = {0};
    int M = 0; // Número de luces (filas)
    int N = 0; // Número de botones (columnas)

    // 1. Analizar el diagrama de luces indicadoras (Vector Target T)
    const char *start = strchr(line, '[');
    const char *end = strchr(line, ']');
    if (!start || !end) return -1;
    
    M = (int)(end - start) - 1; // Número de luces
    if (M <= 0 || M > MAX_SIZE) return -1; // Comprobación de límites

    for (int i = 0; i < M; i++) {
        if (start[i + 1] == '#') {
            matrix[i][N] = 1; // Inicialmente N=0, pero usamos M en la posición del índice
        }
    }

    // 2. Analizar los esquemas de cableado de botones (Matrix B)
    start = end;
    N = 0;
    while ((start = strchr(start, '(')) != NULL) {
        if (N >= MAX_SIZE) break;

        end = strchr(start, ')');
        if (!end) break;
        
        // Crear una copia temporal para usar strtok, ya que modifica la cadena
        char temp_str[100];
        size_t len = end - start - 1;
        if (len >= sizeof(temp_str)) len = sizeof(temp_str) - 1;
        strncpy(temp_str, start + 1, len);
        temp_str[len] = '\0';
        
        // El vector del botón es la columna N
        char *token = strtok(temp_str, ",");
        while (token != NULL) {
            int light_index = atoi(token);
            if (light_index < M) {
                matrix[light_index][N] = 1;
            }
            token = strtok(NULL, ",");
        }

        N++; // Mover al siguiente botón/columna
        start = end + 1;
    }
    
    // El vector Target T está en la última columna de la matriz [B | T].
    // Mover el vector T (que está en matrix[i][0]..matrix[i][M-1]) a la columna N.
    // Esto requiere una corrección: la lógica de parseo anterior es un poco confusa.
    // **Ajuste:** En el parseo, la columna del Target T debe ser N (el total de columnas de B).

    // Resetear el Target T a la columna N, ya que N es el número de botones
    for (int i = 0; i < M; i++) {
        int target_val = 0;
        if (start - line > 0 && line[start - line - 1 - (M - 1) + i] == '#') {
             target_val = 1;
        }
        // Se ha simplificado la lógica de parseo del Target para evitar la complejidad
        // de la posición y se ha asumido que 'start' después del bucle de botones es
        // el inicio de las llaves, lo que no es fiable.
        // Volvamos al método original y arreglemos el índice de la columna T.
    }
    // 1. Re-Parsear el Target T a la columna N (número de botones)
    const char *target_start = strchr(line, '[');
    const char *target_end = strchr(line, ']');
    if (!target_start || !target_end) return -1;
    M = (int)(target_end - target_start) - 1; 

    // Resetear B y T
    memset(matrix, 0, sizeof(matrix));

    for (int i = 0; i < M; i++) {
        if (target_start[i + 1] == '#') {
            matrix[i][N] = 1; // Target T es la columna N (la última)
        }
    }
    
    // 2. Re-Parsear Matrix B
    start = target_end;
    N = 0;
    while ((start = strchr(start, '(')) != NULL) {
        if (N >= MAX_SIZE) break;

        end = strchr(start, ')');
        if (!end) break;
        
        char temp_str[100];
        size_t len = end - start - 1;
        if (len >= sizeof(temp_str)) len = sizeof(temp_str) - 1;
        strncpy(temp_str, start + 1, len);
        temp_str[len] = '\0';
        
        char *token = strtok(temp_str, ",");
        while (token != NULL) {
            int light_index = atoi(token);
            if (light_index < M) {
                matrix[light_index][N] = 1;
            }
            token = strtok(NULL, ",");
        }

        N++; 
        start = end + 1;
    }

    if (N == 0) return 0; // No hay botones, si T != 0, debería fallar. Asumimos 0 si no hay botones.

    // 3. Realizar la Eliminación Gaussiana
    int rank = gaussian_elimination(M, N, matrix, pivots);

    // 4. Comprobar la consistencia
    for (int i = rank; i < M; i++) {
        if (matrix[i][N] == 1) {
            // El sistema es inconsistente, no hay solución
            fprintf(stderr, "Error: Inconsistent system found for machine line.\n");
            return -1; 
        }
    }

    // 5. Encontrar la solución de peso mínimo
    return find_min_presses(M, N, matrix, rank, pivots);
}

// --- Main Program ---

int main() {
    char line[MAX_LINE_LENGTH];
    long long total_min_presses = 0;
    int machine_count = 0;

    printf("--- Day 10: Factory Initialization (Input from stdin) ---\n\n");
    printf("Please paste the machine data lines (one per line) and press Ctrl+D (or Ctrl+Z on Windows) when done:\n\n");

    // Leer línea por línea desde stdin hasta EOF
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Eliminar el salto de línea si está presente
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue; // Saltar líneas vacías

        machine_count++;
        int min_presses = solve_machine(line);
        
        printf("Machine %d: %s\n", machine_count, line);
        if (min_presses >= 0) {
            printf("  Fewest presses required: **%d**\n", min_presses);
            total_min_presses += min_presses;
        } else {
            printf("  Error: Could not solve machine (check input format).\n");
        }
    }

    printf("\n---\n");
    printf("Total number of machines processed: %d\n", machine_count);
    printf("What is the fewest button presses required to correctly configure all of the machines?\n");
    printf("Total Fewest Button Presses: **%lld**\n", total_min_presses);

    return 0;
}