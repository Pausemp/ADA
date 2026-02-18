/*
ADA. 2025-26
Practice 2: "Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
OSCAR CASADO LORENZO 77580351V
*/

#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

double STEPS = 0;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int *v, long left, long right)
{
    STEPS++;
    long i, j;
    int pivot;

    if (left < right)
    {
        i = left;
        j = right;
        pivot = v[(i + j) / 2];
        do
        {
            STEPS++;
            while (v[i] < pivot)
            {
                i++;
                STEPS++;
            }
            while (v[j] > pivot)
            {
                j--;
                STEPS++;
            }

            if (i <= j)
            {
                swap(v[i], v[j]);
                i++;
                j--;
            }
        } while (i <= j);

        if (left < j)
        {
            middle_QuickSort(v, left, j);
        }
        if (i < right)
        {
            middle_QuickSort(v, i, right);
        }
    }
}

//--------------------------------------------------------------
// HEAPSORT - Sink Procedure
void sink(int *v, size_t n, size_t i)
{
    STEPS++;
    size_t largest;
    size_t l, r;

    do
    {
        largest = i;
        l = 2 * i + 1;
        r = 2 * i + 2;

        if (l < n && v[l] > v[largest])
            largest = l;
        if (r < n && v[r] > v[largest])
            largest = r;

        if (largest == i)
            break;

        swap(v[i], v[largest]);
        i = largest;
        STEPS++;

    } while (true);
}

//--------------------------------------------------------------
// Heapsort algorithm
void heapSort(int *v, size_t n)
{
    STEPS++;
    for (size_t i = n / 2 - 1;; i--)
    {
        sink(v, n, i);
        STEPS++;
        if (i == 0)
            break;
    }
    for (size_t i = n - 1; i > 0; i--)
    {
        swap(v[0], v[i]);
        sink(v, i, 0);
        STEPS++;
    }
}

int main(void)
{
    srand(0);

    // Configuración de formato
    cout.setf(ios::fixed);
    cout.precision(3);

    cout << "# QUICKSORT VERSUS HEAPSORT." << endl;
    cout << "# Average processing Msteps (millions of program steps)" << endl;
    cout << "# Number of samples (arrays of integer): 30" << endl;

    cout << "#\t\t   RANDOM ARRAYS\t\t   SORTED ARRAYS\t\t   REVERSE SORTED" << endl;

    cout << "#\t\t -----------------\t\t -----------------\t\t -----------------" << endl;

    cout << "# Size\t\tQuickSort\tHeapSort\tQuickSort\tHeapSort\tQuickSort\tHeapSort" << endl;

    cout << "#==================================================================================================" << endl;

    for (int exp = 15; exp <= 20; exp++)
    {
        size_t size = size_t(pow(2, exp));

        int *vecAleatorio = new int[size];
        int *vecAleatorioCopia = new int[size];
        int *vecOrdenado = new int[size];
        int *vecOrdenadoInv = new int[size];

        if (!vecAleatorio || !vecAleatorioCopia || !vecOrdenado || !vecOrdenadoInv)
        {
            cerr << "Error, not enough memory! " << endl;
            exit(EXIT_FAILURE);
        }

        // Imprimir Tamaño con doble tabulador
        cout << size << "\t" << flush;

        // --- 1. RANDOM ---
        double totalStepsQS = 0;
        double totalStepsHS = 0;

        for (unsigned i = 0; i < 30; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                int val = rand();
                vecAleatorio[j] = val;
                vecAleatorioCopia[j] = val;
            }

            STEPS = 0;
            middle_QuickSort(vecAleatorio, 0, size - 1);
            totalStepsQS += STEPS;

            STEPS = 0;
            heapSort(vecAleatorioCopia, size);
            totalStepsHS += STEPS;
        }

        // AQUI ESTA EL CAMBIO CLAVE: Usamos "\t\t" (doble) después de cada número
        // para compensar que "QuickSort" es una palabra larga.
        cout << (totalStepsQS / 30.0) / 1000000.0 << "\t";
        cout << (totalStepsHS / 30.0) / 1000000.0 << "\t"; // Doble tab para saltar al siguiente bloque

        // --- 2. SORTED ---
        for (size_t j = 0; j < size; j++)
            vecOrdenado[j] = j;

        STEPS = 0;
        middle_QuickSort(vecOrdenado, 0, size - 1);
        cout << STEPS / 1000000.0 << "\t"; // Doble tab

        for (size_t j = 0; j < size; j++)
            vecOrdenado[j] = j;
        STEPS = 0;
        heapSort(vecOrdenado, size);
        cout << STEPS / 1000000.0 << "\t"; // Doble tab para saltar al siguiente bloque

        // --- 3. REVERSE ---
        for (size_t j = 0; j < size; j++)
            vecOrdenadoInv[j] = (size - 1) - j;

        STEPS = 0;
        middle_QuickSort(vecOrdenadoInv, 0, size - 1);
        cout << STEPS / 1000000.0 << "\t"; // Doble tab

        for (size_t j = 0; j < size; j++)
            vecOrdenadoInv[j] = (size - 1) - j;
        STEPS = 0;
        heapSort(vecOrdenadoInv, size);
        cout << STEPS / 1000000.0 << endl; // Aquí solo endl

        delete[] vecAleatorio;
        delete[] vecAleatorioCopia;
        delete[] vecOrdenado;
        delete[] vecOrdenadoInv;
    }

    return 0;
}