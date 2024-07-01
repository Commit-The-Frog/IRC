#include "Parser.hpp"

Parser::Parser(string& raw) :raw(raw) {
	stringstream ss(this->raw);
	string tmp;
	string params;

	ss >> this->cmd;
	transform(this->cmd.begin(), this->cmd.end(), this->cmd.begin(), ::toupper);
	std::getline(ss, params);

	if (!params.empty() && params[0] == ' ') {
		params.erase(0, params.find_first_not_of(' '));
	}
	// parameter 파싱
	string current;
	bool stop = false;
	for (int i = 0; i < params.size(); i++) {
		if ( params[i] == ':' ) {
			// :를 만나면 그 뒤로부터는 split하지 않음.
			stop = true;
			continue;
		}
		if (!stop && params[i] == ' ') {
			if (!current.empty()) {
				param_vec.push_back(current);
				current.clear();
			}
		}else {
				current += params[i];
		}
	}
	if (!current.empty())
		param_vec.push_back(current);
}

string Parser::getRaw() const {
	return raw;
}

string Parser::getCmd() const {
	return cmd;
}

vector<string> Parser::getParams() const {
	return param_vec;
}
