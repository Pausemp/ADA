// Nombre Apellidos, DNI: XXXXXXXXA
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


const int INF = 1e9; 


int maze_naive(const vector<vector<int>>& maze, int i, int j, int n, int m) {
    if (i >= n || j >= m || maze[i][j] == 0) return INF;
    if (i == n - 1 && j == m - 1) return 1;

    int derecha = maze_naive(maze, i, j + 1, n, m);
    int abajo = maze_naive(maze, i + 1, j, n, m);
    int diagonal = maze_naive(maze, i + 1, j + 1, n, m);

    int camino = min({derecha, abajo, diagonal});
    if (camino == INF) {
        return INF; 
    } else {
        return 1 + camino; 
    }
}


int maze_memo(const vector<vector<int>>& maze, vector<vector<int>>& memo, int i, int j, int n, int m) {
    // Si nos salimos de los límites, simplemente devolvemos INF (no existe en la matriz)
    if (i >= n || j >= m) return INF;

    // Si es un obstáculo, el enunciado exige que quede marcado como 'X' en la tabla si se intenta visitar 
    if (maze[i][j] == 0) {
        memo[i][j] = INF;
        return INF;
    }

    // Si hemos llegado al destino, guardamos el 1 antes de retornar
    if (i == n - 1 && j == m - 1) {
        memo[i][j] = 1;
        return 1;
    }

    // Si ya lo hemos calculado previamente, lo devolvemos
    if (memo[i][j] != -1) return memo[i][j];

    int derecha = maze_memo(maze, memo, i, j + 1, n, m);
    int abajo = maze_memo(maze, memo, i + 1, j, n, m);
    int diagonal = maze_memo(maze, memo, i + 1, j + 1, n, m);

    int camino = min({derecha, abajo, diagonal});
    
    if (camino == INF) {
        memo[i][j] = INF; 
    } else {
        memo[i][j] = 1 + camino; 
    }
    
    return memo[i][j];
}

int maze_it_matrix(const vector<vector<int>>& maze, int n, int m, vector<vector<int>>& memo) {
    // Inicializamos con INF
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            memo[i][j] = INF;
        }
    }

    // Recorremos de abajo hacia arriba, de derecha a izquierda
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (maze[i][j] == 0) {
                memo[i][j] = INF;
            } else if (i == n - 1 && j == m - 1) {
                memo[i][j] = 1;
            } else {
                int derecha = INF;
                if (j + 1 < m) {
                    derecha = memo[i][j + 1];
                }

                int abajo = INF;
                if (i + 1 < n) {
                    abajo = memo[i + 1][j];
                }

                int diag = INF;
                if (i + 1 < n && j + 1 < m) {
                    diag = memo[i + 1][j + 1];
                }
                
                int minimo = min({derecha, abajo, diag});
                if (minimo != INF) {
                    memo[i][j] = minimo + 1;
                }
            }
        }
    }
    return memo[0][0];
}

// 4. Iterativo con complejidad espacial mejorada
int maze_it_vector(const vector<vector<int>>& maze, int n, int m) {
    vector<int> fila_abajo(m, INF);
    vector<int> fila_actual(m, INF);
    
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (maze[i][j] == 0) {
                fila_actual[j] = INF;
            } 
            else if (i == n - 1 && j == m - 1) {
                fila_actual[j] = 1;
            } 
            else {
                int derecha = INF;
                if (j + 1 < m) {
                    derecha = fila_actual[j + 1];
                }

                int abajo = fila_abajo[j]; 
                
                int diag = INF;
                if (j + 1 < m) {
                    diag = fila_abajo[j + 1];
                }

                int minimo = min({derecha, abajo, diag});
                if (minimo == INF) {
                    fila_actual[j] = INF;
                } else {
                    fila_actual[j] = minimo + 1;
                }
            }
        }
        swap(fila_abajo, fila_actual); 
    }
    
    return fila_abajo[0];
}

// 5. Extracción de la selección (Parsing)
vector<string> maze_parser(const vector<vector<int>>& maze, const vector<vector<int>>& memo, int n, int m) {
    vector<string> mapa_salida(n, string(m, '0'));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] != 0) {
                mapa_salida[i][j] = '1';
            } else {
                mapa_salida[i][j] = '0';
            }
        }
    }

    if (memo[0][0] == INF) return mapa_salida;

    int i = 0;
    int j = 0;
    mapa_salida[i][j] = '*';

    while (i != n - 1 || j != m - 1) {
        int coste_actual = memo[i][j];
        
        if (j + 1 < m && memo[i][j + 1] == coste_actual - 1) {
            j = j + 1;
        } else if (i + 1 < n && memo[i + 1][j] == coste_actual - 1) {
            i = i + 1;
        } else if (i + 1 < n && j + 1 < m && memo[i + 1][j + 1] == coste_actual - 1) {
            i = i + 1;
            j = j + 1;
        } else {
            break;
        }
        
        mapa_salida[i][j] = '*';
    }
    
    return mapa_salida;
}

// Función auxiliar para imprimir el uso correcto del programa
void print_usage() {
    cerr << "Usage:" << endl;
    cerr << "maze [-t] [--p2D] [--ignore-naive] -f file" << endl;
}

int main(int argc, char* argv[]) {
    bool p2D = false;
    bool t = false;
    bool ignore_naive = false;
    string filename = "";

    // Parseo de argumentos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--p2D") {
            p2D = true;
        } else if (arg == "-t") {
            t = true;
        } else if (arg == "--ignore-naive") {
            ignore_naive = true;
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[++i];
            }
        } else {
            cerr << "ERROR: unknown option " << arg << ".\n";
            print_usage();
            return 1;
        }
    }

    if (filename == "") {
        cerr << "ERROR: missing filename.\n";
        print_usage();
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can't open file: " << filename << ".\n";
        print_usage();
        return 1;
    }

    // Lectura del laberinto
    int n, m;
    file >> n >> m;
    vector<vector<int>> maze(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> maze[i][j];
        }
    }
    file.close();

    // Ejecución de algoritmos
    int ans_naive = INF;
    if (!ignore_naive) {
        ans_naive = maze_naive(maze, 0, 0, n, m);
    }

    vector<vector<int>> memo(n, vector<int>(m, -1));
    int ans_memo = maze_memo(maze, memo, 0, 0, n, m);

    vector<vector<int>> M(n, vector<int>(m));
    int ans_it_matrix = maze_it_matrix(maze, n, m, M);

    int ans_it_vector = maze_it_vector(maze, n, m);

    // Salida 1: Resultados de los algoritmos
    auto format_ans = [](int val) {
        return (val == INF) ? "0" : to_string(val);
    };

    if (ignore_naive) {
        cout << "- ";
    } else {
        cout << format_ans(ans_naive) << " ";
    }
    cout << format_ans(ans_memo) << " " 
         << format_ans(ans_it_matrix) << " " 
         << format_ans(ans_it_vector) << "\n";

    // Salida 2: Opción --p2D
    if (p2D) {
        if (ans_it_matrix == INF) {
            cout << "0\n";
        } else {
            vector<string> path = maze_parser(maze, M, n, m);
            for (const string& row : path) {
                cout << row << "\n";
            }
        }
    }

    // Salida 3: Opción -t
    if (t) {
        cout << "Memoization table:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (memo[i][j] == -1) cout << "-";
                else if (memo[i][j] == INF) cout << "X";
                else cout << memo[i][j];
                
                if (j < m - 1) cout << " ";
            }
            cout << "\n";
        }

        cout << "Iterative table:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (M[i][j] == INF) cout << "X";
                else cout << M[i][j];
                
                if (j < m - 1) cout << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}