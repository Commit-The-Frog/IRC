#ifndef PRIVMSG_HPP_
#define PRIVMSG_HPP_

#include "Command.hpp"

class Privmsg : public Command
{
	public:
		Privmsg(map<int, Client> client_map, map<string, Channel> channel_map)
		: Command(client_map, channel_map) {};
		~Privmsg() {};
		/*
			PRIVMSG 명령 실행
		*/
		void execute(const Parser& parser, int client_fd) {
			vector<string> params = parser.getParams();
		}
};

#endif