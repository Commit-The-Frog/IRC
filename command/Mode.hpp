#ifndef MODE_HPP
# define MODE_HPP

# include "Command.hpp"
# include <vector>
# include <string>

using namespace std;

class Mode: public Command {
	private:
		string	getModeString(vector<string>& str_v);
		string	getCurModeString(const Channel& channel);
	public:
		Mode(map<int, Client>& client_map, map<string, Channel>& channel_map)
		:Command(client_map, channel_map) {};
		virtual void execute(const Parser& parser, int client_fd);
};

#endif
