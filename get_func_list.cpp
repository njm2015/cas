#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <curl/curl.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include "parser.h"
#include "get_func_list.h"
#include "msc_func_list.h"

price_pair get_price(std::string symbol, tm* date, int day_future) {

	price_pair ret;
	std::string URL, html, xpath;
	tm* curr;
	double day_0, day_1, seconds_0, seconds_1;

	time_t rawtime;
	time(&rawtime);
	curr = localtime(&rawtime);

	if(date == NULL) {
		date = curr;
	}

	int week_day = day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);

	if(week_day == 6 || week_day == 0) {
		error("in parsing date. Enter weekday (Mon-Fri)");
		return ret;
	}

	if(!date_compare(date, curr, true)) {
		error("in parsing date. Please enter dates in chronological order.");
		return ret;
	}

	day_0 = days_since_epoch(date->tm_mday, date->tm_mon, date->tm_year) - 1 + 0.2083;
	day_1 = day_0 + day_future;
	seconds_0 = day_0 * 86400;
	seconds_1 = day_1 * 86400;

	if(day_0 == -1) {
		return ret;
	}

	for(int i = 0; i < day_future; i++) {
		ret.date_arr.push_back(date_to_string(add_to_date(date, i)));
	}

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/history?period1="
			+ std::to_string((int)seconds_0) + "&period2="
			+ std::to_string((int)seconds_1) + "&interval=1d&filter=history&frequency=1d";

	std::cout << URL << std::endl;

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	for(int i = 1; i <= day_future + 1; i++) {

		std::vector<double> temp;

		for(int j = 2; j <= 6; j++) {

			xpath = "//section[@id=\"quote-leaf-comp\"]//section/div[2]/table/tbody/tr["
					+ std::to_string(i) + "]/td["
					+ std::to_string(j) + "]/span/text()";

			auto elements = root->find(xpath);
			if(elements.size() == 1) {
				temp.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
			} else {
				break;
			}

		}

		ret.price_arr.push_back(temp);
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
/*
std::vector<double> get_diff_price(std::string symbol, tm* date_1, tm* date_2) {

	std::list<std::string> no_val;

	std::vector<double> prices_1, prices_2, ret;

	if(!date_1 || !date_2) {
		std::cout << "error in parsing dates. Please enter valid dates MM/DD/YYYY" << std::endl;
		return ret;
	}

	if(!(date_compare(date_1, date_2, false))) {
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
*/
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

std::pair<std::vector<std::vector<double>>, std::vector<std::string>> get_option_call(std::string symbol, tm* date, double strike, bool call) {

	tm* strike_date;
	tm* curr;
	long URL_seconds;
	double distance_from_strike, last_distance;
	std::vector<std::vector<double>> ret_price;
	std::vector<double> temp_call;
	std::vector<std::string> ret_date;
	std::string URL, html, xpath;

	time_t rawtime;
	time(&rawtime);
	curr = localtime(&rawtime);

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/options?p="
			+ symbol;

	html = get_page(URL);

	xmlDoc* doc_1 = get_tree(html);
	xmlpp::Element* root_1 = new xmlpp::Element(get_root(doc_1));

	std::vector<tm*> date_list;

	for(int i = 1; i > 0; i++) {

		xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/div[1]/div[1]/select[1]/option["
				+ std::to_string(i) + "]/text()";

		auto elements = root_1->find(xpath);
		if(elements.size() == 1) {
			date_list.push_back(parse_string_date(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
		} else {
			break;
		}
	}

	strike_date = closest_date(date, date_list);

	if(date_compare(date, curr, true)) {
		error("parsing date. please enter date after today");
		return std::make_pair(ret_price, ret_date);
	}

	URL_seconds = get_opt_seconds(strike_date);

	ret_date.push_back(date_to_string(strike_date));

	for(int i = 0; i < date_list.size(); i++) {
		delete date_list[i];
	}

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/options?p=" 
			+ symbol + "&date="
			+ std::to_string(URL_seconds);

	html = get_page(URL);

	xmlDoc* doc_2 = get_tree(html);
	xmlpp::Element* root_2 = new xmlpp::Element(get_root(doc_2));

	if(strike == -1) {
		for(int i = 1; i > 0; i++) {

			std::vector<double> temp_ret_price;

			if(call) {
				xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
			} else {
				xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
			}

			auto elements = root_2->find(xpath);
			if(elements.size() == 1) {
				temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
			} else {
				break;
			}

			for(int j = 4; j <= 6; j++) {
				if(call) {
					xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";
				} else {
					xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";
				}

				auto elements = root_2->find(xpath);
				if(elements.size() == 1) {
					temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
				}

			}

			ret_price.push_back(temp_ret_price);
		}

	} else {

		std::vector<double> temp_ret_price;

		distance_from_strike = strike;
		last_distance = strike;

		int i;

		for(i = 1; distance_from_strike - strike < last_distance && distance_from_strike > 0; i++) {

			if(call) {
				xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
			} else {
				xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
			}

			auto elements = root_2->find(xpath);

			if(elements.size() == 1) {
				last_distance = distance_from_strike;
				distance_from_strike = strike - std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
			} else {
				error("in getting call options");
				return make_pair(ret_price, ret_date);
			}
		}

		i = (i == 1) ? 1 : i-1;

		if(call) {
			xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
		} else {
			xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td[3]/a/text()";
		}

		auto elements = root_2->find(xpath);
		if(elements.size() == 1) {
			temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
		}

		for(int j = 4; j <= 6; j++) {

			if(call) {
				xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";
			} else {
				xpath = xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
					+ std::to_string(i) + "]/td["
					+ std::to_string(j) + "]/text()";
			}

			auto elements = root_2->find(xpath);
			if(elements.size() == 1) {
				temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
			}

		}

		ret_price.push_back(temp_ret_price);

	}

	delete root_1;
	delete root_2;
	xmlFreeDoc(doc_1);
	xmlFreeDoc(doc_2);

	return std::make_pair(ret_price, ret_date);
}