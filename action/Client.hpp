#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include "../reply/Reply.hpp"
using namespace std;

class Client
{
	private:
		int client_fd;
		string send_buff;
		string recv_buff;
		string nickname;
		string username;
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
		void setSendBuff(const Reply& data);
		void clearSendBuff();
};

#endif
