#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <iostream>
#include <vector>
using namespace std;

class Command
{
	private:
		string raw;
		string command;
		vector<string> params;
	public:
		Command(string raw);
		~Command();
};


#endif