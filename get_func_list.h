#ifndef GET_FUNC_LIST_H
#define GET_FUNC_LIST_H

#include <list>

std::vector<double> get_price(std::string symbol, tm* date, std::list<std::string> flags);

std::vector<double> get_pe(std::string symbol);

std::vector<double> get_diff_price(std::string symbol, tm* date_1, tm* date_2);

std::vector<double> get_eps(std::string symbol);

std::vector<double> get_cap(std::string symbol);

#endif