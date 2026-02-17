// PAU SEMPERE MARTÍNEZ 20520990E

/*
ADA 2025-26
Práctica 1: "Empirical analysis of Quicksort average-time complexity."
*/

//--------------------------------------------------------------
// Middle QuickSort

#include <unistd.h>
#include <iostream>
#include <cmath>

using namespace std;

void middle_QuickSort(int *v, long left, long right)
{

    long i, j;
    int pivot, aux;
    if (left < right)
    {
        i = left;
        j = right;
        pivot = v[(i + j) / 2];
        do
        {
            while (v[i] < pivot)
                i++;
            while (v[j] > pivot)
                j--;
            if (i <= j)
            {
                aux = v[i];
                v[i] = v[j];
                v[j] = aux;
                i++;
                j--;
            }
        } while (i <= j);
        if (left < j)
            middle_QuickSort(v, left, j);
        if (i < right)
            middle_QuickSort(v, i, right);
    }
}

int main(void)
{
    srand(0);

    cout << "# QuickSort CPU-times in milliseconds:" << endl
         << "# Size \t Average CPU time (ms.)" << endl
         << "# ----------------------------" << endl;

    for (int exp = 15; exp <= 20; exp++)
    {
        size_t size = size_t(pow(2, exp));

        double tiempo = 0;

        int *v = new int[size];

        if (!v)
        {
            cerr << "Error, not enough memory!" << endl;
            exit(EXIT_FAILURE);
        }

        cout << size << "\t\t" << std::flush;

        for (int r = 0; r < 30; r++)
        {

            for (size_t j = 0; j < size; j++)
            {
                v[j] = rand();
            }

            auto start = clock();
            middle_QuickSort(v, 0, size - 1);
            auto end = clock();

            tiempo += 1000.0 * (end - start) / CLOCKS_PER_SEC;
        }

        cout << tiempo / 30 << endl;

        for (size_t i = 0; i < size; i++)
        {
            if (v[i] < v[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }
        delete[] v;
    }
}