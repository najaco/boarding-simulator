COMP = g++ -std=c++17
CFLAG = -Wall -g -o
OPT_FLAGS = -O0

all: boarding

boarding: main.cpp
	$(COMP) $(OPT_FLAGS) $(CFLAG) boarding main.cc plane.h

clean:
	rm -f boarding *.o