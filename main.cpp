#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <curl/curl.h>
#include <limits>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include <time.h>
#include "parser.h"
#include "get_func_list.h"
#include "print_func_list.h"
#include "msc_func_list.h"

enum MainValue { main_base, exitt, print, get };
enum SubValue { sub_base, price, pe, diff };

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
		case main_base:
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

	if(args.size() < 1) {
		unknown_func();
	}

	switch(mapSubVals[args[0]]) {
		case sub_base:
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

		case diff:
			args.erase(args.begin());
			print_diff(args);
			return;
	}
}

std::string get_choose(std::vector<std::string> args) {
	
	if(args.size() < 1) {
		unknown_func();
	}

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
	mapSubVals["diff"] = diff;
}