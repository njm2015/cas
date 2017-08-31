#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>
#include <limits>
#include <list>
#include <vector>
#include <iomanip>
#include <ios>
#include "parser.h"
#include "get_func_list.h"
#include "print_func_list.h"
#include "msc_func_list.h"

static std::streamsize ss = std::cout.precision();

void print_price_header() {

	std::cout << "\n\t|\tdate\t|\topen\t|\thigh\t|\tlow\t|\tclose\t|\tadj close\t|\n" << std::endl;
}

void print_price(std::vector<std::string> args, std::list<std::string> flags) {

	std::cout << __LINE__ << std::endl;	

	price_pair prices;

	if(args.size() < 2) {
		prices = get_price(args[0], NULL, 0);
	} else { 

		tm* date = parse_date(args[1]);

		//delete date;

		if(args.size() < 3) {
			prices = get_price(args[0], NULL, 0);
		} else {
			prices = get_price(args[0], NULL, std::stoi(args[2]));
		}

		std::cout << __FILE__ << __LINE__ << std::endl;

		//delete date;
	
	}

/*

	print_price_header();

	for(int i = 0; i < prices.price_arr.size(); i++) {

		std::cout << "\t" << prices.date_arr[i];

		for(int j = 0; j < prices.price_arr[i].size(); j++) {
			std::cout << "\t\t" << prices.price_arr[i][j];
		}

		std::cout << std::endl;
	}
*/
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

	tm* date_1 = parse_date(args[1]);
	tm* date_2 = parse_date(args[2]);

	diff = get_diff_price(args[0], date_1, date_2);

	if(diff.size() == 2) {
		std::cout << "\n\t|\t" << "$$" << "\t|\t" << "%%\t|" << std::endl;
		std::cout << "\t\t" << diff[0] << "\t\t";
		std::cout.precision(get_precision(diff[1], 0.01));
		std::cout << diff[1] << std::endl;
		std::cout.precision(ss);
	} else {
		std::cout << "error" << std::endl;
	}

	if(date_1) {
		delete date_1;
	}

	if(date_2) {
		delete date_2;
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

void print_option(std::vector<std::string> args, std::list<std::string> flags) {

	if(args.size() < 3) {
		usage("print option call");
	} else if(args.size() == 4) {

		bool call = (args[0] == "call") ? true : false;
		args.erase(args.begin());

		tm* date = parse_date(args[1]);

		std::pair<std::vector<std::vector<double>>, std::vector<std::string>> call_pair = get_option_call(args[0], date, std::stod(args[2]), call);
		std::vector<std::vector<double>> call_double = call_pair.first;
		std::vector<std::string> call_string = call_pair.second;

		if(call_double[0].size() > 0) {
			std::cout << "\n\t|\tstrike\t|\tprice\t|\tbid\t|\task\t|\texpire\t|" << std::endl;
			std::cout << "\t\t" << call_double[0][0] << "\t\t" << call_double[0][1] << "\t\t" << call_double[0][2] << "\t\t" << call_double[0][3];
			std::cout << "\t\t" << call_string[0] << std::endl;
		}

		delete date;

	} else if(args.size() == 3) {

		bool call = (args[0] == "call") ? true : false;
		args.erase(args.begin());

		tm* date = parse_date(args[1]);

		std::pair<std::vector<std::vector<double>>, std::vector<std::string>> call_pair = get_option_call(args[0], date, -1, call);

		if(call_pair.first.size() > 0) {
			std::cout << "\n\t|\tstrike\t|\tprice\t|\tbid\t|\task\t|\texpire\t\t|" << std::endl;

			std::vector<std::vector<double>> call_double = call_pair.first;
			std::vector<std::string> call_string = call_pair.second;

			for(int i = 0; i < call_double.size(); i++) {
				for(int j = 0; j < call_double[i].size(); j++) {
					std::cout << "\t\t" << call_double[i][j];
				}

				std::cout << "\t\t" << call_string[0] << std::endl;
			}
		}	

		if(date) {
			delete date;
		}
	}
}

void print_help() {

	std::cout << "\nexit\t\t\t\t\tterminate the program\n" << std::endl;
	std::cout << "print" << std::endl;
	std::cout << "\tprice [symbol]\t\t\tprints current price of stock [symbol]" << std::endl;
	std::cout << "\tprice [symbol] [date]\t\tprints closing price of stock [symbol] for date [date]" << std::endl;
	std::cout << "\t\tuse flag -all to display full information for print price" << std::endl;
	std::cout << "\tpe [symbol]\t\t\tprints P/E ratio for stock [symbol]" << std::endl;
	std::cout << "\tdiff [symbol] [date1] [date2]\tprints difference (dollar and percent) of stock\n" <<
					"\t\t\t\t\t[symbol] from date1 to date2" << std::endl;
	std::cout << "\teps [symbol]\t\t\tprints eps for stock [symbol]" << std::endl;
	std::cout << "\tcap [symbol]\t\t\tprints market cap for stock [symbol]" << std::endl;
	std::cout << "\tcall [symbol] [date] [strike]\tprints call option price for stock [symbol]\n" <<
					"\t\t\t\t\texpiring date [date] with strike price [strike]" << std::endl;

}