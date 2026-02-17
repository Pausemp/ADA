//OSCAR CASADO LORENZO 77580351V

#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

double STEPS = 0;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int * v, long left, long right){
	STEPS++;
	
	long i,j;
	int pivot; 
	
	if (left<right){
		
		i=left; j=right;
		pivot=v[(i+j)/2];
		
		do{
			while (v[i]<pivot){ i++; STEPS++;}
			while (v[j]>pivot){ j--; STEPS++;}
			
			if (i<=j){ 
				swap(v[i], v[j]);
				i++; j--;
			}	
		}while (i<=j);

		if (left < j){  middle_QuickSort(v,left,j);}
		if (i < right){ middle_QuickSort(v,i,right);}
	}
	
	STEPS++;
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i){
	STEPS++;
	size_t largest;
	size_t l, r; //left and right child

	do{

		largest = i;  // Initialize largest as root
		l = 2*i + 1;  // left = 2*i + 1
		r = 2*i + 2;  // right = 2*i + 2

		// Is left child larger than root?
		if (l < n && v[l] > v[largest]){
			largest = l;
		}
	 
		// Is right child larger than largest so far?
		if (r < n && v[r] > v[largest]){
			largest = r;
		}
		
		// If largest is still root then the process is done
		if (largest == i) {break;}
		
		// If not, swap new largest with current node i and repeat the process with childs. 
		swap(v[i], v[largest]);
		i=largest;  
		
		STEPS++;
		
	} while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n){
	
	STEPS++;
	
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; ; i--){
        sink(v, n, i);
        STEPS++;
		if (i==0) break; //as size_t is unsigned type
	}	

	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by one swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
		// The procedure ends when HEAP has only one element, the lower of the input array
		STEPS++;
	}
}




int main(void){
	
    srand(0);

    cout << "# QUICKSORT VERSUS HEAPSORT"
		<< endl
		<< "# Average processing Msteps (millions of program steps)"
        << endl
        << "# Number of samples (arrays of integer): 30"
        << endl
        << "# \t\t RANDOM ARRAYS \t\t\t SORTED ARRAYS \t\t\t REVERSE SORTED ARRAYS"
        << endl
        << "# \t\t ------------------------ \t ------------------------ \t ------------------------"
        << endl
        << "# Size \t\t QuickSort \t HeapSort \t QuickSort \t HeapSort \t QuickSort \t HeapSort"
        << endl
        << "# ======================================================================================================="
        << endl;

    for (int exp = 15; exp <= 21; exp++){
        size_t size = size_t( pow(2,exp) );
        int* vAl = new int [size];
		int* vOrd = new int [size];
		int* vOrdInversa = new int [size];

        if (!vAl){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        if (!vOrd){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        if (!vOrdInversa){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush;


        for(unsigned i = 0; i<30; i++){
            for (size_t j = 0; j < size; j++) 
                vAl[j] = rand(); 

            middle_QuickSort(vAl, 0, size - 1);
            
        }

		cout << (STEPS/1000000)/30 << std::flush;
		cout << "\t";
		STEPS = 0;


        for(unsigned i = 0; i<30; i++){	//lo hacemos 30 veces y una media porqeu es el aleatorio
        
            for (size_t j = 0; j < size; j++) 
                vAl[j] = rand(); 

            heapSort(vAl, size);
            
        }

		cout << (STEPS/1000000)/30 << std::flush;
		cout << "\t";
		STEPS = 0;

        for (size_t j = 0; j < size; j++) 
            vOrd[j] = j; 

		for (size_t j = 0; j < size; j++) //29 28 ... 1 0
            vOrdInversa[j] = (size - 1) - j; 


		middle_QuickSort(vOrd, 0, size - 1);
		cout << (STEPS/1000000) << std::flush;
		cout << "\t";
		STEPS = 0;

		heapSort(vOrd, size);
		cout << (STEPS/1000000) << std::flush;
		cout << "\t";
		STEPS = 0;


		middle_QuickSort(vOrdInversa, 0, size - 1);
		cout << (STEPS/1000000) << std::flush;
		cout << "\t";
		STEPS = 0;

		for (size_t j = 0; j < size; j++) //29 28 ... 1 0
			vOrdInversa[j] = (size - 1) - j; 

		heapSort(vOrdInversa, size);
		cout << (STEPS/1000000) << std::flush;
		cout << endl;
		STEPS = 0;

		for (size_t i = 1; i < size; i++)
			if (vAl[i] < vAl[i-1]){ 
				cerr << "Panic, array not sorted! " << i << endl; 
				exit(EXIT_FAILURE);            
			}

        delete[] vAl; 

		for (size_t i = 1; i < size; i++)
			if (vOrd[i] < vOrd[i-1]){ 
				cerr << "Panic, array not sorted! " << i << endl; 
				exit(EXIT_FAILURE);            
			}

        delete[] vOrd; 

		for (size_t i = 1; i < size; i++)
			if (vOrdInversa[i] < vOrdInversa[i-1]){ 
				cerr << "Panic, array not sorted! " << i << endl; 
				exit(EXIT_FAILURE);            
			}

        delete[] vOrdInversa;         
    }
}

