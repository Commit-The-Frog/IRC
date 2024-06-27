#include "Client.hpp"
#include "Command.hpp"

Client::Client(int client_fd, string server_pwd) : client_fd(client_fd) {}
Client::~Client() {}

/* 
	recv_buff에 data append
*/
void Client::addRecvBuff(const string& data) {
	recv_buff.append(data);
}

/*
	recv_buff를 data로 대체
*/
void Client::setRecvBuff(const string& data) {
	recv_buff = data;
}

/*
	send_buff 반환
*/
string Client::getSendBuff() const {
	return send_buff;
}

/*
	send_buff 비움
*/
void Client::clearSendBuff() {
	send_buff.clear();
}
