# PAU SEMPERE MARTINEZ 20520990E

CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++11
DNI = 20520990E

all: maze_bb

maze_bb: maze_bb.cc
	$(CXX) $(CXXFLAGS) -o maze_bb maze_bb.cc

tar: maze_bb.cc makefile memoria.pdf
	tar -cvzf $(DNI).tar.gz maze_bb.cc makefile memoria.pdf

clean:
	rm -f maze_bb
