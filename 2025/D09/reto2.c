#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Definición de estructuras
typedef struct {
    long long x, y;
} Point;

typedef struct {
    int p1_idx;
    int p2_idx;
    long long area;
} Candidate;

// Funciones auxiliares para long long
long long min_ll(long long a, long long b) { return (a < b) ? a : b; }
long long max_ll(long long a, long long b) { return (a > b) ? a : b; }

// Verifica si dos intervalos se solapan estrictamente
bool intervals_overlap_strict(long long a1, long long a2, long long b1, long long b2) {
    long long min_a = min_ll(a1, a2);
    long long max_a = max_ll(a1, a2);
    long long min_b = min_ll(b1, b2);
    long long max_b = max_ll(b1, b2);

    long long inter_min = max_ll(min_a, min_b);
    long long inter_max = min_ll(max_a, max_b);

    return inter_min < inter_max;
}

// Comprueba si el rectángulo es válido
bool is_valid_rectangle(Point p1, Point p2, Point* polygon, int n) {
    long long min_x = min_ll(p1.x, p2.x);
    long long max_x = max_ll(p1.x, p2.x);
    long long min_y = min_ll(p1.y, p2.y);
    long long max_y = max_ll(p1.y, p2.y);

    // 1. COMPROBACIÓN DE INTERSECCIÓN DE BORDES
    // Ningún borde del polígono debe pasar por el medio del rectángulo
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        // Borde Vertical del polígono
        if (a.x == b.x) {
            // Si la X está estrictamente entre los bordes del rectángulo
            if (a.x > min_x && a.x < max_x) {
                // Y hay solapamiento en el eje Y
                if (intervals_overlap_strict(min_y, max_y, a.y, b.y)) {
                    return false;
                }
            }
        }
        // Borde Horizontal del polígono
        else if (a.y == b.y) {
            // Si la Y está estrictamente entre los bordes del rectángulo
            if (a.y > min_y && a.y < max_y) {
                // Y hay solapamiento en el eje X
                if (intervals_overlap_strict(min_x, max_x, a.x, b.x)) {
                    return false;
                }
            }
        }
    }

    // 2. PUNTO EN POLÍGONO (Ray Casting) con el centro del rectángulo
    double mid_x = (double)(min_x + max_x) / 2.0;
    double mid_y = (double)(min_y + max_y) / 2.0;

    int intersections = 0;
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        // Rayo horizontal hacia la derecha
        // Comprobar si el borde cruza la altura Y del punto medio
        if ((a.y > mid_y) != (b.y > mid_y)) {
            // Calcular intersección exacta en X
            double intersectX = (double)(b.x - a.x) * (mid_y - a.y) / (double)(b.y - a.y) + a.x;
            if (mid_x < intersectX) {
                intersections++;
            }
        }
    }

    // Impar = Dentro, Par = Fuera
    return (intersections % 2 != 0);
}

// Función de comparación para qsort (Orden Descendente por Área)
int compareCandidates(const void* a, const void* b) {
    Candidate* c1 = (Candidate*)a;
    Candidate* c2 = (Candidate*)b;
    if (c1->area < c2->area) return 1;
    if (c1->area > c2->area) return -1;
    return 0;
}

int main() {
    // 1. Lectura de puntos con array dinámico
    int capacity = 1000;
    int n = 0;
    Point* tiles = (Point*)malloc(capacity * sizeof(Point));
    
    long long x, y;
    while (scanf("%lld,%lld", &x, &y) == 2) {
        if (n >= capacity) {
            capacity *= 2;
            tiles = (Point*)realloc(tiles, capacity * sizeof(Point));
        }
        tiles[n].x = x;
        tiles[n].y = y;
        n++;
    }

    if (n < 4) {
        printf("Error: Menos de 4 puntos.\n");
        free(tiles);
        return 1;
    }

    printf("Puntos leidos: %d\n", n);

    // 2. Generar candidatos
    // Estimación de tamaño: n*n/2 puede ser grande, usamos realloc si es necesario
    long long cand_capacity = n * 100; 
    if (cand_capacity < 1000) cand_capacity = 1000;
    
    Candidate* candidates = (Candidate*)malloc(cand_capacity * sizeof(Candidate));
    long long cand_count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (tiles[i].x != tiles[j].x && tiles[i].y != tiles[j].y) {
                // Resize si se llena
                if (cand_count >= cand_capacity) {
                    cand_capacity *= 2;
                    candidates = (Candidate*)realloc(candidates, cand_capacity * sizeof(Candidate));
                }
                
                // Área Inclusiva
                long long w = llabs(tiles[i].x - tiles[j].x) + 1;
                long long h = llabs(tiles[i].y - tiles[j].y) + 1;
                
                candidates[cand_count].p1_idx = i;
                candidates[cand_count].p2_idx = j;
                candidates[cand_count].area = w * h;
                cand_count++;
            }
        }
    }

    printf("Candidatos generados: %lld. Ordenando...\n", cand_count);

    // 3. Ordenar (qsort)
    qsort(candidates, cand_count, sizeof(Candidate), compareCandidates);

    printf("Verificando candidatos (de mayor a menor area)...\n");

    // 4. Buscar la solución
    int found = 0;
    for (long long k = 0; k < cand_count; k++) {
        Point p1 = tiles[candidates[k].p1_idx];
        Point p2 = tiles[candidates[k].p2_idx];

        if (is_valid_rectangle(p1, p2, tiles, n)) {
            printf("\n!!! ENCONTRADO !!!\n");
            printf("Area Maxima (Inclusiva): %lld\n", candidates[k].area);
            printf("Esquinas: (%lld,%lld) y (%lld,%lld)\n", p1.x, p1.y, p2.x, p2.y);
            found = 1;
            break; 
        }
    }

    if (!found) {
        printf("No se encontro ningun rectangulo valido.\n");
    }

    // Limpieza
    free(tiles);
    free(candidates);

    return 0;
}