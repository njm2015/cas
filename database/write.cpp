#include <iostream>
#include <fstream>
#include <vector>
#include "write.h"
#include "get_func_list.h"
#include "msc_func_list.h"
#include "parser.h"

void write_to_csv(std::string symbol, std::string date_string) {

	std::string filename = std::string("../database/") + symbol + std::string(".csv"); // path to database

	std::ofstream file;
	file.open(filename, std::ios::out);

	time_t rawtime;
	time(&rawtime);
	tm* curr = localtime(&rawtime);

	tm *date = parse_date(date_string);

	while(date_compare(date, curr, true)) {					// Stops when date is past current date
		std::cout << date_to_string(date) << std::endl;		// Purely for debugging purposes

		price_pair prices = get_price(symbol, date, 90);
		std::vector<std::string> date_list = prices.date_arr;
		std::vector<std::vector<double>> price_list = prices.price_arr;

		for(int i = date_list.size() - 1; i >= 0; i--) {
			size_t temp = date_list[i].find(',');
			date_list[i] = date_list[i].substr(0, temp) + date_list[i].substr(temp+1);
			file << modify_date_format(date_list[i]);
			for(int j = 0; j < price_list[i].size(); j++) {
				file << "," << price_list[i][j];
			}
			file << "\n";
		}

		date->tm_mday = prices.day;
		date->tm_mon = prices.month;
		date->tm_year = prices.year;

		int week_day = day_of_week(date->tm_mday, date->tm_mon+1, date->tm_year+1900);

		if(week_day == 6 || week_day == 0) {	// If date is weekend, add appropriate number
			add_to_date_no_weekends(date, 0);	// of days to itself to be a business day
		} else {
			add_to_date_no_weekends(date, 1);
		}

	}

	delete date;

	file.close();
}

void write_companies(int skip, std::string date_string, std::string source) {

	std::string filename = source;

	std::ifstream file (filename);

	std::string line, symbol;

	std::getline(file, line);

	while(skip > 2) {				// Skip lines until desired
		std::getline(file, line);	// line (skip) is reached
		skip--;
	}

	while(file.good()) {							
		std::getline(file, line);					// Parse stock symbol list
		std::cout << line << std::endl;
		symbol = line.substr(0, line.find(','));
		write_to_csv(symbol, date_string);
	}
	file.close();
}


