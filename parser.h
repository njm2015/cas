#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> parse_args(std::string query);

bool validate_date(tm* date);

bool date_compare(tm* date_1, tm* date_2);

tm* parse_date(std::string date);

#endif