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
				// client.setSendBuff(cur_nick + " :No nickname given\r\n");
			} else {
				nick = params[0];
				try {
					client.setNickname(nick);
					// client.setSendBuff("You're now known as " + nick + "\r\n");
				} catch (Client::AlreadyInUseNickException& e) {
					client.setSendBuff(Reply("433", client.getNickname(), nick + " :Nickname is already in use"));
					// client.setSendBuff("<client> " + cur_nick + " :Nickname is already in use\r\n");
				} catch (Client::SameNickException& e) {}
			}
		}
};

#endif
