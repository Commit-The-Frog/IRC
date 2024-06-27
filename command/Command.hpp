#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "../action/Client.hpp"
#include <map>
using namespace std;

class Parser;
class Channel;

class Command
{
	protected:
		map<int, Client> client_map;
		map<int, Channel> channel_map;
	public:
		Command(map<int, Client>& client_map, map<int, Channel>& channel_map)
		:client_map(client_map), channel_map(channel_map) {};
		virtual void execute(const Parser&, int);
};


#endif