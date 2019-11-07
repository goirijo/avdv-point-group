CXX = g++
CPPFLAGS = -I eigen-git-mirror

all: main

main: main.cpp
	$(CXX) -o main $(CPPFLAGS) main.cpp

clean:
	rm -f main
