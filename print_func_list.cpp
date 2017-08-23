#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>
#include <limits>
#include <list>
#include <vector>
#include <iomanip>
#include "parser.h"
#include "get_func_list.h"
#include "print_func_list.h"
#include "msc_func_list.h"

void print_price_header() {

	std::cout << "\n\t|\tOpen\t|\tHigh\t|\tLow\t|\tClose\t|\tAdj Close\t|" << std::endl;
}

void print_price(std::vector<std::string> args, std::list<std::string> flags) {

	std::vector<double> prices;

	if(args.size() < 1) {
		usage("print price");
		return;
	}

	if(args.size() == 1) {
		prices = get_price(args[0], NULL, flags);
	} else {
		tm* date = parse_date(args[1]);

		if(!date) {
			error("in getting date. Please enter valid date MM/DD/YYYY");
			return;
		}

		prices = get_price(args[0], date, flags);
		delete date;
	}

	
	if(prices.size() != 0) {
		if(std::find(flags.begin(), flags.end(), "-all") != flags.end()) {
			print_price_header();

			std::cout << "\t\t";

			for(auto it = prices.begin(); it != prices.end(); it++) {
				std::cout << *it << "\t\t";
			}

			std::cout << std::endl;
		} else {
			std::cout << "\n\t|\tClose\t|" << std::endl;
			std::cout << "\t\t" << prices[3] << std::endl;
		}
	}
	
}

void print_pe(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() > 0) {
		std::vector<double> pe = get_pe(args[0]);
		if(pe.size() > 0) {
			std::cout << "\n\t|\tpe\t|" << std::endl;
			std::cout << "\t\t" << pe[0] << std::endl;
		}
	} else {
		usage("print pe");
	}
}

void print_diff(std::vector<std::string> args, std::list<std::string> flags) {

	std::vector<double> diff;

	if(args.size() < 3) {
		usage("print_diff");
		return;
	}

	diff = get_diff_price(args[0], parse_date(args[1]), parse_date(args[2]));

	if(diff.size() == 2) {
		std::cout << std::setprecision(2);
		std::cout << "\n\t|\t" << "$$" << "\t|\t" << "%%\t|" << std::endl;
		std::cout << "\t\t" << diff[0] << "\t\t" << diff[1] << std::endl;
	} else {
		std::cout << "error" << std::endl;
	}
	
}

void print_eps(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() > 0) {
		std::vector<double> eps = get_eps(args[0]);
		if(eps.size() > 0) {
			std::cout << "\n\t|\teps\t|" << std::endl;
			std::cout << "\t\t" << eps[0] << std::endl;
		}
	} else {
		usage("print eps");
	}
}

void print_cap(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() > 0) {
		std::vector<double> cap = get_cap(args[0]);
		if(cap.size() > 0) {
			std::cout << "\n\t|\tmkt cap\t|" << std::endl;
			std::cout << "\t\t" << reverse_large_number_parse(cap[0]) << std::endl;
		}
	} else {
		usage("print eps");
	}
}