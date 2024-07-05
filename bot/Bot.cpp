#include "Bot.hpp"

void Bot::parseMsg(string str) {
	string source;
	string command;
	stringstream ss(str);

	ss >> source;
	ss >> command;
	if (command.compare("PRIVMSG") == 0) {
		string nick;
		char colon;
		string msg;
		ss >> nick;
		ss >> colon;
		ss >> msg;

		cout << nick << " : " << msg << endl;
		reverse(msg.begin(), msg.end());
		send_buff.append("privmsg " + nick + " :" + "reversed <" + msg + ">\r\n");
	}
}

void Bot::parseConnectMsg(string str) {
	string source;
	string command;
	stringstream ss(str);

	ss >> source;
	ss >> command;
	if (command.compare("001") == 0) {
		this->server_registered = true;
		cout << "Bot Nick : <" << bot_nick << ">" << endl;
		return ;
	} else if (command.compare("433") == 0) {
		string client;
		string nick;
		ss >> client;
		ss >> nick;
		if (server_registered)
			return ;
		cout << "<" << nick << "> is already in use" << endl;
		bot_nick += "_";
	}
}


Bot::Bot(string server_ip, string server_port, string server_pwd) : server_ip(server_ip), server_port(server_port), server_pwd(server_pwd) {
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		throw BotSocketCreateException();
	struct sockaddr_in	serv_addr;
	stringstream		ss;

	bot_nick = "bot";
	server_registered = false;
	memset(&serv_addr, 0, sizeof(sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	ss.str(server_port);
	int port;
	ss >> port;
	serv_addr.sin_port = htons(port);

	if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw BotSocketConnectException();
}

void Bot::connectToServer() {
	stringstream	init_ss;
	string			init_string;
	string			source;
	string			command;
	size_t			clrf_idx;


	while (!server_registered) {
		stringstream init_ss;
		init_ss << "PASS " << server_pwd << "\r\n";
		init_ss << "USER " << "Bot 0 * :Reverse Bot" << "\r\n";
		init_ss << "NICK " << bot_nick << "\r\n";
		init_string = init_ss.str();
		send(sock_fd, init_string.c_str(), init_string.size(), 0);
		char buffer[512];
		int bytes = recv(sock_fd, buffer, sizeof(buffer), 0);
		if (bytes > 0) {
			buffer[bytes] = '\0';
			recv_buff.append(buffer);
			while ((clrf_idx = recv_buff.find("\r\n")) != string::npos) {
				string substr_data = recv_buff.substr(0, clrf_idx);
				recv_buff = recv_buff.substr(clrf_idx + 2);
				if (substr_data.length() != 0)
					parseConnectMsg(substr_data);
				if (server_registered)
					break;
			}
		}
	}
}

void Bot::run() {
	stringstream	init_ss;
	string			init_string;
	int				init = 2;
	struct kevent	listen_event[2];
	struct kevent	curr_event;
	size_t			clrf_idx;

	connectToServer();
	this->kq = kqueue();
	if (this->kq < 0)
		throw KqueueInitException();
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	EV_SET(&listen_event[0], sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	EV_SET(&listen_event[1], sock_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	while(true) {
		int event_cnt = kevent(kq, listen_event, init, &curr_event, 1, NULL);
		if (init)
			init = 0;
		if (event_cnt < 0)
			throw KqueueInitException();
		if (event_cnt == 0)
			continue;
		if (curr_event.flags & EV_EOF) {
			cout << "Server closed" << endl;
			exit(0);
		}
		if (curr_event.filter == EVFILT_READ) {
			char buffer[512];
			int bytes = recv(curr_event.ident, buffer, sizeof(buffer), MSG_DONTWAIT);
			if (bytes > 0) {
				buffer[bytes] = '\0';
				recv_buff.append(buffer);
				while ((clrf_idx = recv_buff.find("\r\n")) != string::npos) {
					string substr_data = recv_buff.substr(0, clrf_idx);
					recv_buff = recv_buff.substr(clrf_idx + 2);
					parseMsg(substr_data);
				}
			}
		} else if (curr_event.filter == EVFILT_WRITE) {
			if (send_buff.length() > 0) {
				send(sock_fd, send_buff.c_str(), send_buff.size(), MSG_DONTWAIT);
				send_buff.clear();
			}
		}
	}
}
