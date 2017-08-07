#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>

std::string parse_func(std::string query);

std::vector<std::string> parse_args(std::string query);

std::string no_spaces(std::string function);

#endif