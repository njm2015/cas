#ifndef PARSER_H
#define PARSER_H



/**
* A function to push a space separated query into vectors.
* This particular function pushes into an argument vector and 
* a flag vector.
* @param query They initial query
* @return A pair containing a string vector and a string list.
* The string vector is for the arguments, the list is for the flags
**/ 

std::pair<std::vector<std::string>, std::list<std::string>> parse_args(std::string query);

/**
* A function that makes sure that the reference date is within
* basic parameters of a date
* @param date The reference date
* @return true if the reference date complies with given properties
* false otherwise
**/ 

bool validate_date(tm* date);

/**
* A function to determine if two dates are in chronological order.
* @param date_1 The first reference date
* @param date_2 The second reference date
* @param equal Tells the function if the two dates are allowed
* to be the same date for the function to return true;
* @return true if date_1 is chronologically before date_2
* false otherwise
**/

bool date_compare(tm* date_1, tm* date_2, bool equal);

/**
* This function receives a date in string format and creates a tm
* object from that.
* @param date The string to convert to tm object
* @return The tm object created from the reference string
**/ 

tm* parse_date(std::string date);

/**
* This function receives large numbers as strings with either
* the suffix 'M' or 'B' (million or billion) and converts the
* number to its appropriate decimal equivalent.
* @param num The string of the number to be converted
* @return The decimal value of the reference string
**/

double large_number_parse(std::string num);

/**
* This function takes a large number and makes it more readable
* by appending either 'M' or 'B' to the end of it.
* @param num_double The decimal version of the number to be
* converted.
* @return The string version of the decimal number with the
* appropriate suffix
**/

std::string reverse_large_number_parse(double num_double);

/**
* A variable function that returns essentially how many 
* nonzero digits are in a given number.
* @param num The number to evaluate
* @param epsilon The degree of precision you would like
* to evaluate with
* @return The number of digits of precision
**/

int get_precision(double num, double epsilon);

tm* parse_string_date(std::string date_string);

#endif