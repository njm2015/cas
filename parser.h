#ifndef PARSER_H
#define PARSER_H

#include <utility>

std::pair<std::vector<std::string>, std::list<std::string>> parse_args(std::string query);

bool validate_date(tm* date);

bool date_compare(tm* date_1, tm* date_2);

tm* parse_date(std::string date);

double large_number_parse(std::string num);

std::string reverse_large_number_parse(double num_double);

#endif