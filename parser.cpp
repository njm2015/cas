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

	size_t first_par = query.find("(");

	std::vector<std::string> args;

	for(size_t i = first_par + 1; i < query.length()-1; i++) {

	}

	return args;
}

