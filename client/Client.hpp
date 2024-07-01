#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include "../reply/Reply.hpp"
#include "../channel/Channel.hpp"
#include <map>
#include <exception>

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
		map<string, Channel> channel_map;
		static map<string, int>	nick_map;
	public:
		Client();
		Client(int client_fd, const string& ip_addr);
		~Client();
		void setNickname(const string&);
		string getNickname();
		void setUsername(const string&);
		string getUsername();
		void setRealname(const string&);
		string getRealname();
		void addRecvBuff(const string& data);
		void setRecvBuff(const string& data);
		string getRecvBuff() const;
		string getSendBuff() const;
		void setSendBuff(const string& data);
		void setSendBuff(const Reply& data);
		void clearSendBuff();
		bool getIsRegistered(); // set으ㄹ 빼버리고... 
		void setIsPassedTrue();
		bool getIsPassed();

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
					return ("No Such Nickname");
				}
		};
};

#endif
