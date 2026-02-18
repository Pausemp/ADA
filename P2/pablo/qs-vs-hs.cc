// Pablo Riquelme Gutiérrez 46081260R
/*
ADA. 2025-26
Practice 2: "Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

double pasos = 0;

//--------------------------------------------------------------
// Middle-Quicksort:
// The algorithm selects the middle element of the array as the "pivot".
// In a process called "partitioning", it rearranges the elements so that
// all elements smaller than the pivot are placed to its left, and
// all elements greater than the pivot are placed to its right.
// The process is then repeated recursively on the two resulting
// subarrays (left and right of the pivot).
//--------------------------------------------------------------

void middle_QuickSort(int *v, long left, long right)
{

    long i, j;
    int pivot;

    if (left < right)
    {
        i = left;
        j = right;
        pivot = v[(i + j) / 2];
        pasos++;
        // pivot based partitioning:
        do
        {
            while (v[i] < pivot)
            {
                i++;
                pasos++;
            }
            while (v[j] > pivot)
            {
                j--;
                pasos++;
            }
            if (i <= j)
            {
                swap(v[i], v[j]);
                i++;
                j--;
                pasos++;
            }
        } while (i <= j);
        // Repeat for each non-empty subarray:
        if (left < j)
            middle_QuickSort(v, left, j);
        if (i < right)
            middle_QuickSort(v, i, right);
    }
}

//--------------------------------------------------------------
// Heapsort:
// The algorithm works by repeatedly selecting the largest remaining element
// and placing it at the end of the vector in its correct position.
//
// To efficiently select the largest element, it builds a max-heap.
//
// The sink procedure is used for heap construction (or reconstruction).
//--------------------------------------------------------------

void sink(int *v, size_t n, size_t i)
// Sink an element (indexed by i) in a tree to maintain the heap property.
// n is the size of the heap.
{

    size_t largest;
    size_t l, r; // indices of left and right childs

    do
    {
        pasos++;

        largest = i;   // Initialize largest as root
        l = 2 * i + 1; // left = 2*i + 1
        r = 2 * i + 2; // right = 2*i + 2

        // If the left child exists and is larger than the root
        if (l < n && v[l] > v[largest])
        {
            largest = l;
        }

        // If the right child exists and is larger than the largest so far
        if (r < n && v[r] > v[largest])
        {
            largest = r;
        }

        // If the largest is still the root, the process is done
        if (largest == i)
        {
            break;
        }

        // Otherwise, swap the new largest with the current node i and repeat the process with the children
        swap(v[i], v[largest]);
        i = largest;
    } while (true);
}

//--------------------------------------------------------------
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    // Build a max-heap with the input array ("heapify"):
    // Starting from the last non-leaf node (right to left), sink each element to construct the heap.
    for (size_t i = n / 2 - 1; true; i--)
    {
        pasos++;
        sink(v, n, i);
        if (i == 0)
        {
            break; // As size_t is an unsigned type
        }
    }

    // At this point, we have a max-heap. Now, sort the array:
    // Repeatedly swap the root (largest element) with the last element and rebuild the heap.
    for (size_t i = n - 1; i > 0; i--)
    {
        pasos++;
        // Move the root (largest element) to the end by swapping it with the last element.
        swap(v[0], v[i]);
        // Rebuild the heap by sinking the new root element.
        // Note that the heap size is reduced by one in each iteration (so the element moved to the end stays there)
        sink(v, i, 0);
        // The process ends when the heap has only one element, which is the smallest and remains at the beginning of the array.
    }
}

int main(void)
{

    srand(0);
    double millones_pasos;

    cout << "# QUICKSORT VERSUS HEAPSORT."
         << endl
         << "# Average processing Msteps (millions of program steps)"
         << endl
         << "# Number of samples (arrays of integer): 30"
         << endl
         << "# \t\t   RANDOM ARRAYS\tSORTED ARRAYS\t  REVERSE SORTED ARRAYS"
         << endl
         << "# \t\t-------------------  -------------------  ----------------------"
         << endl
         << "# \tSize\tQuickSort  HeapSort  QuickSort  HeapSort  QuickSort     HeapSort"
         << endl
         << "#==============================================================================="
         << endl;

    for (int exp = 15; exp <= 20; exp++)
    {
        size_t size = size_t(pow(2, exp));

        int *v_aleatorio = new int[size];
        int *v_creciente = new int[size];
        int *v_decreciente = new int[size];

        if (!v_aleatorio)
        {
            cerr << "Error, not enough memory!" << endl;
            exit(EXIT_FAILURE);
        }

        if (!v_creciente)
        {
            cerr << "Error, not enough memory!" << endl;
            exit(EXIT_FAILURE);
        }

        if (!v_decreciente)
        {
            cerr << "Error, not enough memory!" << endl;
            exit(EXIT_FAILURE);
        }

        cout << size << "\t";

        for (int i = 0; i < 30; i++)
        {
            for (size_t j = 0; j < size; j++)
                v_aleatorio[j] = rand();

            middle_QuickSort(v_aleatorio, 0, size - 1);
        }

        for (size_t i = 1; i < size; i++)
        {
            if (v_aleatorio[i] < v_aleatorio[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }

        millones_pasos = pasos / 1000000.0;

        cout << "\t" << millones_pasos / 30;

        pasos = 0;

        for (int i = 0; i < 30; i++)
        {
            for (size_t j = 0; j < size; j++)
                v_aleatorio[j] = rand();

            heapSort(v_aleatorio, size);
        }

        for (size_t i = 1; i < size; i++)
        {
            if (v_aleatorio[i] < v_aleatorio[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }
        delete[] v_aleatorio;
        millones_pasos = pasos / 1000000.0;

        cout << "    " << millones_pasos / 30;

        pasos = 0;

        for (size_t i = 0; i < size; i++)
        { // para ordenados crecientemente, con quicksort
            v_creciente[i] = i;
        }

        middle_QuickSort(v_creciente, 0, size - 1);

        for (size_t i = 1; i < size; i++)
        {
            if (v_creciente[i] < v_creciente[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }

        millones_pasos = pasos / 1000000.0;

        cout << "   " << millones_pasos;
        pasos = 0;

        for (size_t i = 0; i < size; i++)
        { // para ordenados crecientemente, con heapsort
            v_creciente[i] = i;
        }

        heapSort(v_creciente, size);

        for (size_t i = 1; i < size; i++)
        {
            if (v_creciente[i] < v_creciente[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }
        delete[] v_creciente;
        millones_pasos = pasos / 1000000.0;

        cout << "   " << millones_pasos;
        pasos = 0;

        for (size_t i = 0; i < size; i++)
        { // para ordenados decrecientemente, con quicksort
            v_decreciente[i] = size - i;
        }

        middle_QuickSort(v_decreciente, 0, size - 1);

        for (size_t i = 1; i < size; i++)
        {
            if (v_decreciente[i] < v_decreciente[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }
        millones_pasos = pasos / 1000000.0;

        cout << "   " << millones_pasos;
        pasos = 0;

        for (size_t i = 0; i < size; i++)
        { // para ordenados decreciente, con heapsort
            v_decreciente[i] = size - i;
        }

        heapSort(v_decreciente, size);

        for (size_t i = 1; i < size; i++)
        {
            if (v_decreciente[i] < v_decreciente[i - 1])
            {
                cerr << "Panic, array not sorted! " << i << endl;
                exit(EXIT_FAILURE);
            }
        }
        delete[] v_decreciente;
        millones_pasos = pasos / 1000000.0;

        cout << "   " << millones_pasos << endl;
        pasos = 0;
    }
}