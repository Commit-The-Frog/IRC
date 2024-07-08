#include "Server.hpp"

Server::Server(int port, string pwd): _port(port), _pwd(pwd), commandFactory(client_map,channel_map, pwd) {
	int option;
	socklen_t optlen;

	this->serv_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&this->serv_addr, 0, sizeof(this->serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	optlen = sizeof(option);
	option = true;
	setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);
}

Server::~Server() {
	client_map.clear();
	channel_map.clear();
}

/*	[serverSocketBind]
	bind created socket with address
*/
void	Server::serverSocketBind() {
	if (::bind(this->serv_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		throw Server::ServerSocketBindException();
}

/*	[serverSocketListen]
	listen socket
*/
void	Server::serverSocketListen(int listen_queue_size) {
	if (listen(this->serv_sock_fd, listen_queue_size) < 0)
		throw Server::ServerSocketListenException();
}

/*	[initKq]
	get kqueue file discripter
*/
void	Server::initKq() {
	this->kq = kqueue();
	if (this->kq < 0)
		throw Server::KqueueInitException();
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

const char* Server::KeventError::what() const throw() {
	return ("Kevent Exception!!");
}

const char* Server::ClientAcceptError::what() const throw() {
	return ("Client Accept Exception!!");
}

/*	[changeEvents]
	Add event listener and reciever
*/
void	Server::changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags) {
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, 0, 0, NULL);
	change_list.push_back(temp_event);
}

void	Server::run() {
	int event_size = 10;
	std::vector<struct kevent> change_list;
	struct kevent event_list[event_size];
	struct kevent *curr_event;
	std::map<int, Client>::iterator it;

	serverSocketBind();
	serverSocketListen(event_size);
	initKq();
	fcntl(serv_sock_fd, F_SETFL, O_NONBLOCK);
	changeEvents(change_list, serv_sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
	while(true) {
		int event_cnt = kevent(kq, change_list.data(), change_list.size(), event_list, event_size, NULL);
		if (event_cnt < 0)
			throw Server::KeventError();
		change_list.clear();
		for (int i = 0; i < event_cnt; i++) {
			curr_event = &event_list[i];
			it = client_map.find(curr_event->ident);
			if (curr_event->flags & EV_EOF) {
				disconnectClient(curr_event->ident);
				continue;
			}
			if (curr_event->filter == EVFILT_READ) {
				if (static_cast<int>(curr_event->ident) == serv_sock_fd) {
					registerClient(change_list);
				} else {
					if (it != client_map.end())
						recvEventFromClient(curr_event, it->second);
				}
			} else if (curr_event->filter == EVFILT_WRITE) {
				if (it != client_map.end())
					sendEventToClient(curr_event, it->second);
			}
		}
	}
}

/*	[registerClient]
	accept the connection requset
	add event listener for new socket to kqueue
*/
void	Server::registerClient(std::vector<struct kevent>& change_list) {
	struct sockaddr_in client_addr;
	socklen_t	client_len = sizeof(client_addr);
	int client_fd;

	client_fd = accept(this->serv_sock_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
		throw Server::ClientAcceptError();
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	changeEvents(change_list, client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
	changeEvents(change_list, client_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE);
	string sock_addr = string(inet_ntoa(client_addr.sin_addr));
	client_map[client_fd] = Client(client_fd, sock_addr);
	cout << client_fd << ": " << sock_addr << " connected" << endl;
}

/*	[recvEventFromClient]
	1. 	Recive event from client
	2. 	Get Command instance for execute
	3. 	Execute Command instance
		(Command instance will put output to client's recive buff)
*/
void	Server::recvEventFromClient(struct kevent *curr_event, Client &client) {
	char			buffer[512];
	unsigned long	clrf_idx;

	int bytes = recv(curr_event->ident, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes <= 0) {
		return ;
	} else {
		buffer[bytes] = '\0';
		client.addRecvBuff(buffer);
		while (true) {
			string client_recv_buff = client.getRecvBuff();
			if ((clrf_idx = client_recv_buff.find("\r\n")) == string::npos)
				break;
			string substr_data = client_recv_buff.substr(0, clrf_idx);
			cout << "MSG >>>" << client.getNickname() << " : " << substr_data << endl;
			client_recv_buff = client_recv_buff.substr(clrf_idx + 2);
			client.setRecvBuff(client_recv_buff);
			Parser parsed_data = Parser(substr_data);
			try {
				commandFactory.generateCommand(parsed_data)->execute(parsed_data, curr_event->ident);
			} catch (exception& e) { // unknown command exception 만 catch 필요.
				// unknown command에 대한 처리 필요 421) unknown command
			}
		}
	}
}

/*	[sendEventToClient]
	If socket is available to send,
	send data(in send_buff) to client
*/
void	Server::sendEventToClient(struct kevent *curr_event, Client &client) {
	if (client.getSendBuff() != "") {
		int bytes = send(curr_event->ident, client.getSendBuff().c_str(), client.getSendBuff().size(), MSG_DONTWAIT);
		if (bytes < 0)
			return ;
		else
			client.clearSendBuff();
	}
}

void	Server::disconnectClient(int client_fd) {
	close(client_fd);
	client_map.erase(client_fd);
	map<string, Channel>::iterator it = channel_map.begin();
	while ( it != channel_map.end()) {
		map<string, Channel>::iterator tmp_it = it;
		it++;
		if (tmp_it->second.getMemberMap().size() < 1) {
			channel_map.erase(tmp_it->first);
		}
	}
}