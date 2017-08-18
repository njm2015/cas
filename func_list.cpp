#include "func_list.h"
#include "parser.h"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

void usage(std::string func_name) {

	std::cout << "Incorrect usage for " << func_name << ". 'help' for usages." << std::endl;
}

void unknown_func() {

	std::cout << "enter known function (list of functions 'help')" << std::endl;
}


int day_of_week(int day, int month, int year) {

	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	year -= month < 3;
	return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

void error(std::string message) {

	std::cout << "Error in " << message << std::endl;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string get_page(std::string URL) {

	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();

	const char* new_URL = URL.c_str();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, new_URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		return readBuffer;
	} else {
		error("in getting URL");

		std::string ret = "error";
		return ret;
	}
}

xmlDoc* get_tree(std::string html_page) {

	return htmlReadDoc((xmlChar*)html_page.c_str(), NULL, NULL,
						HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
}

xmlNode* get_root(xmlDoc* tree) {

	return xmlDocGetRootElement(tree);
}

double get_price(std::vector<std::string> args) {
	
	double price;
	std::string URL, html, xpath;

	if(args.size() < 1) {
		usage("get price");
		return -1;
	}

	tm* date;

	std::cout << "[args.size()]: " << args.size() << std::endl;

	if(args.size() == 2) {
		date = parse_date(args[1]);
	} else {
		time_t rawtime;
		time(&rawtime);
		date = localtime(&rawtime);
	}

	if(!date) {
		return -1;
	}

	std::cout << "[date->tm_mday]: " << date->tm_mday << std::endl;
	std::cout << "[date->tm_mon]: " << date->tm_mon << std::endl;
	std::cout << "[date->tm_year]; " << date->tm_year << std::endl;

	int week_day = day_of_week(date->tm_mday, date->tm_mon, date->tm_year);

	if(week_day == 6 || week_day == 0) {
		error("in parsing date. Enter weekday (Mon-Fri)");
		delete date;
		return -1;
	}

	std::string day = std::to_string(date->tm_mday);
	std::string month = std::to_string(date->tm_mon);
	std::string year = std::to_string(date->tm_year);
	std::string symbol = args[0];

	URL = "http://bigcharts.marketwatch.com/historical/default.asp?symb="
						+ symbol + "&closeDate=" 
						+ month + "%2F"
						+ day + "%2F"
						+ year;

	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	xpath = "//table[@class=\"historicalquote fatbottomed\"]//tr[3]/td/text()";

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		price = std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
	} else {
		std::cout << "Invalid symbol" << std::endl;
		return -1;
	}

	delete root;
	delete date;
	xmlFreeDoc(doc);

	return price;
}

double get_pe(std::vector<std::string> args) {

	double pe;
	std::string URL, html, xpath;

	if(args.size() < 1) {
		usage("get pe");
	}

	URL = "https://finance.yahoo.com/quote/"
			+ args[0] + "?p="
			+ args[0];

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

void print_price(std::vector<std::string> args) {

	double price = get_price(args);
	if(price != -1) {
		std::cout << price << std::endl;
	}
}

void print_pe(std::vector<std::string> args) {

	double pe = get_pe(args);
	if(pe != -1) {
		std::cout << pe << std::endl;
	}
}
