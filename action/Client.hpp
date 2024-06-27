#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include "Command.hpp"

class Client
{
	private:
		int client_fd;
		const string server_pwd;
		string send_buff;
		string recv_buff;
		string nickname;
		string username;
	public:
		Client(int client_fd, string server_pwd);
		~Client();
		void setRecvBuff(const string& data);
		string getSendBuff() const;
		void clearSendBuff();
};

#endif