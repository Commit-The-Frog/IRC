#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class Parser
{
	private:
		string raw;
		string cmd;
		string params;
	public:
		Parser(string&);
		string getRaw() const;
		string getCmd() const;
		string getParams() const;
};


#endif
