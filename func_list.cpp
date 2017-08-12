#include "func_list.h"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

void error(std::string message) {
	std::cout << "Error in " << message << std::endl;
	exit(0);
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

void unknown_func() {
	std::cout << "enter known function (list of functions 'help')" << std::endl;
}

int get_price(std::string symbol) {
	
	std::string URL = "http://bigcharts.marketwatch.com/historical/default.asp?symb="
						+ symbol + "&closeDate=07%2F07%2F2009";

	std::string html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	std::string xpath = "//table[@class=\"historicalquote fatbottomed\"]//tr[3]/td/text()";

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		std::cout << dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content() << std::endl;
	} else {
		std::cout << "Invalid symbol" << std::endl;
	}

	delete root;
	xmlFreeDoc(doc);

	return 0;
}