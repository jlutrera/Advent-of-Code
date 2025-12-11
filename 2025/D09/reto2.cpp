#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    long long x, y; // Usamos long long por seguridad con coordenadas grandes
};

struct RectCandidate {
    int p1_idx;
    int p2_idx;
    long long area;
};

// Verifica si dos intervalos se solapan estrictamente
// Esto se usa para ver si una pared del polígono cruza "por medio" del rectángulo
bool intervals_overlap_strict(long long a1, long long a2, long long b1, long long b2) {
    long long min_a = min(a1, a2);
    long long max_a = max(a1, a2);
    long long min_b = min(b1, b2);
    long long max_b = max(b1, b2);
    
    // La intersección debe ser un rango válido mayor a 0
    long long inter_min = max(min_a, min_b);
    long long inter_max = min(max_a, max_b);
    
    return inter_min < inter_max;
}

// Comprueba si el rectángulo formado por p1 y p2 es válido
// Válido significa: Todo su interior y bordes son "verdes" (parte del polígono o su interior)
// Implica:
// 1. El centro del rectángulo está dentro del polígono.
// 2. Ningún borde del polígono cruza a través del interior del rectángulo.
bool is_valid_rectangle(Point p1, Point p2, const vector<Point>& polygon) {
    long long min_x = min(p1.x, p2.x);
    long long max_x = max(p1.x, p2.x);
    long long min_y = min(p1.y, p2.y);
    long long max_y = max(p1.y, p2.y);

    int n = polygon.size();

    // 1. COMPROBACIÓN DE INTERSECCIÓN DE BORDES (Cruces ilegales)
    // Recorremos las paredes del polígono. Si alguna pared atraviesa el rectángulo
    // (entra por un lado y sale por otro, o está contenida dentro pero no en el borde),
    // entonces el rectángulo tiene un "hueco" o una pared interna -> Inválido.
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        // Pared Vertical del polígono
        if (a.x == b.x) {
            // Si la pared vertical está estrictamente DENTRO de los límites X del rectángulo
            if (a.x > min_x && a.x < max_x) {
                // Y se solapa verticalmente con el interior del rectángulo
                if (intervals_overlap_strict(min_y, max_y, a.y, b.y)) {
                    return false; 
                }
            }
        }
        // Pared Horizontal del polígono
        else if (a.y == b.y) {
            // Si la pared horizontal está estrictamente DENTRO de los límites Y del rectángulo
            if (a.y > min_y && a.y < max_y) {
                // Y se solapa horizontalmente con el interior del rectángulo
                if (intervals_overlap_strict(min_x, max_x, a.x, b.x)) {
                    return false;
                }
            }
        }
    }

    // 2. PUNTO EN POLÍGONO (Ray Casting)
    // Comprobamos el centro geométrico del rectángulo.
    double mid_x = (double)(min_x + max_x) / 2.0;
    double mid_y = (double)(min_y + max_y) / 2.0;
    
    int intersections = 0;
    for (int i = 0; i < n; i++) {
        Point a = polygon[i];
        Point b = polygon[(i + 1) % n];

        // Rayo horizontal hacia la derecha desde (mid_x, mid_y)
        // Solo interesan bordes que crucen el eje Y del punto
        if ((a.y > mid_y) != (b.y > mid_y)) { // Si uno está arriba y otro abajo
            // Calcular intersección X
            double intersectX = (b.x - a.x) * (mid_y - a.y) / (double)(b.y - a.y) + a.x;
            if (mid_x < intersectX) {
                intersections++;
            }
        }
    }

    // Impar = Dentro, Par = Fuera
    return (intersections % 2 != 0);
}

bool compareCandidates(const RectCandidate& a, const RectCandidate& b) {
    return a.area > b.area;
}

int main() {
    vector<Point> tiles;
    long long x, y; // Leer como long long
    
    // Leer entrada ignorando posibles cabeceras si las hay, asumiendo formato "x,y" o "x,y" en líneas
    while (scanf("%lld,%lld", &x, &y) == 2) {
        tiles.push_back({x, y});
    }

    int n = tiles.size();
    if (n < 4) {
        printf("Error: Menos de 4 puntos (%d).\n", n);
        return 1;
    }

    printf("Puntos leidos: %d. Procesando...\n", n);

    vector<RectCandidate> candidates;
    candidates.reserve(n * 100); // Reserva heurística

    // Generar candidatos
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Filtrar esquinas alineadas (área sería 0 o línea)
            if (tiles[i].x != tiles[j].x && tiles[i].y != tiles[j].y) {
                long long w = abs(tiles[i].x - tiles[j].x) + 1; // +1 para inclusivo
                long long h = abs(tiles[i].y - tiles[j].y) + 1; // +1 para inclusivo
                candidates.push_back({i, j, w * h});
            }
        }
    }

    printf("Candidatos generados: %zu. Ordenando...\n", candidates.size());
    sort(candidates.begin(), candidates.end(), compareCandidates);

    printf("Verificando validez (empezando por el mas grande)...\n");

    for (const auto& cand : candidates) {
        if (is_valid_rectangle(tiles[cand.p1_idx], tiles[cand.p2_idx], tiles)) {
            printf("\n!!! ENCONTRADO !!!\n");
            printf("Area Maxima (Inclusiva): %lld\n", cand.area);
            printf("Esquinas: (%lld,%lld) y (%lld,%lld)\n", 
                   tiles[cand.p1_idx].x, tiles[cand.p1_idx].y, 
                   tiles[cand.p2_idx].x, tiles[cand.p2_idx].y);
            return 0;
        }
    }

    printf("No se encontró solución.\n");
    return 0;
}