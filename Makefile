CXX = g++
CXXFLAGS = -g -Wall 

main: main.o parser.o
	$(CXX) $(CXXFLAGS) main.o parser.o -o main

main.o: main.cpp
	$(CXX) -c main.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

clean:
	rm *.o main