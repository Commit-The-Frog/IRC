#ifndef COMMAND_FACTORY_HPP_
#define COMMAND_FACTORY_HPP_

#include "../action/Client.hpp"
// #include "../channel/Channel.hpp"
#include "Command.hpp"
#include "Pass.hpp"
#include "Nick.hpp"

class CommandFactory
{
	private:
		map<int, Client>& client_map;
		// map<int, Channel>& channel_map;
		string server_pwd;
		map<int, Command*> cmd_map;
	public:
		// CommandFactory(map<int, Client>& client_map, map<int, Channel>& channel_map, const string& server_pwd)
		// :client_map(client_map), channel_map(channel_map), server_pwd(server_pwd)
		// {
		// 	cmd_map[PASS] = new Pass(this->client_map, this->channel_map, this->server_pwd);
		// };
		CommandFactory(map<int, Client>& client_map, const string& server_pwd)
		:client_map(client_map), server_pwd(server_pwd)
		{
			cmd_map[PASS] = new Pass(this->client_map, this->server_pwd);
			cmd_map[NICK] = new Nick(this->client_map);
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
			// ...
			else
				throw invalid_argument("COMMAND NOT FOUND");
			return cmd_map.find(cmd_type)->second;
		};
};


#endif
