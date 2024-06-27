#include "Parser.hpp"

Parser::Parser(string& raw) :raw(raw)
{
	stringstream ss(this->raw);
	string tmp;

	ss >> this->cmd;
	ss >> tmp;
	this->params.push_back(tmp);
}

string Parser::getCmd() const {
	return cmd;
}

vector<string> Parser::getParams() const {
	return params;
}