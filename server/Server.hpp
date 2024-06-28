#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <vector>
#include <map>
#include <fcntl.h>
#include <unistd.h>
#include "../action/Client.hpp"
#include "../command/Pass.hpp"
#include "../command/Command.hpp"
#include "../parser/Parser.hpp"
enum {
	PASS,
	NICK,
	USER,
	PRIVMSG
};

using namespace std;

class Client;

class Server
{
	private:
		const int			_port;
		const string		_pwd;
		struct sockaddr_in	serv_addr;
		int					serv_sock_fd;
		int					kq;
		std::map<int, Client>client_list;
		std::map<int, Command *>cmd_map;
		//Client				*client_list;

		Server();

	public:
		Server(int port, string pwd); // serv_addr도 설정해서 bind 전까지 완료!
		~Server();
		void	serverSocketBind(); // error throw 예정
		void	serverSocketListen(int listen_queue_size); // error throw 예정
		void	initKq(); // try 필요
		void	run(); // 무한루프로 소켓 확인.
		void	registerClient(std::vector<struct kevent>& change_list);
		void	recvEventFromClient(struct kevent *curr_event, Client &client);
		void	sendEventToClient(struct kevent *curr_event, Client &client);
		void	disconnectClient(int client_fd, map<int, Client>&client_list);
		void	changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags);
		void	cmdMapInit();
		
		class ServerSocketBindException : public exception{
			public:
				virtual const char* what() const throw();
		};
		class ServerSocketListenException : public exception{
			public:
				virtual const char* what() const throw();
		};
		class KqueueInitException : public exception{
			public:
				virtual const char* what() const throw();
		};
		class KeventSettingError : public exception{
			public:
				virtual const char* what() const throw();
		};
		class KeventError : public exception{
			public:
				virtual const char* what() const throw();
		};
		class ClientAcceptError : public exception{
			public:
				virtual const char* what() const throw();
		};
};

#endif
