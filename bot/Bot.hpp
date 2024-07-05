#ifndef BOT_HPP
# define BOT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sstream>
#include <fcntl.h>
#include <vector>
#include <algorithm>

using namespace std;

class Bot {
	private:
		string	server_ip;
		string	server_port;
		string	server_pwd;
		int		sock_fd;
		int		kq;
		bool	server_registered;
		string	recv_buff;
		string	send_buff;
		string	bot_nick;

		void parseMsg(string msg);
		void parseConnectMsg(string msg);
		void connectToServer();
	public:
		Bot(string server_ip, string server_port, string server_pwd);
		void run();

		class BotSocketCreateException : public exception {
			public:
				virtual const char* what() const throw() {
					return ("Socket Create Error");
				}
		};
		class BotSocketConnectException : public exception {
			public:
				virtual const char* what() const throw() {
					return ("Socket Connect Error");
				}
		};
		class KqueueInitException : public exception {
			public:
				virtual const char* what() const throw() {
					return ("Kqueue Error");
				}
		};
};

#endif
