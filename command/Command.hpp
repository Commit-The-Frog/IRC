#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "../parser/Parser.hpp"
#include <map>
#include "../action/Client.hpp"
using namespace std;

enum {
	PASS,
	NICK,
	USER,
	PRIVMSG
};

class Command
{
	protected:
		map<int, Client>& client_map;
		// map<int, Channel>& channel_map;
	public:
		Command();
		// Command(map<int, Client>& client_map, map<int, Channel>& channel_map)
		// :client_map(client_map), channel_map(channel_map) {};
		Command(map<int, Client>& client_map)
		:client_map(client_map) {};
		virtual void execute(const Parser&, int) = 0;
		virtual ~Command() {};
};


#endif