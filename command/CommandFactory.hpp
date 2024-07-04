#ifndef COMMAND_FACTORY_HPP_
#define COMMAND_FACTORY_HPP_

#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "Command.hpp"
#include "Pass.hpp"
#include "User.hpp"
#include "Nick.hpp"
#include "Privmsg.hpp"
#include "Unknown.hpp"
#include "Mode.hpp"
#include "Join.hpp"
#include "Invite.hpp"


class CommandFactory
{
	private:
		map<int, Client>& client_map;
		map<string, Channel>& channel_map;
		string server_pwd;
		map<int, Command*> cmd_map;
	public:
		 CommandFactory(map<int, Client>& client_map, map<string, Channel>& channel_map, const string& server_pwd)
		 :client_map(client_map), channel_map(channel_map), server_pwd(server_pwd)
		{
			cmd_map[PASS] = new Pass(client_map, channel_map, server_pwd);
			cmd_map[USER] = new User(client_map, this->channel_map);
			cmd_map[NICK] = new Nick(client_map, channel_map);
			cmd_map[MODE] = new Mode(client_map, channel_map);
			cmd_map[JOIN] = new Join(client_map, this->channel_map);
			cmd_map[PRIVMSG] = new Privmsg(this->client_map, this->channel_map);
			cmd_map[INVITE] = new Invite(client_map, channel_map);
			cmd_map[UNKNOWN] = new Unknown(client_map, channel_map);
		};
		~CommandFactory() {
			map<int, Command*>::iterator it;

			for (it=cmd_map.begin(); it!=cmd_map.end(); it++)
				delete (it->second);
		};

		/*
			cmd_map에서 해당하는 커맨드를 찾아 리턴(포인터 리턴)
		*/
		Command* generateCommand(const Parser& parser) const {
			string cmd = parser.getCmd();
			int cmd_type;

			if (cmd.compare("PASS") == 0)
				cmd_type = PASS;
			else if (cmd.compare("NICK") == 0)
				cmd_type = NICK;
			else if(cmd.compare("USER") == 0)
				cmd_type = USER;
			else if (cmd.compare("JOIN") == 0)
				cmd_type = JOIN;
			else if (cmd.compare("MODE") == 0)
				cmd_type = MODE;
			else if (cmd.compare("PRIVMSG") == 0)
				cmd_type = PRIVMSG;
			else if (cmd.compare("INVITE") == 0)
				cmd_type = INVITE;
			// ...
			else
				cmd_type = UNKNOWN;
			return cmd_map.find(cmd_type)->second;
		};
};


#endif
