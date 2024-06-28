#include "Client.hpp"

Client::Client() {}
Client::Client(int client_fd) : client_fd(client_fd) {}
Client::~Client() {}

void Client::addRecvBuff(const string& data) {
	recv_buff.append(data);
}

void Client::setRecvBuff(const string& data) {
	recv_buff = data;
}

string Client::getRecvBuff() const {
	return recv_buff;
}

string Client::getSendBuff() const {
	return send_buff;
}

void Client::clearSendBuff() {
	send_buff.clear();
}

void Client::setNickname(const string& nickname) {
	this->nickname = nickname;
}

string Client::getNickname() {
	return nickname;
}

void Client::setUsername(const string& username) {
	this->username = username;
}

string Client::getUsername() {
	return username;
}

void Client::setSendBuff(const string& data) {
	this->send_buff.append(data);
}
