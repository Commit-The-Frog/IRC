#include "Server.hpp"

Server::Server(int port, string pwd): _port(port), _pwd(pwd) {
	this->serv_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&this->serv_addr, 0, sizeof(this->serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
}

Server::~Server() {
	delete[] client_list;
}

void	Server::serverSocketBind() {
	if (::bind(this->serv_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		throw Server::ServerSocketBindException();
}

void	Server::serverSocketListen(int listen_queue_size) {
	if (listen(this->serv_sock_fd, listen_queue_size) < 0)
		throw Server::ServerSocketListenException();
}

void	Server::initKq() {
	this->kq = kqueue();
	if (this->kq < 0)
		throw Server::KqueueInitException();
}

void	Server::addKqEventListener(int socket, int16_t filter) {
	struct kevent	change;

	EV_SET(&change, socket, filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(this->kq, &change, 1, NULL, 0, NULL) < 0)
		throw Server::KeventSettingError();
}

const char* Server::ServerSocketBindException::what() const throw() {
	return ("Server Socket Bind Exception!!");
}

const char* Server::ServerSocketListenException::what() const throw() {
	return ("Server Socket Listen Exception!!");
}

const char* Server::KqueueInitException::what() const throw() {
	return ("Kqueue Init Exception!!");
}

const char* Server::KeventSettingError::what() const throw() {
	return ("Kevent Setting Exception!!");
}
