#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "Command.hpp"

class Topic: public Command {
	public:
		Topic(map<int, Client>& client_map, map<string, Channel>& channel_map)
		:Command(client_map, channel_map) {};
		virtual void execute(const Parser& parser, int client_fd);
		virtual ~Topic() {};
};

#endif
