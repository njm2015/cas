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

		query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());

		int price = get_price(query);
	}

	return 0;
}

void choose(StringValue query) {

	std::string temp = "";

	switch (query) {
		case zero:
			unknown_func();
			break;
		case exitt:
			exit(0);
			break;
		case get:
			get_price(temp);
			break;
	}
}

void initialize() {
	mapFuncVals["exit"] = exitt;
	mapFuncVals["get"] = get;
	mapFuncVals["getsym"] = getSym;
	mapFuncVals["geteps"] = getEPS;
}