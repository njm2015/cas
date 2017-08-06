#include <iostream>
#include <string>

std::string parse(std::string query) {

	size_t first_par = query.find("(");

	std::string function = query.substr(0, first_par);

	while(function.find(' ') != std::string::npos) {
		size_t space = function.find(' ');
		function = function.substr(0, space) + function.substr(space+1);		
	}

	return function;


}