#include "Client.hpp"

set<string> Client::nick_set;

Client::Client() {}
Client::Client(int client_fd) : client_fd(client_fd) {}
Client::~Client() {
	nick_set.erase(this->nickname);
}

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

void Client::setNickname(const string& new_nick) {
	if (new_nick.compare(this->nickname) == 0)
		throw Client::SameNickException();
	if (nick_set.find(new_nick) != nick_set.end())
		throw Client::AlreadyInUseNickException();
	if (this->nickname.length() > 0)
		nick_set.erase(this->nickname);
	this->nickname = new_nick;
	nick_set.insert(new_nick);
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

