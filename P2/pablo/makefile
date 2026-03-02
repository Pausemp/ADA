#PAU SEMPERE MARTÍNEZ 20520990E


OPTS = -Wall -O3 -std=c++0x

qs-vs-hs: qs-vs-hs.cc
	g++-13 ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.Msteps: qs-vs-hs
	./qs-vs-hs > qs-vs-hs.Msteps

graphs: qs-vs-hs.Msteps qs-vs-hs.gpi
	gnuplot qs-vs-hs.gpi

all: qs-vs-hs qs-vs-hs.Msteps

tar: qs-vs-hs.cc qs-vs-hs.gpi makefile
	tar cvfz 20520990E.tar.gz qs-vs-hs.cc qs-vs-hs.gpi makefile

clean: 
	-rm qs-vs-hs qs-vs-hs.Msteps fit.log *.png 
 
