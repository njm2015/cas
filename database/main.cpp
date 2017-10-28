#include <iostream>
#include <pthread.h>
#include <vector>

void usage() {
	std::cout << "./database <filename> <destination>" << std::endl;
}

int main(int argc, char **argv) {

	if(argc != 3) {
		usage();
		exit(1);
	}

	std::cout << "Correct args entered!" << std::endl;

	return 0;
}