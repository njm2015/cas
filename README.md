Dependencies: 

make
libxml++-2.6
curl
g++

Program was compiled error-free with g++.

Note: If you have a version of libxml++ other than 2.6 (default), you will need to change the makefile

Some things you will need to change:

	1. To use the interpreter, uncomment line 36-53 in main.cpp and comment line 52.

	2. Line 54 in main.cpp is used for the database creator tool. The following pertain to it:

		a. Change the first parameter in "write_companies()" in main to the symbol in the list you would like the tool to start from.
				i.e. if I had a .csv file full of symbols, and AAPL was the 100th entry, write_companies(100, .... , ....) would start
				creating the database from row 100 in the .csv file.

		b. Change the second parameter in "write_companies()" in main to the string literal of the start date in format MM/DD/YYYY.

		c. Change the third parameter in "write_companies()" in main to the filename of the symbol .csv file.
		
		d. In write.cpp, change the first part of the filename string (line 11) to the destination folder for the database.

Currently, get_price() outputs the URL solely for debugging purposes while using the database feature.

**WARNING**

XPath expressions change constantly on Yahoo Finance, so tool may not work until updated



**USE**

Common queries to the interpreter include:

print price [symbol]

print price [symbol] mm/dd/yyyy

print eps [symbol]

print cap [symbol]

print diff [symbol] mm/dd/yyyy mm/dd/yyyy

print pe [symbol]

print call [symbol] mm/dd/yyyy <-- strike date

print put [symbol] mm/dd/yyyy

'help' for complete uses