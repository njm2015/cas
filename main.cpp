#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <curl/curl.h>
#include <limits>
#include <utility>
#include <list>
#include <iomanip>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include <time.h>
#include "parser.h"
#include "get_func_list.h"
#include "print_func_list.h"
#include "msc_func_list.h"
#include "write.h"

enum MainValue { MAIN_BASE, EXITT, PRINT, GET, HELP };				// Main calls
enum SubValue { SUB_BASE, PRICE, PE, DIFF, EPS, CAP, CALL, PUT };	// Sub calls

static std::map<std::string, MainValue> mapMainVals;
static std::map<std::string, SubValue> mapSubVals;

void main_choose(std::pair<std::vector<std::string>, std::list<std::string>> query);
void print_choose(std::vector<std::string> args, std::list<std::string> flags);

void initialize_main_value();
void initialize_sub_value();

int main() {

	// Uncomment to use interpreter

/*
	initialize_main_value();
	initialize_sub_value();

	while(true) {
		
		std::string query;
		std::cout << std::endl << ">> ";
		std::getline(std::cin, query);

		query.erase(std::remove(query.begin(), query.end(), '\n'), query.end());

		std::transform(query.begin(), query.end(), query.begin(), ::tolower);

		main_choose(parse_args(query));
		
	}
*/
	write_companies(2937, "1/1/2000", "nasdaq_list.csv");

	return 0;
}

void main_choose(std::pair<std::vector<std::string>, std::list<std::string>> query) {

	switch (mapMainVals[query.first[0]]) {
		case MAIN_BASE:
			unknown_func();
			return;

		case EXITT:
			exit(0);
			return;

		case PRINT:
			query.first.erase(query.first.begin());
			print_choose(query.first, query.second);
			return;

		case HELP:
			print_help();
			return;
	}
}

void print_choose(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() < 1) {
		unknown_func();
	}

	switch(mapSubVals[args[0]]) {
		case SUB_BASE:
			unknown_func();
			return;

		case PRICE:
			args.erase(args.begin());
			print_price(args, flags);
			return;

		case PE:
			args.erase(args.begin());
			print_pe(args, flags);
			return;

		case DIFF:
			args.erase(args.begin());
			print_diff(args, flags);
			return;

		case EPS:
			args.erase(args.begin());
			print_eps(args, flags);
			return;

		case CAP:
			args.erase(args.begin());
			print_cap(args, flags);
			return;

		case CALL:
			print_option(args, flags);
			return;

		case PUT:
			print_option(args, flags);
	}
}

void initialize_main_value() {
	mapMainVals["exit"] = EXITT;
	mapMainVals["print"] = PRINT;
	mapMainVals["get"] = GET;
	mapMainVals["help"] = HELP;
}

void initialize_sub_value() {
	mapSubVals["price"] = PRICE;
	mapSubVals["pe"] = PE;
	mapSubVals["diff"] = DIFF;
	mapSubVals["eps"] = EPS;
	mapSubVals["cap"] = CAP;
	mapSubVals["call"] = CALL;
	mapSubVals["put"] = PUT;
}