#include <iostream>
#include <string>
#include <vector>


std::string no_spaces(std::string function) {

	while(function.find(' ') != std::string::npos) {
		size_t space = function.find(' ');
		function = function.substr(0, space) + function.substr(space+1);		
	}

	return function;
}

std::string parse_func(std::string query) {

	size_t first_par = query.find("(");

	std::string function = query.substr(0, first_par);

	function = no_spaces(function);

	return function;
}

std::vector<std::string> parse_args(std::string query) {

	size_t most_recent_comma = query.find("(");

	std::vector<std::string> args;

	std::string curr_string = query.substr(most_recent_comma + 1);

	if(curr_string.find(',') == -1) {

		args.push_back(curr_string.substr(0, curr_string.length() - 1));

	} else {

		while(curr_string.find(',') != -1) {
			size_t next_comma = curr_string.find(',');
			args.push_back(curr_string.substr(0, next_comma));
			curr_string = curr_string.substr(next_comma + 1);
		}

		args.push_back(curr_string.substr(0, curr_string.length() - 1));

	}
	
	return args;
}

