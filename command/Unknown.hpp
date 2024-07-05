#ifndef UNKNOWN_HPP
# define UNKNOWN_HPP

# include "Command.hpp"

class Unknown: public Command {
	public:
		Unknown(map<int, Client>& client_map, map<string, Channel>& channel_map)
		:Command(client_map, channel_map) {};
		virtual void execute(const Parser& parser, int client_fd) {
			Client&	client = this->client_map[client_fd];

			if (!client.getIsRegistered() || parser.getRaw().length() == 0) {
				return ;
			} else {
				client.setSendBuff(Reply::getCodeMsg("421", client.getNickname(), parser.getCmd() + " :Unknown command"));
			}
		}
};

#endif
