#ifndef WRITE_H
#define WRITE_H

/**
* Takes in a stock symbol and a start date and retreives the
* prices for that equity from the start date to present date.
* Then puts that data in a .csv file named SYMBOL.csv
* @param symbol The reference stock symbol
* @param date_string The start date in string form.
**/
void write_to_csv(std::string symbol, std::string date_string);

/**
* Given a .csv file of stock symbols separated by newline, this
* function creates the database of historical prices from the 
* start date 'date_string' to the present. See 'write_to_csv' to
* see how the data is stored.
* @param skip The row of the .csv file you would like to start from.
* @param date_string The start date in string form.
* @param source The path to the .csv file containing the symbols
**/
void write_companies(int skip, std::string date_string, std::string source);

#endif