#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <stdexcept>

#include "parser.h"
#include "func_list.h"

enum StringValue { zero, exitt, get, getSym, getEPS };

static std::map<std::string, StringValue> mapFuncVals;

void initialize();
void choose(StringValue query);

int main() {

	initialize();

	std::cout << "Welcome to Command CAS terminal. exit() to end program\n\n" << std::endl;

	while(true) {
		std::string query;
		std::cout << ">> ";
		std::getline(std::cin, query);

		std::transform(query.begin(), query.end(), query.begin(), ::tolower);

		std::string curr_func = parse_func(query);
	
		std::vector<std::string> args = parse_args(query);

		std::cout << "[args]: ";

		for(auto it = args.begin(); it != args.end(); it++) {
			std::cout << *it << std::endl;
		}

		std::cout << std::endl;
	}

	return 0;
}

void choose(StringValue query) {

	switch (query) {
		case zero:
			unknown_func();
			break;
		case exitt:
			exit(0);
			break;
		case get:
			get_info();
			break;
	}
}

void initialize() {
	mapFuncVals["exit"] = exitt;
	mapFuncVals["get"] = get;
	mapFuncVals["getsym"] = getSym;
	mapFuncVals["geteps"] = getEPS;
}