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
#include "../command/CommandFactory.hpp"

using namespace std;

class Server {
	private:
		const int				_port;
		const string			_pwd;
		struct sockaddr_in		serv_addr;
		int						serv_sock_fd;
		int						kq;
		std::map<int, Client>	client_list;
		CommandFactory			commandFactory;

		Server();

	public:
		Server(int port, string pwd);
		~Server();
		void	serverSocketBind();
		void	serverSocketListen(int listen_queue_size);
		void	initKq();
		void	run();
		void	registerClient(std::vector<struct kevent>& change_list);
		void	recvEventFromClient(struct kevent *curr_event, Client& client);
		void	sendEventToClient(struct kevent *curr_event, Client& client);
		void	disconnectClient(int client_fd, map<int, Client>& client_list);
		void	changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags);

		class ServerSocketBindException : public exception {
			public:
				virtual const char* what() const throw();
		};
		class ServerSocketListenException : public exception {
			public:
				virtual const char* what() const throw();
		};
		class KqueueInitException : public exception {
			public:
				virtual const char* what() const throw();
		};
		class KeventSettingError : public exception {
			public:
				virtual const char* what() const throw();
		};
		class KeventError : public exception {
			public:
				virtual const char* what() const throw();
		};
		class ClientAcceptError : public exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif
