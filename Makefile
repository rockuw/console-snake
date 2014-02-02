CFLAGS=-Wall
LIBS=-lncurses
CXX=g++

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

all: snake
snake: map.o snake.o game.o main.o
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm *.o
	rm snake
