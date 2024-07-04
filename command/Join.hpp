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
	map<string, string> joinParse(const string &channels, const string &keys);
	void verificateKey(const Channel &channel, const string &str);
	void verificateInvite(const Channel &channel, int client_fd);
	void verificateLimit(const Channel &channel);
	class JoinVerificateKeyException : public exception {
		public :
			virtual const char* what() const throw();
	};
	class JoinVerificateInviteException : public exception {
		public :
			virtual const char* what() const throw();
	};
	class JoinVerificateLimitException : public exception {
		public :
			virtual const char* what() const throw();
	};
};

#endif