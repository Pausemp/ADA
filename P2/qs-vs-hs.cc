// PAU SEMPERE MARTÍNEZ 20520990E

/*
ADA. 2025-26
Practice 2: "Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/

#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

double PASOS = 0;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int *v, long left, long right)
{
    PASOS++;
    long i, j;
    int pivot;

    if (left < right)
    {
        i = left;
        j = right;
        pivot = v[(i + j) / 2];
        do
        {
            PASOS++;
            while (v[i] < pivot)
            {
                i++;
                PASOS++;
            }
            while (v[j] > pivot)
            {
                j--;
                PASOS++;
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
    PASOS++;
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
        PASOS++;

    } while (true);
}

//--------------------------------------------------------------
// Heapsort algorithm
void heapSort(int *v, size_t n)
{
    PASOS++;
    for (size_t i = n / 2 - 1;; i--)
    {
        sink(v, n, i);
        PASOS++;
        if (i == 0)
            break;
    }
    for (size_t i = n - 1; i > 0; i--)
    {
        swap(v[0], v[i]);
        sink(v, i, 0);
        PASOS++;
    }
}

int main(void)
{
    srand(0);

    // Configuración de formato
    cout.setf(ios::fixed);
    cout.precision(3);

    cout << "# QUICKSORT VERSUS HEAPSORT." << endl;
    cout << "# Average processing M (millions of program )" << endl;
    cout << "# Number of samples (arrays of integer): 30" << endl;

    cout << "#\t\t\t    RANDOM ARRAYS\t\t   SORTED ARRAYS\t\tREVERSE SORTED ARRAYS" << endl;

    cout << "#\t\t\t-------------------\t\t-------------------\t\t---------------------" << endl;

    cout << "#Size\t\tQuickSort\tHeapSort\tQuickSort\tHeapSort\tQuickSort\tHeapSort" << endl;

    cout << "#====================================================================================" << endl;

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

        cout << size << "\t";

        // --- 1. RANDOM ---
        double pasosTotalesQS = 0;
        double pasosTotalesHS = 0;

        for (int i = 0; i < 30; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                int val = rand();
                vecAleatorio[j] = val;
                vecAleatorioCopia[j] = val;
            }

            PASOS = 0;
            middle_QuickSort(vecAleatorio, 0, size - 1);
            pasosTotalesQS += PASOS;

            PASOS = 0;
            heapSort(vecAleatorioCopia, size);
            pasosTotalesHS += PASOS;
        }

        cout << "\t" << (pasosTotalesQS / 30.0) / 1000000.0 << "\t";
        cout << "\t" << (pasosTotalesHS / 30.0) / 1000000.0 << "\t";

        // --- 2. SORTED ---
        for (size_t j = 0; j < size; j++)
        {
            vecOrdenado[j] = j;
        }

        PASOS = 0;
        middle_QuickSort(vecOrdenado, 0, size - 1);
        cout << "\t" << PASOS / 1000000.0 << "\t\t";

        for (size_t j = 0; j < size; j++)
        {
            vecOrdenado[j] = j;
        }
        PASOS = 0;
        heapSort(vecOrdenado, size);
        cout << PASOS / 1000000.0 << "\t\t";

        // --- 3. REVERSE ---
        for (size_t j = 0; j < size; j++)
        {
            vecOrdenadoInv[j] = size - j;
        }

        PASOS = 0;
        middle_QuickSort(vecOrdenadoInv, 0, size - 1);
        cout << PASOS / 1000000.0 << "\t\t";

        for (size_t j = 0; j < size; j++)
        {
            vecOrdenadoInv[j] = size - j;
        }

        PASOS = 0;
        heapSort(vecOrdenadoInv, size);
        cout << PASOS / 1000000.0 << endl;

        delete[] vecAleatorio;
        delete[] vecAleatorioCopia;
        delete[] vecOrdenado;
        delete[] vecOrdenadoInv;
    }

    return 0;
}