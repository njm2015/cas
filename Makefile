CXX = g++
CXXFLAGS = -Wall -g -pg `xml2-config --cflags --libs` `pkg-config libxml++-2.6 --cflags --libs` --std=c++1y -Wno-inconsistent-missing-override


LDFLAGS = -lcurl `pkg-config libxml++-2.6 --cflags --libs`

OBJECTS = main.o parser.o msc_func_list.o get_func_list.o print_func_list.o 

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o main $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

msc_func_list.o: msc_func_list.cpp msc_func_list.h
	$(CXX) $(CXXFLAGS) -c msc_func_list.cpp

get_func_list.o: get_func_list.cpp get_func_list.h
	$(CXX) $(CXXFLAGS) -c get_func_list.cpp

print_func_list.o: print_func_list.cpp print_func_list.h
	$(CXX) $(CXXFLAGS) -c print_func_list.cpp



clean:
	rm *.o main