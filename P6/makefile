# PAU SEMPERE MARTINEZ 20520990E
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
DNI = 20520990E

maze: maze.cc
	$(CXX) $(CXXFLAGS) -o maze maze.cc

# Objetivo para crear el archivo de entrega
tar: maze.cc makefile
	tar -cvzf $(DNI).tar.gz maze.cc makefile

clean:
	rm -f maze $(DNI).tar.gz