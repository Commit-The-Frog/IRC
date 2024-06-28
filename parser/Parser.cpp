#include "Parser.hpp"

Parser::Parser(string& raw) :raw(raw)
{
	stringstream ss(this->raw);
	string tmp;

	ss >> this->cmd;
	if (ss.eof())
		this->params = "";
	else
		this->params = ss.str();
}

string Parser::getRaw() const {
	return raw;
}

string Parser::getCmd() const {
	return cmd;
}

string Parser::getParams() const {
	return params;
}
