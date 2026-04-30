# PAU SEMPERE MARTINEZ 20520990E

CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11
DNI = 20520990E

maze_greedy: maze_greedy.cc
	$(CXX) $(CXXFLAGS) -o maze_greedy maze_greedy.cc

tar: maze_greedy.cc makefile
	tar -cvzf $(DNI).tar.gz maze_greedy.cc makefile
clean:
	rm -f maze_greedy