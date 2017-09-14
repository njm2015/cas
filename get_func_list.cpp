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

	
	std::string URL, html, xpath_date, xpath_price;
	tm* curr;
	double day_0, day_1, seconds_0, seconds_1;
	bool exit;

	time_t rawtime;
	time(&rawtime);
	curr = localtime(&rawtime);

	if(date == NULL) {
		date = curr;
	}

	int week_day = day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);

	if(week_day == 6) {
		date->tm_mday += 2;
	} else if(week_day == 0) {
		date->tm_mday++;
	}

	if(!date_compare(date, curr, true)) {
		error("in parsing date. Please enter dates in chronological order.");
		return ret;
	}

	tm* temp_date = new tm();				// Create this only for URL purposes
	temp_date->tm_mday = date->tm_mday;
	temp_date->tm_mon = date->tm_mon;
	temp_date->tm_year = date->tm_year;

	day_future += add_to_date_no_weekends(temp_date, day_future);

	day_0 = days_since_epoch(date->tm_mday, date->tm_mon, date->tm_year) - 1 + 0.20833;
	day_1 = (date_compare(date, curr, true) && date_compare(curr, date, true)) ? day_0 + day_future + 1 : day_0 + day_future;
	seconds_0 = day_0 * 86400;
	seconds_1 = day_1 * 86400;

	ret.day = temp_date->tm_mday;
	ret.month = temp_date->tm_mon;
	ret.year = temp_date->tm_year;
	delete temp_date;

	if(day_0 == -1) {
		return ret;
	}

	URL = "https://finance.yahoo.com/quote/"
			+ symbol + "/history?period1="
			+ std::to_string((int)seconds_0) + "&period2="
			+ std::to_string((int)seconds_1) + "&interval=1d&filter=history&frequency=1d";

	std::cout << URL << std::endl;	// Purely for debugging purposes

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);

	xmlNode* temp_node = get_root(doc);

	xmlpp::Element* root = new xmlpp::Element(temp_node);
	
	for(int i = 1; i <= day_future + 1; i++) {

		exit = false;

		xpath_date = "//table[@data-test=\"historical-prices\"]/tbody/tr["
						+ std::to_string(i) + "]/td[1]/span/text()";

		auto elements_date = root->find(xpath_date);

		if(elements_date.size() == 1) {
			ret.date_arr.push_back(dynamic_cast<xmlpp::ContentNode*>(elements_date[0])->get_content());
			delete elements_date[0];
		} else {
			break;
		}


		std::vector<double> temp;

		for(int j = 2; j <= 6; j++) {

			xpath_price = "//table[@data-test=\"historical-prices\"]/tbody/tr["
							+ std::to_string(i) + "]/td["
							+ std::to_string(j) + "]/span/text()";

			auto elements_price = root->find(xpath_price);
			if(elements_price.size() == 1) {
				if(dynamic_cast<xmlpp::ContentNode*>(elements_price[0])->get_content() == "Stock Split" ||
					dynamic_cast<xmlpp::ContentNode*>(elements_price[0])->get_content() == "Dividend") {
					delete elements_price[0];
					ret.date_arr.pop_back();
					exit = true;
					break;
				} else {
					temp.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements_price[0])->get_content()));
					delete elements_price[0];
				}
			} else {
				break;
			}

		}
		if(!exit) {
			ret.price_arr.push_back(temp);
		}
		
	}

	
	xmlCleanupParser();

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

	price_pair price_1, price_2;
	std::vector<double> ret;

	if(!date_1 || !date_2) {
		std::cout << "error in parsing dates. Please enter valid dates MM/DD/YYYY" << std::endl;
		return ret;
	}

	if(!(date_compare(date_1, date_2, false))) {
		std::cout << "please enter the dates in chronological order" << std::endl;
		return ret;
	}

	price_1 = get_price(symbol, date_1, 0);
	price_2 = get_price(symbol, date_2, 0);

	if(price_1.price_arr.size() != 0 && price_2.price_arr.size() != 0) {

		double diff = price_2.price_arr[0][3] - price_1.price_arr[0][3];
		double perc = (diff / price_1.price_arr[0][3]) * 100;
		ret.push_back(diff);
		ret.push_back(perc);

	} else {
		std::cout << "in the else" << std::endl;
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

	std::vector<tm*> date_list;		// Will store the list of option strike dates

	for(int i = 1; i > 0; i++) {

		xpath = "//*[@id=\"Col1-1-OptionContracts-Proxy\"]/section/"
				+ std::string("div/div[1]/select/option[")
				+ std::to_string(i) + "]/text()";

		//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/div[1]/div[1]/select[1]/option["
		//		+ std::to_string(i) + "]/text()";

		auto elements = root_1->find(xpath);
		if(elements.size() == 1) {
			date_list.push_back(parse_string_date(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
			delete elements[0];
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

	std::cout << URL << std::endl;

	html = get_page(URL);

	xmlDoc* doc_2 = get_tree(html);
	xmlpp::Element* root_2 = new xmlpp::Element(get_root(doc_2));

	if(strike == -1) {
		for(int i = 1; i > 0; i++) {

			std::vector<double> temp_ret_price;

			if(call) {
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[1]" 
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

				//xpath = "//tbody[@data-reactid=\"86\"]/tr["
				//		+ std::to_string(i) + "]/td[3]/a/text()";


				//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
						//+ std::to_string(i) + "]/td[3]/a/text()";
			} else {
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[2]"
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

				//xpath = "//tbody[@data-reactid=\"958\"]/tr["
				//		+ std::to_string(i) + "]/td[3]/a/text()";

				//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
				//		+ std::to_string(i) + "]/td[3]/a/text()";
			}

			auto elements = root_2->find(xpath);
			if(elements.size() == 1) {
				temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
				delete elements[0];
			} else {
				break;
			}

			for(int j = 4; j <= 6; j++) {
				if(call) {
					xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[1]" 
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";

					//xpath = "//tbody[@data-reactid=\"86\"/tr["
					//		+ std::to_string(i) + "]/td["
					//		+ std::to_string(j) + "]/text()";

					//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
					//	+ std::to_string(i) + "]/td["
					//	+ std::to_string(j) + "]/text()";
				} else {
					xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[2]"
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";

					//xpath = "//tbody[@data-reactid=\"958\"/tr["
					//		+ std::to_string(i) + "]/td["
					//		+ std::to_string(j) + "]/text()";

					//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
					//	+ std::to_string(i) + "]/td["
					//	+ std::to_string(j) + "]/text()";
				}

				auto elements = root_2->find(xpath);
				if(elements.size() == 1) {
					temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
					delete elements[0];
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
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[1]" 
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

				//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
				//		+ std::to_string(i) + "]/td[3]/a/text()";
			} else {
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[2]"
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

				//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
				//		+ std::to_string(i) + "]/td[3]/a/text()";
			}

			auto elements = root_2->find(xpath);

			if(elements.size() == 1) {
				last_distance = distance_from_strike;
				distance_from_strike = strike - std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
				delete elements[0];
			} else {
				error("in getting call options");
				return make_pair(ret_price, ret_date);
			}
		}

		i = (i == 1) ? 1 : i-1;

		if(call) {
			xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[1]" 
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

			//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[1]/div[2]/table/tbody/tr["
			//			+ std::to_string(i) + "]/td[3]/a/text()";
		} else {
			xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[2]"
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td[3]/a/text()";

			//xpath = "//section[@id=\"quote-leaf-comp\"]/section[1]/section[2]/div[2]/table/tbody/tr["
			//			+ std::to_string(i) + "]/td[3]/a/text()";
		}

		std::cout << xpath << std::endl;

		std::cout << "[i]: " << i << std::endl;

		std::string temp_double;

		auto element = root_2->find(xpath);
		if(element.size() == 1) {
			std::cout << element.size() << std::endl;
			std::cout << dynamic_cast<xmlpp::ContentNode*>(element[0])->get_content() << std::endl;
			delete element[0];
		}

		//temp_ret_price.push_back(temp_double);


		for(int j = 4; j <= 6; j++) {

			if(call) {
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[1]" 
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";
			} else {
				xpath = "//div[@id=\"Col1-1-OptionContracts-Proxy\"]/section[1]/section[2]"
						+ std::string("/div[2]/table[1]/tbody[1]/tr[")
						+ std::to_string(i) + "]/td["
						+ std::to_string(j) + "]/text()";
			}

			auto elements = root_2->find(xpath);
			if(elements.size() == 1) {
				temp_ret_price.push_back(std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content()));
				delete elements[0];
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