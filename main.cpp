#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <curl/curl.h>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

#include <time.h>

#include "parser.h"
#include "func_list.h"

enum StringValue { zero, exitt, getprice };

static std::map<std::string, StringValue> mapFuncVals;

void choose(StringValue function, std::vector<std::string> args);

void initialize();

int main() {

	initialize();

	std::cout << "Welcome to Command CAS terminal. exit() to end program\n\n" << std::endl;

	while(true) {
		std::string query;
		std::cout << std::endl << ">> ";
		std::getline(std::cin, query);

		query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());

		std::transform(query.begin(), query.end(), query.begin(), ::tolower);

		choose(mapFuncVals[parse_func(query)], parse_args(query));
	}

	return 0;
}

void choose(StringValue function, std::vector<std::string> args) {

	switch (function) {
		case zero:
			unknown_func();
			return;
		case exitt:
			exit(0);
			return;
		case getprice:
			get_price(args);
			return;
	}
}

void initialize() {
	mapFuncVals["exit"] = exitt;
	mapFuncVals["getprice"] = getprice;
}