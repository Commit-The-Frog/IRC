#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "../channel/Channel.hpp"
#include "../parser/Parser.hpp"
#include "../reply/Reply.hpp"
#include <map>
#include "../client/Client.hpp"
using namespace std;

enum {
	PASS,
	NICK,
	USER,
	PRIVMSG,
	UNKNOWN
};

class Command
{
	protected:
		map<int, Client>& client_map;
		map<string, Channel>& channel_map;
	public:
		Command();
		Command(map<int, Client>& client_map, map<string, Channel>& channel_map)
		:client_map(client_map), channel_map(channel_map) {};
		virtual void execute(const Parser&, int) = 0;
		virtual ~Command() {};
};


#endif
