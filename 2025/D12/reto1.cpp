#include <bits/stdc++.h>
using namespace std;

// --- Estructuras de Datos ---
struct Point {
    int r, c;
    bool operator<(const Point& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
};

struct Piece {
    vector<Point> coords; // Coordenadas normalizadas
    int height;           // Altura de la pieza (para optimizar)
    int width;
};

// --- Datos Globales ---
map<int, vector<string>> raw_shapes;
map<int, vector<Piece>> unique_pieces; 
long long total_regions_fit = 0;

// --- Funciones Auxiliares ---
static void trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
}

// Normaliza las coordenadas para que empiecen en (0,0) y ordena
static Piece make_piece(const vector<Point>& coords) {
    if (coords.empty()) return {{}, 0, 0};
    int min_r = INT_MAX, min_c = INT_MAX;
    int max_r = INT_MIN, max_c = INT_MIN;
    for (auto &p : coords) {
        min_r = min(min_r, p.r); min_c = min(min_c, p.c);
        max_r = max(max_r, p.r); max_c = max(max_c, p.c);
    }
    vector<Point> norm;
    for (auto &p : coords) norm.push_back({p.r - min_r, p.c - min_c});
    return {norm, max_r - min_r + 1, max_c - min_c + 1};
}

// Aplica espejo y rotación
static vector<Point> transform_coords(const vector<Point>& original, int m, int r) {
    vector<Point> transformed;
    for (auto &p : original) {
        int row = p.r, col = p.c;
        if (m == 1) col = -col; // Espejo
        for (int i=0;i<r;i++) { // Rotar 90 deg
            int new_row = col;
            int new_col = -row;
            row = new_row; col = new_col;
        }
        transformed.push_back({row,col});
    }
    return transformed;
}

static void generate_unique_pieces() {
    for (auto &entry : raw_shapes) {
        int index = entry.first;
        auto &raw_grid = entry.second;
        vector<Point> base;
        for (int r=0;r<(int)raw_grid.size();r++)
            for (int c=0;c<(int)raw_grid[r].size();c++)
                if (raw_grid[r][c]=='#') base.push_back({r,c});
        
        // Usamos un set temporal para eliminar duplicados geométricos
        set<vector<Point>> uniq_coords;
        for (int m=0;m<2;m++) {
            for (int r=0;r<4;r++) {
                vector<Point> t = transform_coords(base, m, r);
                // Normalizamos antes de insertar al set para comparar formas correctamente
                Piece p = make_piece(t);
                sort(p.coords.begin(), p.coords.end()); 
                uniq_coords.insert(p.coords);
            }
        }
        
        for (auto &coords : uniq_coords) {
            unique_pieces[index].push_back(make_piece(coords));
        }
    }
}

// --- Solver (Backtracking con First Free Cell y Slack) ---
// grid: 0 = libre, 1 = ocupado
// counts: piezas disponibles
// slacks_left: número de celdas que se permite dejar vacías
static bool solve(map<int, int>& counts, vector<vector<int>>& grid, int slacks_left) {
    int H = grid.size();
    int W = grid[0].size();

    // 1. Encontrar la primera celda libre (arriba a abajo, izq a der)
    int fr = -1, fc = -1;
    for(int r=0; r<H; r++) {
        for(int c=0; c<W; c++) {
            if (grid[r][c] == 0) {
                fr = r; fc = c;
                goto found;
            }
        }
    }
    found:;

    // Caso Base: Si no hay celdas libres, hemos terminado con éxito
    if (fr == -1) return true;

    // 2. Intentar colocar una pieza REAL
    for (auto &kv : counts) {
        int id = kv.first;
        int &qty = kv.second;
        
        if (qty > 0) {
            qty--; 
            
            for (const auto &piece : unique_pieces[id]) {
                // OPTIMIZACIÓN: Solo intentamos anclar la pieza usando sus bloques
                // que están en su fila superior (r=0).
                // Si usáramos un bloque con r>0 como ancla en (fr, fc), la parte superior
                // de la pieza caería en filas anteriores a fr, que ya están llenas.
                for (const auto &anchor : piece.coords) {
                    if (anchor.r != 0) continue; 

                    // Calcular la posición top-left de la pieza
                    int r0 = fr - anchor.r; // será == fr
                    int c0 = fc - anchor.c;

                    // Verificar límites y colisiones
                    bool fits = true;
                    // Pre-chequeo rápido de límites del bounding box
                    if (c0 < 0 || c0 + piece.width > W || r0 + piece.height > H) {
                        fits = false;
                    } else {
                        for (const auto &p : piece.coords) {
                            if (grid[r0 + p.r][c0 + p.c] != 0) {
                                fits = false; break;
                            }
                        }
                    }

                    if (fits) {
                        // Colocar
                        for (const auto &p : piece.coords) grid[r0 + p.r][c0 + p.c] = 1;
                        
                        // Recurrir
                        if (solve(counts, grid, slacks_left)) return true;
                        
                        // Deshacer (Backtrack)
                        for (const auto &p : piece.coords) grid[r0 + p.r][c0 + p.c] = 0;
                    }
                }
            }
            qty++; // Devolver pieza al inventario
        }
    }

    // 3. Intentar usar SLACK (saltar esta celda dejándola vacía)
    if (slacks_left > 0) {
        grid[fr][fc] = -1; // Marcamos como "saltada/vacía"
        if (solve(counts, grid, slacks_left - 1)) return true;
        grid[fr][fc] = 0;  // Backtrack
    }

    return false;
}

// --- Main ---
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> lines; string line;
    while (getline(cin,line)) lines.push_back(line);

    int curShape = -1;
    int firstRegionLine = -1;

    // --- PARSING ---
    for (int i=0; i<(int)lines.size(); i++){
        line = lines[i]; trim(line);
        if (line.empty()) continue;

        // Detectar Región: Debe tener 'x' (ej: "4x4:")
        // Lo comprobamos ANTES que el formato de forma para evitar confusión.
        if (line.find('x') != string::npos) {
            if (firstRegionLine == -1) firstRegionLine = i;
            break; // Dejamos de leer formas
        } 
        // Detectar Cabecera de Forma: Dígito seguido de ':'
        else if (isdigit(line[0]) && line.find(':') != string::npos) {
            curShape = stoi(line.substr(0, line.find(':')));
            raw_shapes[curShape] = {};
        } 
        // Detectar Cuerpo de Forma
        else if (curShape != -1 && (line.find('#') != string::npos || line.find('.') != string::npos)) {
            raw_shapes[curShape].push_back(line);
        }
    }

    if (firstRegionLine == -1 && raw_shapes.empty()) { cout << 0 << "\n"; return 0; }

    generate_unique_pieces();

    // Procesar Regiones
    for (int i=firstRegionLine; i<(int)lines.size(); i++) {
        line = lines[i]; trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string dim; ss >> dim; // "4x4:" o "4x4"
        
        // Limpieza de ':' si existe
        if (!dim.empty() && dim.back() == ':') dim.pop_back();
        
        size_t xpos = dim.find('x');
        if (xpos == string::npos) continue;

        int W = stoi(dim.substr(0, xpos));
        int H = stoi(dim.substr(xpos + 1));

        map<int, int> counts;
        int val, idx = 0;
        int piecesArea = 0;

        while (ss >> val) {
            if (val > 0) {
                // Asumimos que el orden en la línea corresponde a los índices de forma 0, 1, 2...
                if (unique_pieces.count(idx)) {
                    counts[idx] = val;
                    // Calculamos área de la pieza[idx] usando la primera orientación
                    piecesArea += val * (int)unique_pieces[idx][0].coords.size();
                }
            }
            idx++;
        }

        int totalGridArea = W * H;
        if (piecesArea > totalGridArea) continue; // No caben por área

        int slack = totalGridArea - piecesArea; // Celdas que quedarán vacías
        vector<vector<int>> grid(H, vector<int>(W, 0));

        if (solve(counts, grid, slack)) {
            total_regions_fit++;
        }
    }

    cout << total_regions_fit << "\n";
    return 0;
}