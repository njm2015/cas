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

enum MainValue { zero, exitt, print, get };
enum SubValue { base, price, pe };

static std::map<std::string, MainValue> mapMainVals;
static std::map<std::string, SubValue> mapSubVals;

void main_choose(std::vector<std::string> args);

void main_choose(std::vector<std::string> args);
void print_choose(std::vector<std::string> args);
std::string get_choose(std::vector<std::string> args);
void initialize_main_value();
void initialize_sub_value();

int main() {

	initialize_main_value();
	initialize_sub_value();

	std::cout << "Welcome to Command CAS terminal. exit() to end program\n\n" << std::endl;

	while(true) {
		std::string query;
		std::cout << std::endl << ">> ";
		std::getline(std::cin, query);

		query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());

		std::transform(query.begin(), query.end(), query.begin(), ::tolower);

		main_choose(parse_args(query));

	}

	return 0;
}

void main_choose(std::vector<std::string> args) {

	switch (mapMainVals[args[0]]) {
		case zero:
			unknown_func();
			return;

		case exitt:
			exit(0);
			return;

		case print:
			args.erase(args.begin());
			print_choose(args);
			return;

		case get:
			args.erase(args.begin());
			get_choose(args);
			return;
	}
}

void print_choose(std::vector<std::string> args) {

	switch(mapSubVals[args[0]]) {
		case zero:
			unknown_func();
			return;

		case price:
			args.erase(args.begin());
			print_price(args);
			return;

		case pe:
			args.erase(args.begin());
			print_pe(args);
			return;
	}
}

std::string get_choose(std::vector<std::string> args) {
	std::string ret;
	return ret;
}

void initialize_main_value() {
	mapMainVals["exit"] = exitt;
	mapMainVals["print"] = print;
	mapMainVals["get"] = get;
}

void initialize_sub_value() {
	mapSubVals["price"] = price;
	mapSubVals["pe"] = pe;
}