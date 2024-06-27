#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
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
		Client(int client_fd, string server_pwd);
		~Client();
		void addRecvBuff(const string& data);
		void setRecvBuff(const string& data);
		string getSendBuff() const;
		void clearSendBuff();
};

#endif