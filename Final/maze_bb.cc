// PAU SEMPERE MARTINEZ 20520990E

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace std::chrono;

// Estructura que representa un nodo en el espacio de estados
struct Nodo
{
    int fila, columna;
    int g;         // Coste real acumulado (longitud del camino)
    int h;         // Cota optimista (heurística)
    int f;         // Coste total estimado (g + h)
    string camino; // Secuencia de movimientos realizados

    // Priorizamos los nodos con menor 'f'. A igualdad de 'f', priorizamos mayor 'g' (más profundos en el laberinto)
    bool operator>(const Nodo &otro) const
    {
        if (f == otro.f)
            return g < otro.g;
        return f > otro.f;
    }
};

// Cota optimista
int calcular_heuristica(int fila, int columna, int dest_fila, int dest_columna)
{
    return max(abs(fila - dest_fila), abs(columna - dest_columna));
}

void imprimir_uso()
{
    cerr << "Uso: maze_bb [-p] [--p2D] -f fichero_entrada\n";
    exit(1);
}

// Variables globales para las estadísticas requeridas
long long nodos_visitados = 0;
long long nodos_explorados = 0;
long long nodos_hoja = 0;
long long nodos_no_factibles = 0;
long long nodos_no_prometedores = 0;
long long prometedores_descartados = 0;
long long sol_actualizada_hoja = 0;
long long sol_actualizada_pesimista = 0;

// Función principal de ramificación y poda requerida por el enunciado
void maze_bb(const vector<vector<int>> &laberinto, int n, int m, int &mejor_coste, string &mejor_camino_str)
{
    // Matriz de dominancia: guarda el menor coste 'g' encontrado para llegar a cada celda
    vector<vector<int>> min_coste_real(n, vector<int>(m, 1e9));

    if (laberinto[0][0] == 1)
    {
        priority_queue<Nodo, vector<Nodo>, greater<Nodo>> cola_prioridad;
        int h_inicial = calcular_heuristica(0, 0, n - 1, m - 1);
        cola_prioridad.push({0, 0, 1, h_inicial, 1 + h_inicial, ""});
        min_coste_real[0][0] = 1;
        nodos_explorados++;

        // Vectores de desplazamiento: Norte, NE, Este, SE, Sur, SO, Oeste, NO
        int d_fila[] = {-1, -1, 0, 1, 1, 1, 0, -1};
        int d_col[] = {0, 1, 1, 1, 0, -1, -1, -1};
        char char_direccion[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

        while (!cola_prioridad.empty())
        {
            Nodo actual = cola_prioridad.top();
            cola_prioridad.pop();

            // Poda: Si tras sacarlo de la cola ya no es prometedor o hemos llegado por un camino mejor
            if (actual.f >= mejor_coste || actual.g > min_coste_real[actual.fila][actual.columna])
            {
                prometedores_descartados++;
                continue;
            }

            // ¿Es nodo hoja (hemos llegado a la salida)?
            if (actual.fila == n - 1 && actual.columna == m - 1)
            {
                nodos_hoja++;
                if (actual.g < mejor_coste)
                {
                    mejor_coste = actual.g; // Actualizacion de la cota pesimista global
                    mejor_camino_str = actual.camino;
                    sol_actualizada_hoja++;
                }
                continue;
            }

            // Expandir los 8 vecinos posibles [cite: 33]
            for (int i = 0; i < 8; ++i)
            {
                int nueva_fila = actual.fila + d_fila[i];
                int nueva_col = actual.columna + d_col[i];
                nodos_visitados++;

                // Poda por factibilidad (fuera de límites o es una pared/inaccesible) [cite: 9]
                if (nueva_fila < 0 || nueva_fila >= n || nueva_col < 0 || nueva_col >= m || laberinto[nueva_fila][nueva_col] == 0)
                {
                    nodos_no_factibles++;
                    continue;
                }

                int siguiente_g = actual.g + 1;
                // Poda por dominancia (ya llegamos a esta celda antes con igual o mejor coste)
                if (siguiente_g >= min_coste_real[nueva_fila][nueva_col])
                {
                    nodos_no_factibles++;
                    continue;
                }

                int siguiente_h = calcular_heuristica(nueva_fila, nueva_col, n - 1, m - 1);
                int siguiente_f = siguiente_g + siguiente_h;

                // Poda por cota (nodo no prometedor, excede o iguala el mejor coste encontrado)
                if (siguiente_f >= mejor_coste)
                {
                    nodos_no_prometedores++;
                    continue;
                }

                // Si es factible y prometedor, actualizamos dominancia y lo metemos a la cola
                min_coste_real[nueva_fila][nueva_col] = siguiente_g;
                nodos_explorados++;
                cola_prioridad.push({nueva_fila, nueva_col, siguiente_g, siguiente_h, siguiente_f, actual.camino + char_direccion[i]});
            }
        }
    }
}

int main(int argc, char *argv[])
{
    string nombre_fichero = "";
    bool opcion_p = false;
    bool opcion_p2D = false;

    // Procesar los argumentos pasados por consola [cite: 45]
    for (int i = 1; i < argc; ++i)
    {
        string argumento = argv[i];
        if (argumento == "-f")
        {
            if (i + 1 < argc)
                nombre_fichero = argv[++i];
            else
                imprimir_uso();
        }
        else if (argumento == "-p")
        {
            opcion_p = true;
        }
        else if (argumento == "--p2D")
        {
            opcion_p2D = true;
        }
        else
        {
            imprimir_uso();
        }
    }

    if (nombre_fichero.empty())
        imprimir_uso();

    // Lectura del fichero de entrada [cite: 96, 100, 101]
    ifstream archivo(nombre_fichero);
    if (!archivo)
    {
        cerr << "Error: No se pudo abrir el fichero " << nombre_fichero << "\n";
        exit(1);
    }

    int n, m;
    archivo >> n >> m;
    vector<vector<int>> laberinto(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            archivo >> laberinto[i][j];
        }
    }
    archivo.close();

    int mejor_coste = 1e9; // Cota pesimista global inicializada a "infinito"
    string mejor_camino_str = "";

    // Medir tiempo de ejecución [cite: 76]
    auto tiempo_inicio = high_resolution_clock::now();

    // Llamada a la función principal
    maze_bb(laberinto, n, m, mejor_coste, mejor_camino_str);

    auto tiempo_fin = high_resolution_clock::now();
    double tiempo_cpu_ms = duration<double, std::milli>(tiempo_fin - tiempo_inicio).count();

    bool solucion_encontrada = (mejor_coste != 1e9);

    // Salida 1: Longitud del camino (o 0 si no hay salida) [cite: 61, 62]
    cout << (solucion_encontrada ? mejor_coste : 0) << "\n";

    // Salida 2: Los ocho valores estadísticos separados por espacios [cite: 63, 64]
    cout << nodos_visitados << " " << nodos_explorados << " " << nodos_hoja << " "
         << nodos_no_factibles << " " << nodos_no_prometedores << " "
         << prometedores_descartados << " " << sol_actualizada_hoja << " "
         << sol_actualizada_pesimista << "\n";

    // Salida 3: Tiempo en milisegundos [cite: 76]
    cout << tiempo_cpu_ms << "\n";

    // Salida de la opción --p2D: Laberinto con el camino marcado con asteriscos [cite: 79]
    if (opcion_p2D)
    {
        if (!solucion_encontrada)
        {
            cout << "0\n"; // [cite: 82]
        }
        else
        {
            vector<vector<char>> laberinto_salida(n, vector<char>(m));
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                    laberinto_salida[i][j] = laberinto[i][j] ? '1' : '0';

            int fila_actual = 0, col_actual = 0;
            laberinto_salida[fila_actual][col_actual] = '*'; // Casilla de inicio [cite: 79]

            for (char movimiento : mejor_camino_str)
            {
                if (movimiento == '1')
                {
                    fila_actual--;
                }
                else if (movimiento == '2')
                {
                    fila_actual--;
                    col_actual++;
                }
                else if (movimiento == '3')
                {
                    col_actual++;
                }
                else if (movimiento == '4')
                {
                    fila_actual++;
                    col_actual++;
                }
                else if (movimiento == '5')
                {
                    fila_actual++;
                }
                else if (movimiento == '6')
                {
                    fila_actual++;
                    col_actual--;
                }
                else if (movimiento == '7')
                {
                    col_actual--;
                }
                else if (movimiento == '8')
                {
                    fila_actual--;
                    col_actual--;
                }
                laberinto_salida[fila_actual][col_actual] = '*';
            }

            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < m; ++j)
                {
                    cout << laberinto_salida[i][j]; // Sin espacios de separación [cite: 80]
                }
                cout << "\n";
            }
        }
    }

    // Salida de la opción -p: Secuencia numérica continua entre menor y mayor [cite: 83, 85]
    if (opcion_p)
    {
        if (!solucion_encontrada)
        {
            cout << "<0>\n"; // [cite: 89]
        }
        else
        {
            cout << "<" << mejor_camino_str << ">\n";
        }
    }

    return 0;
}