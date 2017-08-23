#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <list>
#include <utility>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "msc_func_list.h"
#include "get_func_list.h"
#include "parser.h"

std::pair<std::vector<std::string>, std::list<std::string>> parse_args(std::string query) {

	std::vector<std::string> args;
	std::list<std::string> flags;

	std::string curr_string = query;

	if(curr_string.find(' ') == -1) {
		if(curr_string[0] == '-') {
			flags.push_back(curr_string);
		} else {
			args.push_back(curr_string);
		}

	} else {

		while(curr_string.find(' ') != -1) {
			size_t next_space = curr_string.find(' ');

			if(curr_string[0] == '-') {
				flags.push_back(curr_string.substr(0, next_space));
			} else {
				args.push_back(curr_string.substr(0, next_space));
			}
			curr_string = curr_string.substr(next_space + 1);
		}

		if(curr_string[0] == '-') {
			flags.push_back(curr_string);
		} else {
			args.push_back(curr_string);
		}

	}
	
	return std::make_pair(args, flags);
}

bool validate_date(tm* date) {

	tm* curr;

	time_t rawtime;
	time(&rawtime);
	curr = localtime(&rawtime);

	std::list<int> thirty = {3, 5, 8, 10};
	std::list<int> thirty_one = {0, 2, 4, 6, 7, 9, 11};

	if((std::find(thirty.begin(), thirty.end(), date->tm_mon) != thirty.end() && date->tm_mday > 30) ||
	   (std::find(thirty_one.begin(), thirty_one.end(), date->tm_mon) != thirty_one.end() && date->tm_mday > 31) ||
	   (date->tm_mon == 1 && date->tm_mday > 28) ||
	   (date->tm_mon < 0 || date->tm_mday < 1 || date->tm_year < 0) || 
	   (date->tm_mon > 11) ||
	   (date->tm_mon >= curr->tm_mon && date->tm_mday > curr->tm_mday && date->tm_year >= curr->tm_year) ||
	   (date->tm_mon <= 1 && date->tm_mday < 19 && date->tm_year <= 102)) {
		
		error("in parsing date. Enter valid date MM/DD/YYYY");
		return 0;
	}

	return 1;
}

bool date_compare(tm* date_1, tm* date_2) {

	if(date_1->tm_year <= date_2->tm_year) {
		if(date_1->tm_mon <= date_2->tm_mon) {
			if(date_1->tm_mday < date_2->tm_mday) {
				return true;
			}
		}
	}

	return false;
}

tm* parse_date(std::string date) {

	tm* ret;

	size_t slash = date.find('/');
	std::vector<int> times;

	while(slash != std::string::npos) {
		times.push_back(stoi(date.substr(0,slash)));
		date = date.substr(slash+1);
		slash = date.find('/');
	}

	ret = new tm();

	if(times.size() == 2) {
		ret->tm_mon = times[0] - 1;
		ret->tm_mday = times[1];
		ret->tm_year = stoi(date) - 1900;
	} else {
		error("in parsing date. Enter valid date MM/DD/YYYY");
		return NULL;
	}

	if(validate_date(ret)) {
		return ret;
	} else {
		return NULL;
	}
}

double large_number_parse(std::string num_string) {

	char suffix;
	double num_double;

	suffix = num_string[num_string.length()-1];

	num_double = std::stod(num_string);

	if(suffix == 'M') {
		return num_double * 1000000;
	}

	if(suffix == 'B') {
		return num_double * 1000000000;
	}

	return num_double;
}

std::string reverse_large_number_parse(double num_double) {

	double epsilon = 0.1;
	char suffix;
	std::string num_string;
	std::stringstream stream;

	if(num_double < 100000) {
		return std::to_string(num_double);
	}

	num_double /= 1000000;

	if(num_double < 1000) {
		suffix = 'M';

		int precision_val;

		for(precision_val = 0; fmod(num_double, pow(10, precision_val * -1)) > epsilon; precision_val++) {

		} 

		stream << std::fixed << std::setprecision(precision_val) << num_double;
		num_string = stream.str();

		return num_string + suffix;
	}

	num_double /= 1000;

	suffix = 'B';

	int precision_val;

		for(precision_val = 0; fmod(num_double, pow(10, precision_val * -1)) > epsilon; precision_val++) {

		}

	stream << std::fixed << std::setprecision(precision_val) << num_double;
	num_string = stream.str();

	return num_string + suffix;
}