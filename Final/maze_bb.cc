// PAU SEMPERE MARTINEZ 20520990E
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Stats {
    long long nvisit = 0;
    long long nexplored = 0;
    long long nleaf = 0;
    long long nunfeasible = 0;
    long long nnot_promising = 0;
    long long npromising_but_discarded = 0;
    long long nbest_solution_updated_from_leafs = 0;
    long long nbest_solution_updated_from_pessimistic_bound = 0;
};

// Global maze dimensions
int n, m;

// 8-way movement definitions
const int dr[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dc[] = {0, 1, 1, 1, 0, -1, -1, -1};
const int dir_code[] = {1, 2, 3, 4, 5, 6, 7, 8};

// Node record in global repository to reconstruct path using parent pointers
struct NodeRecord {
    int r, c;
    int parent_id;
    int dir;
};

vector<NodeRecord> node_repo;

struct Node {
    int r, c;
    int path_len;
    int opt_bound;
    int repo_idx;

    bool operator<(const Node& other) const {
        if (opt_bound != other.opt_bound) {
            return opt_bound > other.opt_bound; // Smaller opt_bound has higher priority
        }
        if (path_len != other.path_len) {
            return path_len < other.path_len; // Larger path_len has higher priority
        }
        int dist = max(abs(n - 1 - r), abs(m - 1 - c));
        int other_dist = max(abs(n - 1 - other.r), abs(m - 1 - other.c));
        return dist > other_dist; // Smaller distance has higher priority
    }
};

// Fast query-based visited structure for greedy searches
vector<vector<int>> visited_id;
int current_query_id = 0;

void mark_visited(int r, int c) {
    visited_id[r][c] = current_query_id;
}

bool is_visited(int r, int c) {
    return visited_id[r][c] == current_query_id;
}

int chebyshev(int r, int c) {
    return max(abs(n - 1 - r), abs(m - 1 - c));
}

// Reconstruct path from repository up to a certain node record
vector<int> reconstruct_path(int repo_idx) {
    vector<int> path;
    int idx = repo_idx;
    while (idx != -1) {
        if (node_repo[idx].dir != 0) {
            path.push_back(node_repo[idx].dir);
        }
        idx = node_repo[idx].parent_id;
    }
    reverse(path.begin(), path.end());
    return path;
}

// Reconstruct path combining repository, a transition, and greedy moves
vector<int> reconstruct_pessimistic_path(int repo_idx, int last_dir, const vector<int>& greedy_moves) {
    vector<int> path = reconstruct_path(repo_idx);
    if (last_dir != 0) {
        path.push_back(last_dir);
    }
    path.insert(path.end(), greedy_moves.begin(), greedy_moves.end());
    return path;
}

// Greedy pathfinder for pessimistic bound
int get_pessimistic_bound(int r, int c, int path_len, int repo_idx, const vector<vector<int>>& grid, vector<int>& greedy_moves) {
    current_query_id++;
    
    // Mark parent path
    int idx = repo_idx;
    while (idx != -1) {
        mark_visited(node_repo[idx].r, node_repo[idx].c);
        idx = node_repo[idx].parent_id;
    }
    // Mark current node
    mark_visited(r, c);

    int curr_r = r, curr_c = c;
    int steps = 0;
    greedy_moves.clear();

    while (curr_r != n - 1 || curr_c != m - 1) {
        int best_dist = 1e9;
        int best_nr = -1, best_nc = -1;
        int best_dir = -1;

        for (int i = 0; i < 8; i++) {
            int nr = curr_r + dr[i];
            int nc = curr_c + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == 1 && !is_visited(nr, nc)) {
                int dist = chebyshev(nr, nc);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_nr = nr;
                    best_nc = nc;
                    best_dir = dir_code[i];
                }
            }
        }

        if (best_nr == -1) {
            return n * m + 1; // Unreachable greedily
        }

        curr_r = best_nr;
        curr_c = best_nc;
        mark_visited(curr_r, curr_c);
        greedy_moves.push_back(best_dir);
        steps++;
    }

    return path_len + steps;
}

void mostrar_error_sintaxis(string msg) {
    cerr << msg << endl;
    cerr << "maze_bb [-p] [--p2D] -f fichero_entrada" << endl;
}

int main(int argc, char *argv[]) {
    bool p_flag = false, p2d_flag = false;
    string filename = "";

    // 1. Command-line options validation
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-f") {
            if (i + 1 < argc) filename = argv[++i];
            else {
                mostrar_error_sintaxis("Error: falta el nombre del fichero tras -f");
                return 1;
            }
        } else if (arg == "-p") p_flag = true;
        else if (arg == "--p2D") p2d_flag = true;
        else {
            mostrar_error_sintaxis("Error: opcion inexistente " + arg);
            return 1;
        }
    }

    if (filename == "") {
        mostrar_error_sintaxis("Error: la opcion -f es obligatoria");
        return 1;
    }

    clock_t start = clock();

    // 2. Open and read the maze file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << filename << endl;
        return 1;
    }

    if (!(file >> n >> m)) {
        cerr << "Error al leer dimensiones del laberinto" << endl;
        return 1;
    }

    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!(file >> grid[i][j])) {
                cerr << "Error al leer matriz del laberinto" << endl;
                return 1;
            }
        }
    }
    file.close();

    // Initialize visited tracking for greedy search
    visited_id.assign(n, vector<int>(m, 0));
    current_query_id = 0;

    Stats s;
    int inf = n * m + 1;
    int best_len = inf;
    vector<int> best_path;

    // Dominance matrix: minimum path length to reach each cell
    vector<vector<int>> min_dist(n, vector<int>(m, inf));

    // Special case: 1x1 maze
    if (n == 1 && m == 1) {
        s.nvisit = 1;
        if (grid[0][0] == 1) {
            s.nexplored = 1;
            s.nleaf = 1;
            s.nbest_solution_updated_from_leafs = 1;
            best_len = 1;
        }
    } 
    // Normal case with open start/end
    else if (grid[0][0] == 1 && grid[n - 1][m - 1] == 1) {
        priority_queue<Node> LNV;
        node_repo.clear();

        s.nvisit++; // Root is considered

        // Root is feasible and promising
        int root_opt = 1 + chebyshev(0, 0);

        s.nexplored++; // Root is inserted into LNV
        
        NodeRecord rec;
        rec.r = 0;
        rec.c = 0;
        rec.parent_id = -1;
        rec.dir = 0;
        node_repo.push_back(rec);

        Node root_node;
        root_node.r = 0;
        root_node.c = 0;
        root_node.path_len = 1;
        root_node.opt_bound = root_opt;
        root_node.repo_idx = 0;

        LNV.push(root_node);
        min_dist[0][0] = 1;

        long long n_popped = 0;

        while (!LNV.empty()) {
            Node curr = LNV.top();
            LNV.pop();
            n_popped++;

            // Check if no longer promising
            if (curr.opt_bound >= best_len) {
                s.npromising_but_discarded++;
                continue;
            }

            // Check dominance
            if (curr.path_len > min_dist[curr.r][curr.c]) {
                s.npromising_but_discarded++;
                continue;
            }

            // Check if leaf
            if (curr.r == n - 1 && curr.c == m - 1) {
                s.nleaf++;
                if (curr.path_len < best_len) {
                    best_len = curr.path_len;
                    best_path = reconstruct_path(curr.repo_idx);
                    s.nbest_solution_updated_from_leafs++;
                }
                continue;
            }

            // Calculate pessimistic bound when exploring (periodically or early in the search to control CPU time)
            if (n_popped < 10 || n_popped % 500 == 0 || curr.path_len % 256 == 0) {
                vector<int> greedy_moves;
                int curr_pess = get_pessimistic_bound(curr.r, curr.c, curr.path_len, curr.repo_idx, grid, greedy_moves);
                if (curr_pess < best_len) {
                    best_len = curr_pess;
                    best_path = reconstruct_pessimistic_path(curr.repo_idx, 0, greedy_moves);
                    s.nbest_solution_updated_from_pessimistic_bound++;
                }
            }

            // Generate neighbors
            for (int i = 0; i < 8; i++) {
                int nr = curr.r + dr[i];
                int nc = curr.c + dc[i];

                s.nvisit++; // Neighbor is considered

                // Feasibility check
                if (nr < 0 || nr >= n || nc < 0 || nc >= m || grid[nr][nc] == 0) {
                    s.nunfeasible++;
                    continue;
                }

                // Promising check (optimistic bound & dominance)
                int child_opt = curr.path_len + 1 + chebyshev(nr, nc);
                if (child_opt >= best_len || curr.path_len + 1 >= min_dist[nr][nc]) {
                    s.nnot_promising++;
                    continue;
                }

                // Valid and promising: update dominance
                min_dist[nr][nc] = curr.path_len + 1;

                // Push neighbor to LNV
                s.nexplored++;
                NodeRecord rec;
                rec.r = nr;
                rec.c = nc;
                rec.parent_id = curr.repo_idx;
                rec.dir = dir_code[i];
                int new_repo_idx = node_repo.size();
                node_repo.push_back(rec);

                Node child_node;
                child_node.r = nr;
                child_node.c = nc;
                child_node.path_len = curr.path_len + 1;
                child_node.opt_bound = child_opt;
                child_node.repo_idx = new_repo_idx;

                LNV.push(child_node);
            }
        }
    } 
    // Closed entrance or exits
    else {
        s.nvisit++;
        // If grid[0][0] == 0, we only visit it and stop.
        // If grid[0][0] == 1 but grid[n-1][m-1] == 0, then root is feasible, but we will fail.
        // The above code only enters if both are 1. If not, it lands here.
    }

    clock_t end = clock();
    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    // Output results in the exact requested format
    cout << (best_len == inf ? 0 : best_len) << "\n";
    cout << s.nvisit << " " << s.nexplored << " " << s.nleaf << " " << s.nunfeasible << " "
         << s.nnot_promising << " " << s.npromising_but_discarded << " "
         << s.nbest_solution_updated_from_leafs << " "
         << s.nbest_solution_updated_from_pessimistic_bound << "\n";
    cout << fixed << setprecision(3) << ms << "\n";

    if (p2d_flag) {
        if (best_len == inf) {
            cout << 0 << "\n";
        } else {
            vector<string> out(n, string(m, ' '));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    out[i][j] = grid[i][j] + '0';
                }
            }
            int r = 0, c = 0;
            out[r][c] = '*';
            for (int d : best_path) {
                r += dr[d - 1];
                c += dc[d - 1];
                out[r][c] = '*';
            }
            for (int i = 0; i < n; i++) {
                cout << out[i] << "\n";
            }
        }
    }

    if (p_flag) {
        if (best_len == inf) {
            cout << "<0>\n";
        } else {
            cout << "<";
            for (int d : best_path) {
                cout << d;
            }
            cout << ">\n";
        }
    }

    return 0;
}
