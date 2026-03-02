# PAU SEMPERE MARTÍNEZ 20520990E

OPTS = -Wall -Wextra -O0 -std=c++11

all: pow2 pow2.dat graphs

graphs: pow2.dat pow2.gpi
	gnuplot pow2.gpi

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	 

pow2.dat: pow2
	./pow2

tar: pow2.cc pow2.gpi makefile
	tar cvfz 20520990E.tar.gz pow2.cc pow2.gpi makefile

clean: 
	rm -f pow2 pow2.dat fit.log pow2*.png *~