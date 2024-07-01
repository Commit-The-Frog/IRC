#include "Client.hpp"

map<string, int> Client::nick_map;

Client::Client() {}
Client::Client(int client_fd, const string& ip_addr) : client_fd(client_fd), ip_addr(ip_addr), is_passed(false) {}
Client::~Client() {
	nick_map.erase(this->nickname);
	//channel map에 대한 나가기도 필요함.
	cout << this->client_fd << ": " << this->ip_addr << " disconnected" << endl;
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
	if (nick_map.find(new_nick) != nick_map.end())
		throw Client::AlreadyInUseNickException();
	if (this->nickname.length() > 0)
		nick_map.erase(this->nickname);
	this->nickname = new_nick;
	nick_map[new_nick] = this->client_fd;
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

void Client::setRealname(const string& realname) {
	this->realname = realname;
}

string Client::getRealname() {
	return realname;
}

void Client::setSendBuff(const string& data) {
	this->send_buff.append(data);
}

void Client::setSendBuff(const Reply& data) {
	this->send_buff.append(data.getString());
}

bool Client::getIsRegistered() {
	return (is_passed && this->username.length() != 0 && this->nickname.length() != 0);
}

void Client::setIsPassedTrue() {
	this->is_passed = true;
}

bool Client::getIsPassed () {
	return this->is_passed;
}

int	Client::getSockFdByNick(const string& nick) {
	map<string, int>::iterator it = nick_map.find(nick);
	if (it == nick_map.end())
		throw Client::NoSuchNickException();
	return (it->second);
}
