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

enum StringValue { zero, exitt, getPrice };

static std::map<std::string, StringValue> mapFuncVals;

void initialize();
void choose(StringValue query, std::vector<std::string> args);

int main() {

	initialize();

	std::cout << "Welcome to Command CAS terminal. exit() to end program\n\n" << std::endl;

	//while(true) {
		std::string query;
		std::cout << ">> ";
		std::getline(std::cin, query);

		query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());

		std::transform(query.begin(), query.end(), query.begin(), ::tolower);
/*
		double price = get_price(parse_func(query), parse_args(query));

		std::cout << price << std::endl;
*/
		tm* date = new tm();
		date->tm_mday = 13;
		date->tm_mon = 8;
		date->tm_year = 2017;

		int day = day_of_week(date);

		std::cout << day << std::endl;

		delete date;
/*
		std::string func = parse_func(query);
		std::vector<std::string> args = parse_args(query);

		std::cout << "[function]: " << func << std::endl;
		std::cout << "[args]: ";

		for(auto it = args.begin(); it != args.end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
*/
	//}

	return 0;
}

void choose(StringValue query, std::vector<std::string> args) {

	std::string temp = "";

	switch (query) {
		case zero:
			unknown_func();
			break;
		case exitt:
			exit(0);
			break;
		case getPrice:
			get_price(temp, args);
			break;
	}
}

void initialize() {
	mapFuncVals["exit"] = exitt;
	mapFuncVals["getPrice"] = getPrice;
}