CXX = g++
CXXFLAGS = -std=c++17 -Wall -pedantic
LDXXFLAGS = -lncurses -lpthread

OBJS = main.o gameboard.o logger.o 

prog: $(OBJS) # Achtung: Einrücken mit Tabs, nicht mit Leerzeichen!
				$(CXX) $(CXXFLAGS) -o sokoban $(OBJS) $(LDXXFLAGS)

%.o: %.cpp
				$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
				$(RM) *.o
