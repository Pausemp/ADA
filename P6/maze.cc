// PAU SEMPERE MARTINEZ 20520990E
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

const int INF = 1e9; 

int maze_naive(const vector<vector<int>>& maze, int i, int j) {
    if (i < 0 || j < 0 || maze[i][j] == 0) return INF;
    if (i == 0 && j == 0) return 1;

    int izquierda = maze_naive(maze, i, j - 1);
    int arriba = maze_naive(maze, i - 1, j);
    int diagonal = maze_naive(maze, i - 1, j - 1);

    int camino = min({izquierda, arriba, diagonal});
    if (camino == INF) {
        return INF; 
    } else {
        return 1 + camino; 
    }
}

int maze_memo(const vector<vector<int>>& maze, vector<vector<int>>& memo, int i, int j) {
    if (i < 0 || j < 0) return INF;
    
    if (maze[i][j] == 0) {
        memo[i][j] = INF;
        return INF;
    }

    if (i == 0 && j == 0) {
        memo[i][j] = 1;
        return 1;
    }

    if (memo[i][j] != -1) return memo[i][j];

    int izquierda = maze_memo(maze, memo, i, j - 1);
    int arriba = maze_memo(maze, memo, i - 1, j);
    int diagonal = maze_memo(maze, memo, i - 1, j - 1);

    int camino = min({izquierda, arriba, diagonal});
    
    if (camino == INF) {
        memo[i][j] = INF; 
    } else {
        memo[i][j] = 1 + camino; 
    }
    
    return memo[i][j];
}

int maze_it_matrix(const vector<vector<int>>& maze, int n, int m, vector<vector<int>>& memo) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 0) {
                memo[i][j] = INF;
            } else if (i == 0 && j == 0) {
                memo[i][j] = 1;
            } else {
                int izquierda = INF;
                if (j > 0) {
                    izquierda = memo[i][j - 1];
                }

                int arriba = INF;
                if (i > 0) {
                    arriba = memo[i - 1][j];
                }

                int diagonal = INF;
                if (i > 0 && j > 0) {
                    diagonal = memo[i - 1][j - 1];
                }
                
                int minimo = min({izquierda, arriba, diagonal});
                if (minimo != INF) {
                    memo[i][j] = minimo + 1;
                } else {
                    memo[i][j] = INF;
                }
            }
        }
    }
    return memo[n - 1][m - 1];
}

int maze_it_vector(const vector<vector<int>>& maze, int n, int m) {
    vector<int> fila_arriba(m, INF);
    vector<int> fila_actual(m, INF);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 0) {
                fila_actual[j] = INF;
            } 
            else if (i == 0 && j == 0) {
                fila_actual[j] = 1;
            } 
            else {
                int izquierda = INF;
                if (j > 0) {
                    izquierda = fila_actual[j - 1];
                }

                int arriba = fila_arriba[j]; 
                
                int diagonal = INF;
                if (j > 0) {
                    diagonal = fila_arriba[j - 1];
                }

                int minimo = min({izquierda, arriba, diagonal});
                if (minimo == INF) {
                    fila_actual[j] = INF;
                } else {
                    fila_actual[j] = minimo + 1;
                }
            }
        }
        fila_arriba = fila_actual; 
    }
    
    return fila_arriba[m - 1];
}

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

    if (memo[n - 1][m - 1] == INF) return mapa_salida;

    int i = n - 1;
    int j = m - 1;
    mapa_salida[i][j] = '*';

    while (i != 0 || j != 0) {
        int coste_actual = memo[i][j];
        
        if (i > 0 && j > 0 && memo[i - 1][j - 1] == coste_actual - 1) {
            i = i - 1;
            j = j - 1;
        } else if (i > 0 && memo[i - 1][j] == coste_actual - 1) {
            i = i - 1;
        } else if (j > 0 && memo[i][j - 1] == coste_actual - 1) {
            j = j - 1;
        } else {
            break;
        }
        
        mapa_salida[i][j] = '*';
    }
    
    return mapa_salida;
}

void print_usage() {
    cerr << "Usage:" << endl;
    cerr << "maze [-t] [--p2D] [--ignore-naive] -f file" << endl;
}

int main(int argc, char* argv[]) {
    bool p2D = false;
    bool t = false;
    bool ignore_naive = false;
    string filename = "";

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

    int n, m;
    file >> n >> m;
    vector<vector<int>> maze(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> maze[i][j];
        }
    }
    file.close();

    int ans_naive = INF;
    if (!ignore_naive) {
        ans_naive = maze_naive(maze, n - 1, m - 1); 
    }

    vector<vector<int>> memo(n, vector<int>(m, -1));
    int ans_memo = maze_memo(maze, memo, n - 1, m - 1);

    vector<vector<int>> M(n, vector<int>(m));
    int ans_it_matrix = maze_it_matrix(maze, n, m, M);

    int ans_it_vector = maze_it_vector(maze, n, m);

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