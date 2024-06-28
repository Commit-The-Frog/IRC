#include "Server.hpp"

Server::Server(int port, string pwd): _port(port), _pwd(pwd) {
	this->serv_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&this->serv_addr, 0, sizeof(this->serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
}

Server::~Server() {
	client_list.clear();
}

void	Server::cmdMapInit()
{
	this->cmd_map[PASS] = new Pass(client_list, this->_pwd);

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

const char* Server::KeventError::what() const throw(){
	return ("Kevent Exception!!");
}

const char* Server::ClientAcceptError::what() const throw(){
	return ("Client Accept Exception!!");
}

/*	[changeEvents]
	Add event listener and reciever
*/
void	Server::changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, 0, 0, NULL);
	change_list.push_back(temp_event);
}

void	Server::run()
{
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
	cmdMapInit();
	while(true)
	{
		int event_cnt = kevent(kq, change_list.data(), change_list.size(), event_list, event_size, NULL);
		if (event_cnt < 0)
			throw Server::KeventError();
		change_list.clear();
		for (int i = 0; i < event_cnt; i++)
		{
			curr_event = &event_list[i];
			it = client_list.find(curr_event->ident);
			if (curr_event->filter == EVFILT_READ)
			{
				if ((int) curr_event->ident == serv_sock_fd)
				{
					registerClient(change_list);
				}
				else
				{
					if (it != client_list.end())
						recvEventFromClient(curr_event, it->second);
				}
			}else if (curr_event->filter == EVFILT_WRITE)
			{
				if (it != client_list.end())
					sendEventToClient(curr_event, it->second);
			}
		}
	}
}

void	Server::registerClient(std::vector<struct kevent>& change_list)
{
	struct sockaddr_in client_addr;
	socklen_t	client_len = sizeof(client_addr);
	int client_fd;

	client_fd = accept(this->serv_sock_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
		throw Server::ClientAcceptError();
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	changeEvents(change_list, client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
	changeEvents(change_list, client_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE);
	client_list[client_fd] = Client(client_fd);
	cout << client_fd << " Connect in" << endl;
}

void	Server::recvEventFromClient(struct kevent *curr_event, Client &client)
{
	char	buffer[512];
	int bytes = recv(curr_event->ident, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (bytes < 0)
	{
		disconnectClient(curr_event->ident, client_list);
	}
	else{
		buffer[bytes] = '\0';
		client.addRecvBuff(buffer);
		// 여기서 파싱을 해야함.
		while (true)
		{
			string client_recv_buff = client.getRecvBuff();
			unsigned long clrf_idx;
			if ((clrf_idx = client_recv_buff.find("\r\n")) == string::npos)
				break;
			string substr_data = client_recv_buff.substr(0, clrf_idx);
			client_recv_buff = client_recv_buff.substr(clrf_idx + 2);
			client.setRecvBuff(client_recv_buff);
			Parser cmd = Parser(substr_data);
			if (cmd.getCmd() == "PASS")
			{
				cmd_map[PASS]->execute(cmd, curr_event->ident);
			}
			// substr_data를 파싱객체에 넣어서 파싱
			// 파싱한 데이터를 각각 넣어서 execute
		}
	}
}

void	Server::sendEventToClient(struct kevent *curr_event, Client &client)
{
	if (client.getSendBuff() != "")
	{
		int bytes = send(curr_event->ident, client.getSendBuff().c_str(), client.getSendBuff().size(),0);
		if (bytes < 0)
		{
			disconnectClient(curr_event->ident, client_list);
		}
		else
			client.clearSendBuff();
	}
}

void	Server::disconnectClient(int client_fd, map<int, Client>&client_list)
{
	close(client_fd);
	client_list.erase(client_fd);
}