#ifndef PRIVMSG_HPP_
#define PRIVMSG_HPP_

#include "Command.hpp"

class Privmsg
{
	private:
		map<int, Client> client_map;
	public:
		Privmsg();
		~Privmsg();
};

#endif