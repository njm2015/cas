#ifndef GET_FUNC_LIST_H
#define GET_FUNC_LIST_H

struct price_pair {
	std::vector<std::vector<double>> price_arr;
	std::vector<std::string> date_arr;
};

/**
* Gets the price for given stock symbol and date
* @param symbol The stock symbol of the price to retrieve
* @param date The date of the price to retrieve
* @param flags The list of flags passed in to the function
* @return A vector<double> with 5 entries for each date:
* open, high, low, close, adjusted close
**/

price_pair get_price(std::string symbol, tm* date, int day_future);

/**
* Gets the P/E ratio for given stock symbol
* @param symbol The stock symbol of the P/E to retrieve
* @return A vector<double> with the P/E ratio as the first element
**/

std::vector<double> get_pe(std::string symbol);

/**
* Gets the difference in price for one stock symbol
* between two different dates
* @param symbol The stock symbol
* @param date_1 The first date (chronological)
* @param date_2 The second date
* @return A vector<double> with the first element being the 
* dollar difference and the second element being percent difference
**/

//std::vector<double> get_diff_price(std::string symbol, tm* date_1, tm* date_2);

/**
* Gets the EPS for a given stock symbol
* @param symbol The stock symbol
* @return A vector<double> with the first element being
* the EPS for the stock symbol
**/

std::vector<double> get_eps(std::string symbol);

/**
* Gets the market cap for given stock symbol
* @param symbol The stock symbol
* @return A vector<double> with the first element being the 
* market cap
**/

std::vector<double> get_cap(std::string symbol);

/**
* Gets the option call price for given stock symbol and given strike price
* @param symbol The stock symbol
* @param date The expire date on the call
* @param strike The strike price of the call
* @return A vector<double> with 4 fields: strike, price, bid, ask
**/

//std::pair<std::vector<double>, std::vector<std::string>> get_option_call(std::string symbol, tm* date, double strike);

/**
* Gets the option call prices for given stock symbol and ALL strike prices
* @param symbol The stock symbol
* @param date The expire date on the call
* @return A pair with the first element being a 2-dimensional vector<double>
* that contains the entire table; the second element is a vector<string> which
* contains the expire date on the call
**/

std::pair<std::vector<std::vector<double>>, std::vector<std::string>> get_option_call(std::string symbol, tm* date, double strike, bool call);

#endif