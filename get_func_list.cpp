#include <iostream>
#include <string>
#include <curl/curl.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include "parser.h"
#include "get_func_list.h"
#include "msc_func_list.h"

double get_price(std::string symbol, tm* date) {
	
	double price;
	std::string URL, html, xpath;

	if(date == NULL) {
		time_t rawtime;
		time(&rawtime);
		date = localtime(&rawtime);
	}

	int week_day = day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);

	if(week_day == 6 || week_day == 0) {
		error("in parsing date. Enter weekday (Mon-Fri)");
		return -1;
	}

	long seconds_1 = seconds_since_epoch(date->tm_mday, date->tm_mon, date->tm_year);
	long seconds_0 = seconds_1 - 86400;

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/history?period1="
			+ std::to_string(seconds_0) + "&period2="
			+ std::to_string(seconds_1) + "&interval=1d&filter=history&frequency=1d";

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	xpath = "//section[@id=\"quote-leaf-comp\"]//section/div[2]/table/tbody/tr[1]/td[6]/span/text()";

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		price = std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
	} else {
		std::cout << "Invalid symbol" << std::endl;
		return -1;
	}

	delete root;
	xmlFreeDoc(doc);

	return price;
}

double get_pe(std::string symbol) {

	double pe;
	std::string URL, html, xpath;

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "?p="
			+ symbol;

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	xpath = "//td[@data-test=\"PE_RATIO-value\"]/span/text()";

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		pe = std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
	} else {
		std::cout << "Invalid symbol" << std::endl;
		return -1;
	}

	return pe;
}

double get_diff_price(std::string symbol, tm* date_1, tm* date_2) {

	if(!date_1 || !date_2) {
		std::cout << "error in parsing dates. Please enter valid dates MM/DD/YYYY" << std::endl;
	}

	if(!(date_compare(date_1, date_2))) {
		std::cout << "please enter the dates in chronological order" << std::endl;
		return std::numeric_limits<double>::max();
	}

	double price_1 = get_price(symbol, date_1);
	double price_2 = get_price(symbol, date_2);

	return price_2 - price_1;

}