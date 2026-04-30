# PAU SEMPERE MARTINEZ 20520990E

CXX = g++-13
CXXFLAGS = -Wall -Wextra -O2 -std=c++11
DNI = 20520990E

maze_greedy: maze_bt.cc
	$(CXX) $(CXXFLAGS) -o maze_bt maze_bt.cc

tar: maze_greedy.cc makefile
	tar -cvzf $(DNI).tar.gz maze_bt.cc makefile
clean:
	rm -f maze_bt