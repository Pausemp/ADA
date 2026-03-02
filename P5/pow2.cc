// PAU SEMPERE MARTÍNEZ 20520990E

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

// Variable global para la cuenta de pasos de programa
unsigned long pasos = 0;

// coste: \Theta(n)
// Explicación: Reduce el problema restando 1. Hace 1 llamada recursiva por nivel.
unsigned long pow2_1(unsigned n) {
    pasos++;
    if (n == 0) return 1;
    return 2 * pow2_1(n - 1);
}

// coste: \Theta(\log n)
// Explicación: Reduce el problema a la mitad (n/2). Divide y vencerás óptimo.
unsigned long pow2_2(unsigned n) {
    pasos++;
    if (n == 0) return 1;
    unsigned long mitad = pow2_2(n / 2);
    if (n % 2 == 0) {
        return mitad * mitad;
    } else {
        return 2 * mitad * mitad;
    }
}

// coste: \Theta(2^n)
// Explicación: Genera 2 llamadas recursivas de tamaño n-1. Árbol binario completo.
unsigned long pow2_3(unsigned n) {
    pasos++;
    if (n == 0) return 1;
    // 2^n es lo mismo que 2^{n-1} + 2^{n-1}
    return pow2_3(n - 1) + pow2_3(n - 1);
}

int main() {
    // Archivo de salida para Gnuplot
    ofstream archivo_datos("pow2.dat");
    if (!archivo_datos) {
        cerr << "Error al crear el archivo de datos." << endl;
        return 1;
    }

    // Imprimir cabecera de la tabla por pantalla
    cout << setw(5)  << "n" 
         << setw(20) << "Pasos Theta(n)" 
         << setw(20) << "Pasos Theta(log n)" 
         << setw(20) << "Pasos Theta(2^n)" << endl;
    cout << string(65, '-') << endl;

    // Ejecutamos hasta n=25. Si pones un valor mucho más alto, Theta(2^n)
    // superará los 5 segundos límite establecidos por la práctica.
    for (unsigned n = 0; n <= 25; ++n) {
        unsigned long pasos1, pasos2, pasos3;
        unsigned long res1, res2, res3;
        unsigned long esperado = (unsigned long)(pow(2, n));

        // Analizar pow2_1
        pasos = 0;
        res1 = pow2_1(n);
        pasos1 = pasos;
        if (res1 != esperado) {
            cerr << "Error: pow2_1(" << n << ") incorrecto." << endl;
            return 1;
        }

        // Analizar pow2_2
        pasos = 0;
        res2 = pow2_2(n);
        pasos2 = pasos;
        if (res2 != esperado) {
            cerr << "Error: pow2_2(" << n << ") incorrecto." << endl;
            return 1;
        }

        // Analizar pow2_3
        pasos = 0;
        res3 = pow2_3(n);
        pasos3 = pasos;
        if (res3 != esperado) {
            cerr << "Error: pow2_3(" << n << ") incorrecto." << endl;
            return 1;
        }

        // Mostrar fila por pantalla
        cout << setw(5)  << n 
             << setw(20) << pasos1 
             << setw(20) << pasos2 
             << setw(20) << pasos3 << endl;

        // Escribir fila en el archivo de datos
        archivo_datos << n << " " << pasos1 << " " << pasos2 << " " << pasos3 << "\n";
    }

    archivo_datos.close();
    return 0;
}