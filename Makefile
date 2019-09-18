COMP = g++ -std=c++17
CFLAG = -c -g

all: boarding

boarding: main.cpp
	$(COMP) -o boarding main.cpp

clean:
	rm -f boarding *.o