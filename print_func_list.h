#ifndef PRINT_FUNC_LIST_H
#define PRINT_FUNC_LIST_H

/**
* Prints the price for a certain stock symbol
* @param args The vector containing the commands
* @param flags The vector containing printing flags
**/

void print_price(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the P/E ratio for a certain stock symbol
* @param args The vector containing the commands
* @param flags The vector containing printing flags
**/

void print_pe(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the difference in price between two given dates
* for one stock symbol
* @param args The vector containing the commands
* @param flags the vector containing printing flags
**/

//void print_diff(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the EPS for a certain stock symbol
* args The vector containing the commands
* flags The vector containing the printing flags
**/

void print_eps(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the market cap for a certain stock symbol
* args The vector containing the commands
* flags The vector containing the printing flags
**/

void print_cap(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the option call contracts for a certain stock symbol,
* expire date, and strike price.
* @param args The vector containing commands
* @param flags The vector containing the printing flags
**/

void print_option(std::vector<std::string> args, std::list<std::string> flags);

/**
* Prints the help menu
**/

void print_help();

#endif