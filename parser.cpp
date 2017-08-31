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
	bool ret;

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
	   (date->tm_mon <= 1 && date->tm_mday < 19 && date->tm_year <= 102)) {
		
		error("in parsing date. Enter valid date MM/DD/YYYY");
		ret = 0;
	} else {
		ret = 1;
	}

	delete curr;

	return ret;
}

bool date_compare(tm* date_1, tm* date_2, bool equal) {

	if(date_1->tm_year < date_2->tm_year) {
		return true;
	} else if(date_1->tm_year > date_2->tm_year) {
		return false;
	} else {
		if(date_1->tm_mon < date_2->tm_mon) {
			return true;
		} else if(date_1->tm_mon > date_2->tm_mon) {
			return false;
		} else {
			if(equal) {
				if(date_1->tm_mday <= date_2->tm_mday) {
					return true;
				} else {
					return false;
				}
			} else {
				if(date_1->tm_mday < date_2->tm_mday) {
					return true;
				} else {
					return false;
				}
			}
		}
	}

	return false;
}

tm* parse_date(std::string date) {

	tm* ret = new tm();

	size_t slash = date.find('/');
	std::vector<int> times;

	while(slash != std::string::npos) {
		times.push_back(stoi(date.substr(0,slash)));
		date = date.substr(slash+1);
		slash = date.find('/');
	}

	if(times.size() == 2) {
		ret->tm_mon = times[0] - 1;
		ret->tm_mday = times[1];
		ret->tm_year = stoi(date) - 1900;
	} else {
		error("in parsing date. Enter valid date MM/DD/YYYY");
		//delete ret;
		return NULL;
	}

	if(validate_date(ret)) {
		return ret;
	} else {
		//delete ret;
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

		stream << std::fixed << std::setprecision(get_precision(num_double, epsilon)) << num_double;
		num_string = stream.str();

		return num_string + suffix;
	}

	num_double /= 1000;

	suffix = 'B';

	stream << std::fixed << std::setprecision(get_precision(num_double, epsilon)) << num_double;
	num_string = stream.str();

	return num_string + suffix;
}

int get_precision(double num, double epsilon) {

	int precision_val;

	for(precision_val = 0; fmod(num, pow(10, precision_val * -1)) > epsilon; precision_val++) {

	}

	return precision_val + 1;
}

tm* parse_string_date(std::string date_string) {

	std::vector<std::string> ret;

	size_t first_space = date_string.find(' ');

	ret.push_back(date_string.substr(0, first_space));

	date_string = date_string.substr(first_space + 1);

	size_t comma = date_string.find(',');

	ret.push_back(date_string.substr(0, comma));

	date_string = date_string.substr(comma + 2);

	ret.push_back(date_string);

	return string_to_date(ret);
}