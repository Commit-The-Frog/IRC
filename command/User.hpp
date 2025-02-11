#ifndef USER_HPP
# define USER_HPP

#include "Command.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class User : public Command {
	public:
		User(std::map<int, Client> &client_map, map<string,  Channel>& channel_map): Command(client_map, channel_map) {};
		~User(){};
		void execute(const Parser &parser, int fd);
		std::vector<string> parseParams(const string &params);
		class ClientNotFoundException : public exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif
