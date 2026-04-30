// PAU SEMPERE MARTINEZ 20520990E
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

struct Stats
{
    long visited = 0, explored = 0, leaf = 0, infeasible = 0, non_promising = 0;
};

int n, m;
vector<vector<int>> grid, min_dist;
vector<vector<bool>> in_path;
const int dr[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dc[] = {0, 1, 1, 1, 0, -1, -1, -1};
const int dir_code[] = {1, 2, 3, 4, 5, 6, 7, 8};

// Función obligatoria para la evaluación
void maze_bt(int r, int c, int cur_len, vector<int> &cur_path, int &best_len, vector<int> &best_path, Stats &s)
{
    s.visited++;

    if (r == n - 1 && c == m - 1)
    {
        s.leaf++;
        if (cur_len < best_len)
        {
            best_len = cur_len;
            best_path = cur_path;
        }
        return;
    }

    s.explored++;
    for (int i = 0; i < 8; i++)
    {
        int nr = r + dr[i], nc = c + dc[i];

        if (nr < 0 || nr >= n || nc < 0 || nc >= m || grid[nr][nc] == 0 || in_path[nr][nc])
        {
            s.infeasible++;
            continue;
        }

        // Poda: No prometedor
        if (cur_len + 1 >= best_len || cur_len + 1 >= min_dist[nr][nc])
        {
            s.non_promising++;
            continue;
        }

        min_dist[nr][nc] = cur_len + 1;
        in_path[nr][nc] = true;
        cur_path.push_back(dir_code[i]);

        maze_bt(nr, nc, cur_len + 1, cur_path, best_len, best_path, s);

        cur_path.pop_back();
        in_path[nr][nc] = false;
    }
}

int main(int argc, char *argv[])
{
    bool p_flag = false, p2d_flag = false;
    string filename = "";

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "-f" && i + 1 < argc)
            filename = argv[++i];
        else if (arg == "-p")
            p_flag = true;
        else if (arg == "--p2D")
            p2d_flag = true;
    }

    if (filename == "")
        return 1;

    ifstream file(filename);
    if (!(file >> n >> m))
        return 1;
    grid.assign(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> grid[i][j];

    clock_t start = clock();

    Stats s;
    int inf = n * m + 1; // Valor superior al camino máximo posible
    int best_len = inf;
    vector<int> best_path, cur_path;
    min_dist.assign(n, vector<int>(m, inf));
    in_path.assign(n, vector<bool>(m, false));

    if (grid[0][0] != 0 && grid[n - 1][m - 1] != 0)
    {
        in_path[0][0] = true;
        min_dist[0][0] = 1;
        if (n == 1 && m == 1)
            best_len = 1;
        else
            maze_bt(0, 0, 1, cur_path, best_len, best_path, s);
    }

    clock_t end = clock();
    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    // Salida según formato estricto del PDF[cite: 1]
    cout << (best_len == inf ? 0 : best_len) << endl;
    cout << s.visited << " " << s.explored << " " << s.leaf << " " << s.infeasible << " " << s.non_promising << endl;
    cout << fixed << setprecision(3) << ms << endl;

    if (p2d_flag)
    {
        if (best_len == inf)
            cout << 0 << endl;
        else
        {
            vector<string> out(n, string(m, ' '));
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    out[i][j] = grid[i][j] + '0';
            int r = 0, c = 0;
            out[r][c] = '*';
            for (int d : best_path)
            {
                r += dr[d - 1];
                c += dc[d - 1];
                out[r][c] = '*';
            }
            for (int i = 0; i < n; i++)
                cout << out[i] << endl;
        }
    }

    if (p_flag)
    {
        if (best_len == inf)
            cout << "<0>" << endl;
        else
        {
            cout << "<";
            for (int d : best_path)
                cout << d;
            cout << ">" << endl;
        }
    }
    return 0;
}