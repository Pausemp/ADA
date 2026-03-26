// Nombre Apellidos, DNI: XXXXXXXXA
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// Representa un camino inaccesible
const int INF = 1e9; 

// 1. Recursivo sin almacén (naive)
int maze_naive(const vector<vector<int>>& grid, int i, int j, int n, int m) {
    if (i >= n || j >= m || grid[i][j] == 0) return INF;
    if (i == n - 1 && j == m - 1) return 1;

    int right = maze_naive(grid, i, j + 1, n, m);
    int down = maze_naive(grid, i + 1, j, n, m);
    int diag = maze_naive(grid, i + 1, j + 1, n, m);

    int min_path = min({right, down, diag});
    return (min_path == INF) ? INF : 1 + min_path;
}

// 2. Recursivo con almacén (memoización)
int maze_memo(const vector<vector<int>>& grid, vector<vector<int>>& memo, int i, int j, int n, int m) {
    if (i >= n || j >= m || grid[i][j] == 0) return INF;
    if (i == n - 1 && j == m - 1) return 1;

    if (memo[i][j] != -1) return memo[i][j];

    int right = maze_memo(grid, memo, i, j + 1, n, m);
    int down = maze_memo(grid, memo, i + 1, j, n, m);
    int diag = maze_memo(grid, memo, i + 1, j + 1, n, m);

    int min_path = min({right, down, diag});
    memo[i][j] = (min_path == INF) ? INF : 1 + min_path;
    return memo[i][j];
}

// Función auxiliar para imprimir el uso correcto del programa
void print_usage() {
    cerr << "Usage:" << endl;
    cerr << "maze [-t] [--p2D] [--ignore-naive] -f file" << endl;
}

int main(int argc, char* argv[]) {
    bool t_flag = false;
    bool p2D_flag = false;
    bool ignore_naive = false;
    string filename = "";
    bool f_found = false;

    // Procesamiento de argumentos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-t") {
            t_flag = true;
        } else if (arg == "--p2D") {
            p2D_flag = true;
        } else if (arg == "--ignore-naive") {
            ignore_naive = true;
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[++i];
                f_found = true;
            } else {
                cerr << "ERROR: missing filename." << endl;
                print_usage();
                return 1;
            }
        } else {
            cerr << "ERROR: unknown option " << arg << "." << endl;
            print_usage();
            return 1;
        }
    }

    if (!f_found) {
        cerr << "ERROR: missing filename." << endl;
        print_usage();
        return 1;
    }

    // Lectura del fichero
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can't open file: " << filename << "." << endl;
        print_usage();
        return 1;
    }

    int n, m;
    if (!(file >> n >> m)) return 1;

    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> grid[i][j];
        }
    }
    file.close();

    // Ejecución de algoritmos requeridos para la Entrega 1
    int res_naive = 0;
    if (!ignore_naive) {
        res_naive = maze_naive(grid, 0, 0, n, m);
        if (res_naive >= INF) res_naive = 0;
    }

    vector<vector<int>> memo(n, vector<int>(m, -1));
    int res_memo = maze_memo(grid, memo, 0, 0, n, m);
    if (res_memo >= INF) res_memo = 0;

    // Salida por terminal según las especificaciones
    if (ignore_naive) {
        cout << "-";
    } else {
        cout << res_naive;
    }
    
    // Los resultados iterativos y de tablas aún no se requieren (Entrega 2)
    cout << " " << res_memo << " ? ?" << endl;

    if (p2D_flag) cout << "?" << endl;
    if (t_flag) cout << "?" << endl;

    return 0;
}