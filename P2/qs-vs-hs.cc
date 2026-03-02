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
        // pivot based partitioning:
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
        PASOS++;

        largest = i;   // Initialize largest as root
        l = 2 * i + 1; // left = 2*i + 1
        r = 2 * i + 2; // right = 2*i + 2
        // If the left child exists and is larger than the root

        if (l < n && v[l] > v[largest])
            largest = l;

        // If the right child exists and is larger than the largest so far
        if (r < n && v[r] > v[largest])
            largest = r;

        // If the largest is still the root, the process is done
        if (largest == i)
            break;

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
    for (size_t i = n / 2 - 1;; i--)
    {
        sink(v, n, i);
        if (i == 0)
            break; // As size_t is an unsigned type
    }

    // At this point, we have a max-heap. Now, sort the array:
    // Repeatedly swap the root (largest element) with the last element and rebuild the heap.
    for (size_t i = n - 1; i > 0; i--)
    {
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

    cout.setf(ios::fixed);
    cout.precision(3);

    cout << "# QUICKSORT VERSUS HEAPSORT.\n";
    cout << "# Average processing M (millions of program )\n";
    cout << "# Number of samples (arrays of integer): 30\n";

    cout << "#\t\t\t    RANDOM ARRAYS\t\t   SORTED ARRAYS\t\tREVERSE SORTED ARRAYS\n";

    cout << "#\t\t\t-------------------\t\t-------------------\t\t---------------------\n";

    cout << "#Size\t\tQuickSort\tHeapSort\tQuickSort\tHeapSort\tQuickSort\tHeapSort\n";

    cout << "#====================================================================================\n";

    for (int exp = 15; exp <= 20; exp++)
    {
        size_t size = size_t(pow(2, exp));

        int *v1 = new int[size];
        int *vCopia = new int[size];

        if (!v1 || !vCopia)
        {
            cerr << "Error, not enough memory! " << endl;
            exit(EXIT_FAILURE);
        }

        cout << size << "\t";

        double pasosTotalesQS = 0;
        double pasosTotalesHS = 0;

        for (int i = 0; i < 30; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                int val = rand();
                v1[j] = val;
                vCopia[j] = val;
            }

            PASOS = 0;
            middle_QuickSort(v1, 0, size - 1);
            pasosTotalesQS += PASOS;

            PASOS = 0;
            heapSort(vCopia, size);
            pasosTotalesHS += PASOS;
        }

        cout << "\t" << (pasosTotalesQS / 30.0) / 1000000.0 << "\t";
        cout << "\t" << (pasosTotalesHS / 30.0) / 1000000.0 << "\t";
        
        for (size_t j = 0; j < size; j++)
        {
            v1[j] = j;
            vCopia[j] = v1[j];
        }
        
        PASOS = 0;
        middle_QuickSort(v1, 0, size - 1);
        cout << "\t" << PASOS / 1000000.0 << "\t\t";   

        PASOS = 0;
        heapSort(vCopia, size);
        cout << PASOS / 1000000.0 << "\t\t";

        for (size_t j = 0; j < size; j++)
        {
            v1[j] = size - j;
            vCopia[j] = v1[j];
        }

        PASOS = 0;
        middle_QuickSort(v1, 0, size - 1);
        cout << PASOS / 1000000.0 << "\t\t";

        PASOS = 0;
        heapSort(vCopia, size);
        cout << PASOS / 1000000.0 << endl;

        delete[] v1;
        delete[] vCopia;
    }

    return 0;
}