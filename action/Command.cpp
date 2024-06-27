#include "Command.hpp"

Command::Command(string raw) :raw(raw) {
	string cmd_to_find;
	size_t idx;

	// PASS
	cmd_to_find = "PASS";
	idx = raw.find(cmd_to_find);
	if (idx != string::npos)
		this->command = raw.substr(idx, cmd_to_find.size());
	cout << command << endl;
}
