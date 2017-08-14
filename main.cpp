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

double choose(StringValue function, std::vector<std::string> args);

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

		double price = choose(mapFuncVals[parse_func(query)], parse_args(query));

		if(price != -1) {
			std::cout << price << std::endl;
		}
	}

	return 0;
}

double choose(StringValue function, std::vector<std::string> args) {

	switch (function) {
		case zero:
			unknown_func();
			break;
		case exitt:
			exit(0);
			break;
		case getprice:
			return get_price(args);
			break;
	}
}

void initialize() {
	mapFuncVals["exit"] = exitt;
	mapFuncVals["getprice"] = getprice;
}