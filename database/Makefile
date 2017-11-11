EXENAME = run

CXX = g++
CXXFLAGS = -Wall -g --std=c++1y


LDFLAGS = -lcurl

OBJECTS = main.o Date.o write.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXENAME) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Date.o: Date.cpp Date.h
	$(CXX) $(CXXFLAGS) -c Date.cpp

write.o: write.cpp write.h
	$(CXX) $(CXXFLAGS) -c write.cpp

clean:
	rm *.o run

