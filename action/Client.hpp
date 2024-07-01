#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include <set>
#include <exception>
using namespace std;

class Client
{
	private:
		int client_fd;
		string send_buff;
		string recv_buff;
		string nickname;
		string username;
		static set<string>	nick_set;
		// map<Channel> channel_list;
	public:
		Client();
		Client(int client_fd);
		~Client();
		void setNickname(const string&);
		string getNickname();
		void setUsername(const string&);
		string getUsername();
		void addRecvBuff(const string& data);
		void setRecvBuff(const string& data);
		string getRecvBuff() const;
		string getSendBuff() const;
		void setSendBuff(const string& data);
		void clearSendBuff();

		class AlreadyInUseNickException: public exception {
			public:
				virtual const char* what() const throw() {
					return ("Duplicated User Nickname");
				}
		};
		class SameNickException: public exception {
			public:
				virtual const char* what() const throw() {
					return ("Same User Nickname");
				}
		};
};

#endif
