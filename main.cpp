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

enum MainValue { main_base, exitt, print, get, help };
enum SubValue { sub_base, price, pe, diff, eps, cap, call, put };

static std::map<std::string, MainValue> mapMainVals;
static std::map<std::string, SubValue> mapSubVals;

void main_choose(std::vector<std::string> args);

void main_choose(std::pair<std::vector<std::string>, std::list<std::string>> query);
void print_choose(std::vector<std::string> args, std::list<std::string> flags);
std::string get_choose(std::vector<std::string> args, std::list<std::string> flags);
void initialize_main_value();
void initialize_sub_value();

int main() {

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
	write_to_csv("aapl");

	return 0;
}

void main_choose(std::pair<std::vector<std::string>, std::list<std::string>> query) {

	switch (mapMainVals[query.first[0]]) {
		case main_base:
			unknown_func();
			return;

		case exitt:
			exit(0);
			return;

		case print:
			query.first.erase(query.first.begin());
			print_choose(query.first, query.second);
			return;

		case get:
			query.first.erase(query.first.begin());
			get_choose(query.first, query.second);
			return;

		case help:
			print_help();
			return;
	}
}

void print_choose(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() < 1) {
		unknown_func();
	}

	switch(mapSubVals[args[0]]) {
		case sub_base:
			unknown_func();
			return;

		case price:
			args.erase(args.begin());
			print_price(args, flags);
			return;

		case pe:
			args.erase(args.begin());
			print_pe(args, flags);
			return;

		case diff:
			args.erase(args.begin());
			print_diff(args, flags);
			return;

		case eps:
			args.erase(args.begin());
			print_eps(args, flags);
			return;

		case cap:
			args.erase(args.begin());
			print_cap(args, flags);
			return;

		case call:
			print_option(args, flags);
			return;

		case put:
			print_option(args, flags);
	}
}

std::string get_choose(std::vector<std::string> args, std::list<std::string> flags) {
	
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
	mapMainVals["help"] = help;
}

void initialize_sub_value() {
	mapSubVals["price"] = price;
	mapSubVals["pe"] = pe;
	mapSubVals["diff"] = diff;
	mapSubVals["eps"] = eps;
	mapSubVals["cap"] = cap;
	mapSubVals["call"] = call;
	mapSubVals["put"] = put;
}