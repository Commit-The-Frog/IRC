#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Client.hpp"

class Client;

class Server
{
	private:
		const std::string	_pwd;
		const int			_port;
		struct sockaddr_in	serv_addr;
		int					serv_sock_fd;
		int					kq;
		Client				*client_list;

		Server();

	public:
		Server(int port, std::string pwd); // serv_addr도 설정해서 bind 전까지 완료!
		void	bind(); // error throw 예정
		void	listen(); // error throw 예정
		void	init_kq(); // try 필요
		void	register_change_kq(); // try 필요
		void	run(); // 무한루프로 소켓 확인.
};

#endif
