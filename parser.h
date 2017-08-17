#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> parse_args(std::string query);

bool validate_date(tm* date);

tm* parse_date(std::string date);

#endif