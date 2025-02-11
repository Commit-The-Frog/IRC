#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include "../channel/Channel.hpp"
#include <map>
#include <exception>
#include <set>

using namespace std;

class Channel;

class Client
{
	private:
		int client_fd;
		string send_buff;
		string recv_buff;
		string nickname;
		string username;
		string realname;
		bool is_passed;
		string ip_addr;
		map<string, Channel *> channel_map;
		map<string, Channel *> invite_map;
		static map<string, int>	nick_map;
	public:
		Client();
		Client(int client_fd, const string& ip_addr);
		~Client();
		void setNickname(const string&);
		string getNickname() const;
		void setUsername(const string&);
		string getUsername() const;
		void setRealname(const string&);
		string getRealname() const;
		string getIpAddr() const;
		void addSendBuff(const string& data);
		void addRecvBuff(const string& data);
		void setSendBuff(const string& data);
		void setRecvBuff(const string& data);
		string getSendBuff() const;
		string getRecvBuff() const;
		void clearSendBuff();
		bool getIsRegistered() const;
		void setIsPassedTrue();
		bool getIsPassed() const;
		void addChannel(const string &channel_name, Channel &channel);
		void deleteChannel(const string &channel_name);
		void addInvite(const string& channel_name, Channel& channel);
		void deleteInvite(const string& channel_name);

		static int getSockFdByNick(const string& nick);

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
		class NoSuchNickException: public exception {
			public:
				virtual const char* what() const throw() {
					return (":No such nick/channel");
				}
		};
};

#endif
