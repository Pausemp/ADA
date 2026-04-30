// PAU SEMPERE MARTINEZ 20520990E

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void print_usage() {
    cerr << "Usage:\nmaze_greedy [--p2D] -f file\n";
}

int maze_greedy(vector<vector<char>>& maze, int n, int m) {
    if (maze.empty() || maze[0][0] == '0') {
        return 0;
    }

    int r = 0, c = 0;
    int length = 1; 
    maze[r][c] = '*';


    while (r != n - 1 || c != m - 1) {
        
        if (r + 1 < n && c + 1 < m && maze[r + 1][c + 1] == '1') {
            r++;
            c++;
        }
        else if (c + 1 < m && maze[r][c + 1] == '1') {
            c++;
        }
        else if (r + 1 < n && maze[r + 1][c] == '1') {
            r++;
        }
        else {
            return 0;
        }

        maze[r][c] = '*'; 
        length++;
    }

    return length;
}

int main(int argc, char* argv[]) {
    bool p2d = false;
    string filename = "";
    bool f_found = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--p2D") {
            p2d = true;
        } else if (arg == "-f") {
            f_found = true;
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                cerr << "ERROR: missing filename.\n";
                print_usage();
                return 1;
            }
        } else {
            cerr << "ERROR: unknown option " << arg << ".\n";
            print_usage();
            return 1;
        }
    }

    if (!f_found) {
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
    if (!(file >> n >> m)) {
        return 1; 
    }

    vector<vector<char>> maze(n, vector<char>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> maze[i][j];
        }
    }
    file.close();

    int path_length = maze_greedy(maze, n, m);

    cout << path_length << "\n";

    if (p2d) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << maze[i][j];
            }
            cout << "\n";
        }
    }

    return 0;
}