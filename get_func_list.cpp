#include <iostream>
#include <string>
#include <list>
#include <curl/curl.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include "parser.h"
#include "get_func_list.h"
#include "msc_func_list.h"

std::vector<double> get_price(std::string symbol, tm* date, std::list<std::string> flags) {

	std::vector<double> ret, empty;
	std::string URL, html, xpath;

	if(date == NULL) {
		time_t rawtime;
		time(&rawtime);
		date = localtime(&rawtime);
	}

	int week_day = day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);

	if(week_day == 6 || week_day == 0) {
		error("in parsing date. Enter weekday (Mon-Fri)");
		return empty;
	}

	long seconds_1 = seconds_since_epoch(date->tm_mday, date->tm_mon, date->tm_year);
	long seconds_0 = seconds_1 - 86400;

	if(seconds_1 == -1) {
		return empty;
	}

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/history?period1="
			+ std::to_string(seconds_0) + "&period2="
			+ std::to_string(seconds_1) + "&interval=1d&filter=history&frequency=1d";

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	for(int i = 2; i <= 6; i++) {
		xpath = "//section[@id=\"quote-leaf-comp\"]//section/div[2]/table/tbody/tr[1]/td["
				 + std::to_string(i) + "]/span/text()";

		auto elements = root->find(xpath);
		if(elements.size() == 1) {
			ret.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
		} else {
			std::cout << "Invalid symbol" << std::endl;
			return empty;
		}
	}

	delete root;
	xmlFreeDoc(doc);

	return ret;
}

std::vector<double> get_pe(std::string symbol) {

	std::vector<std::string> pe_string;
	std::vector<double> pe_double;
	std::string URL, xpath;

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "?p="
			+ symbol;

	xpath = "//td[@data-test=\"PE_RATIO-value\"]/span/text()";

	pe_string = get_xml_element_text(URL, xpath);

	if(pe_string.size() > 0) {
		pe_double.push_back(std::stod(pe_string[0]));
	}

	return pe_double;
}

std::vector<double> get_diff_price(std::string symbol, tm* date_1, tm* date_2) {

	std::list<std::string> no_val;

	std::vector<double> prices_1, prices_2, ret;

	if(!date_1 || !date_2) {
		std::cout << "error in parsing dates. Please enter valid dates MM/DD/YYYY" << std::endl;
		return ret;
	}

	if(!(date_compare(date_1, date_2))) {
		std::cout << "please enter the dates in chronological order" << std::endl;
		return ret;
	}

	prices_1 = get_price(symbol, date_1, no_val);
	prices_2 = get_price(symbol, date_2, no_val);

	if(prices_1.size() != 0 && prices_2.size() != 0) {

		double diff = prices_2[3] - prices_1[3];
		double perc = (diff / prices_1[3]) * 100;
		ret.push_back(diff);
		ret.push_back(perc);

	} 

	return ret;
}

std::vector<double> get_eps(std::string symbol) {

	std::vector<std::string> eps_string;
	std::vector<double> eps_double;
	std::string URL, xpath;

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "?p=" 
			+ symbol;

	xpath = "//td[@data-test=\"EPS_RATIO-value\"]/span/text()";

	eps_string = get_xml_element_text(URL, xpath);

	if(eps_string.size() > 0) {
		eps_double.push_back(std::stod(eps_string[0]));
	}

	return eps_double;

}

std::vector<double> get_cap(std::string symbol) {

	std::vector<std::string> cap_string;
	std::vector<double> cap_double;
	std::string URL, xpath;

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "?p=" 
			+ symbol;

	xpath = "//td[@data-test=\"MARKET_CAP-value\"]/span/text()";

	cap_string = get_xml_element_text(URL, xpath);

	if(cap_string.size() > 0) {
		cap_double.push_back(large_number_parse(cap_string[0]));
	}

	return cap_double;

}