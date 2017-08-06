#include <iostream>
#include <string>

#include "parser.h"

int main() {
	std::cout << "Welcome to Command CAS terminal. exit() to end program\n\n" << std::endl;

	while(true) {
		std::string query;
		std::cout << ">> ";
		std::getline(std::cin, query);

		std::cout << parse(query) << std::endl;
	}

	return 0;
}