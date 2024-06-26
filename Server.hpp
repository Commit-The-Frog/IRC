#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <exception>
# include <sys/socket.h>
# include <netinet/in.h>
#include <sys/event.h>
// # include "Client.hpp"

using namespace std;

class Client;

class Server
{
	private:
		const string		_pwd;
		const int			_port;
		struct sockaddr_in	serv_addr;
		int					serv_sock_fd;
		int					kq;
		Client				*client_list;

		Server();
		void	addKqEventListener(int socket, int16_t filter); // try 필요

	public:
		Server(int port, string pwd); // serv_addr도 설정해서 bind 전까지 완료!
		~Server();
		void	serverSocketBind(); // error throw 예정
		void	serverSocketListen(int listen_queue_size); // error throw 예정
		void	initKq(); // try 필요
		void	run(); // 무한루프로 소켓 확인.

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
};

#endif
