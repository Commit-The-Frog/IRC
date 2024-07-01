#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "../parser/Parser.hpp"
# include "../action/Client.hpp"
# include <sstream>

class Nick: public Command
{
	public:
		/*	[execute]
			ERR_NONICKNAMEGIVEN (431)
			ERR_NICKNAMEINUSE (433)
		*/
		Nick(map<int, Client>& client_map)
		:Command(client_map) {};
		virtual void execute(const Parser& parser, int client_fd) {
			stringstream	ss(parser.getParams());
			string			nick;
			Client&			client = this->client_map[client_fd];

			ss >> nick;
			if (nick.length() == 0) {
				client.setSendBuff("<client> :No nickname given\r\n");
			} else {
				try {
					client.setNickname(nick);
					client.setSendBuff("You're now known as " + nick + "\r\n");
				} catch (Client::AlreadyInUseNickException& e) {
					client.setSendBuff("<client> <nick> :Nickname is already in use\r\n");
				} catch (Client::SameNickException& e) {}
			}
		}
};

#endif
