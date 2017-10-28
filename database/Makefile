EXENAME = run

CXX = g++
CXXFLAGS = -Wall -g --std=c++1y


LDFLAGS = 

OBJECTS = main.o Date.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXENAME) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Date.o: Date.cpp Date.h
	$(CXX) $(CXXFLAGS) -c Date.cpp

clean:
	rm *.o run

