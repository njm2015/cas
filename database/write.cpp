#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <curl/curl.h>

typedef struct _thread_data {
	std::string symbol;
	Date *d;
} thread_data;

std::vector<std::string> create_threads(std::vector<std::string> symbols, Date *d) {
	int num_threads = symbols.size();

	thread_data td[num_threads];
	pthread_t tid[num_threads];
	std::vector<std::string> res;

	for(size_t i = 0; i < num_threads; ++i) {
		td[i].symbol = symbols[i];
		td[i].d = d;
		pthread_create(&tid[i], NULL, get_data, &td[i]);
	}

	std::string temp_res;

	for(size_t i = 0; i < num_threads, ++i) {
		pthread_join(tid[i], (void*)temp_res);
		res.push_back(*temp_res);
	}

	return res;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void *get_data(void *ptr) {

	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();

	const char* URL = "https://www.quandl.com/api/v3/datasets/WIKI/" + symbol + ".csv?"
					"&start_date=2017-11-01&end_date=2017-11-07&api_key=dmHSV7g6x4TNGkyypv8q";

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

	}

	std::string *ret_string = new std::string(readBuffer);

	return (void*)ret_string;
}


