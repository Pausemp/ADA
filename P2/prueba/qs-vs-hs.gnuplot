#OSCAR CASADO LORENZO 77580351V

set terminal png
set output "quickSort.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "QuickSort"

plot "qs-vs-hs.Msteps" using 1:2 with lines title "random arrays" ,\
"qs-vs-hs.Msteps" using 1:4 with lines title "sorted arrays" ,\
"qs-vs-hs.Msteps" using 1:6 with lines title "reverse-sorted arrays" ,\

#==============================================================================

set terminal png
set output "heapSort.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "HeapSort"

plot "qs-vs-hs.Msteps" using 1:3 with lines title "random arrays" ,\
"qs-vs-hs.Msteps" using 1:5 with lines title "sorted arrays" ,\
"qs-vs-hs.Msteps" using 1:7 with lines title "reverse-sorted arrays" ,\

#==============================================================================

#Comparativa del número de pasos entre vectores con con vectores aleatorios
set terminal png
set output "qs-vs-hs-RA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (random arrays)"

y(x)=a*x*log(x) + b 
fit y(x) "qs-vs-hs.Msteps" using 1:2 via a,b
z(x)=c*x*log(x) + d 
fit z(x) "qs-vs-hs.Msteps" using 1:3 via c,d


plot "qs-vs-hs.Msteps" using 1:2 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:3 with lines title "HeapSort" ,\
y(x) title "fitting QuickSort time value to..." ,\
z(x) title "fitting HeapSort time value to..."

#==============================================================================

#Comparativa del número de pasos entre vectores con con vectores ordenados
set terminal png
set output "qs-vs-hs-SA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (sorted arrays)"

y(x)=e*x*log(x) + f 
fit y(x) "qs-vs-hs.Msteps" using 1:4 via e,f
z(x)=g*x*log(x) + h 
fit z(x) "qs-vs-hs.Msteps" using 1:5 via g,h


plot "qs-vs-hs.Msteps" using 1:4 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:5 with lines title "HeapSort" ,\
y(x) title "fitting QuickSort time value to..." ,\
z(x) title "fitting HeapSort time value to..."

#==============================================================================

#Comparativa del número de pasos entre vectores con con vectores invertidos
set terminal png
set output "qs-vs-hs-RSA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (reverse-sorted arrays)"
 
y(x)=i*x*log(x) + j 
fit y(x) "qs-vs-hs.Msteps" using 1:6 via i,j
z(x)=k*x*log(x) + l 
fit z(x) "qs-vs-hs.Msteps" using 1:7 via k,l


plot "qs-vs-hs.Msteps" using 1:6 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:7 with lines title "HeapSort" ,\
y(x) title "fitting QuickSort time value to..." ,\
z(x) title "fitting HeapSort time value to..."
