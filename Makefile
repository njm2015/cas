CXX = g++
CXXFLAGS = -g -Wall -std=c++11

OBJECTS = main.o parser.o func.o func_list.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o main

main.o: main.cpp
	$(CXX) -c main.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

func.o: func.cpp func.h
	$(CXX) -c func.cpp

func_list.o: func_list.cpp func_list.h
	$(CXX) -c func_list.cpp

clean:
	rm *.o main