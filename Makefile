CXX = g++
CXXFLAGS = -std=c++11
CPPFLAGS = -I eigen-git-mirror

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o main $(CPPFLAGS) main.cpp

clean:
	rm -f main
