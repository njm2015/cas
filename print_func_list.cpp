#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>
#include <limits>
#include "parser.h"
#include "get_func_list.h"
#include "print_func_list.h"
#include "msc_func_list.h"

void print_price(std::vector<std::string> args) {

	double price;

	if(args.size() < 1) {
		usage("print price");
	}

	if(args.size() == 1) {
		price = get_price(args[0], NULL);
	} else {
		tm* date = parse_date(args[1]);

		if(!date) {
			error("in getting date. Please enter valid date MM/DD/YYYY");
			return;
		}

		price = get_price(args[0], date);
		delete date;
	}

	if(price != -1) {
		std::cout << price << std::endl;
	}
}

void print_pe(std::vector<std::string> args) {

	if(args.size() > 0) {
		double pe = get_pe(args[0]);
		if(pe != -1) {
			std::cout << pe << std::endl;
		}
	} else {
		usage("print pe");
	}
}

void print_diff(std::vector<std::string> args) {

	if(args.size() < 3) {
		usage("print_diff");
		return;
	}

	double diff = get_diff_price(args[0], parse_date(args[1]), parse_date(args[2]));

	if(diff != std::numeric_limits<double>::max()) {
		std::cout << diff << std::endl;
	} 
}