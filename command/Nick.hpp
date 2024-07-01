#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "../parser/Parser.hpp"
# include "../client/Client.hpp"
# include <sstream>

class Nick: public Command
{
	public:
		/*	[execute]
			ERR_NONICKNAMEGIVEN (431)
			ERR_NICKNAMEINUSE (433)
		*/
		Nick(map<int, Client>& client_map, map<string, Channel>& channel_map)
		:Command(client_map, channel_map) {};
		virtual void execute(const Parser& parser, int client_fd) {
			vector<string>	params = parser.getParams();
			string			nick;
			Client&			client = this->client_map[client_fd];

			if (params.size() == 0) {
				client.setSendBuff(Reply("431", client.getNickname(), ":No nickname given"));
			} else {
				nick = params[0];
				try {
					if (nick.find(' ') != string::npos || nick[0] == '&' || nick[0] == '#') {
						client.setSendBuff(Reply("432", client.getNickname(), nick + " :Erroneus nickname"));
						return ;
					}
					client.setNickname(nick);
				} catch (Client::AlreadyInUseNickException& e) {
					client.setSendBuff(Reply("433", client.getNickname(), nick + " :Nickname is already in use"));
				} catch (Client::SameNickException& e) {}
			}
		}
};

#endif
