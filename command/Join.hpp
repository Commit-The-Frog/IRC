#ifndef JOIN_HPP
# define JOIN_HPP

#include "Command.hpp"
#include <map>
#include <string>
#include <vector>

using namespace std;

class Join : public Command {
	public :
		Join(map<int, Client> &client_map, map<string, Channel> &channel_map) : Command(client_map, channel_map) {};
		~Join(){};
	void execute(const Parser &parser, int fd);
	void verificateKey(const Channel &channel);
	void verificateInvite(const Channel &channel);
	void verificateLimit(const Channel &channel);
	class JoinVerficateKeyException : public exception {
		public :
			virtual const char* what() const throw();
	};
	class JoinVerficateInviteException : public exception {
		public :
			virtual const char* what() const throw();
	};
	class JoinVerficateInviteException : public exception {
		public :
			virtual const char* what() const throw();
	};
};

#endif