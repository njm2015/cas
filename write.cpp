#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "write.h"
#include "get_func_list.h"
#include "msc_func_list.h"
#include "parser.h"

typedef struct _write_data {
	std::string symbol;
	std::string date_string;
} write_data;

void *write_to_csv(void* ptr) {

	write_data *w = (write_data*)ptr;

	std::cout << "symbol: " << w->symbol << std::endl;
	std::cout << "date: " << w->date_string << std::endl;

	std::string filename = std::string("../database/") + w->symbol + std::string(".csv"); // path to database

	std::ofstream file;
	file.open(filename, std::ios::out);

	time_t rawtime;
	time(&rawtime);
	tm* curr = localtime(&rawtime);

	std::cout << w->date_string << std::endl;

	tm *date = parse_date(w->date_string);

	while(date_compare(date, curr, true)) {					// Stops when date is past current date
		std::cout << date_to_string(date) << std::endl;		// Purely for debugging purposes

		price_pair prices = get_price(w->symbol, date, 90);
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
		sleep(1);
	}

	delete date;

	file.close();

	return NULL;
}

void write_companies(int skip, std::string date_string, std::string source) {

	std::string filename = source;

	std::ifstream file_2(filename);
	std::ifstream file(filename);

	std::string line, symbol;

	std::getline(file, line);
	std::getline(file_2, line);

	size_t line_no = 1;
	while(file_2.good()) {
		std::getline(file_2, line);
		++line_no;
	}

	line_no -= skip;

	std::cout << line_no << std::endl;

	while(skip > 2 && file.good()) {				// Skip lines until desired
		std::getline(file, line);	// line (skip) is reached
		skip--;
	}

	write_data w[line_no];
	pthread_t tid[line_no];
	line_no = 0;

	while(file.good()) {
		std::getline(file, line);					// Parse stock symbol list
		std::cout << line << std::endl;
		symbol = line.substr(0, line.find(','));

		w[line_no].symbol = symbol;
		w[line_no].date_string = date_string;

		pthread_create(&tid[line_no], NULL, write_to_csv, &w[line_no]);

		++line_no;
	}

	--line_no;

	void *result;
	for(size_t i = 0; i < line_no; i++) {
		pthread_join(tid[line_no], &result);
	}

	file_2.close();
	file.close();
}


