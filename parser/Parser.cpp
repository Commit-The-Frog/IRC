#include "Parser.hpp"

Parser::Parser(string& raw) :raw(raw) {
	stringstream ss(this->raw);
	string tmp;

	ss >> this->cmd;
	transform(this->cmd.begin(), this->cmd.end(), this->cmd.begin(), ::toupper);
	std::getline(ss, this->params);

	if (!this->params.empty() && this->params[0] == ' ') {
		this->params.erase(0, this->params.find_first_not_of(' '));
	}
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
