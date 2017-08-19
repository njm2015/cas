#ifndef GET_FUNC_LIST_H
#define GET_FUNC_LIST_H

double get_price(std::string symbol, tm* date);

double get_pe(std::string symbol);

double get_diff_price(std::string symbol, tm* date_1, tm* date_2);

#endif