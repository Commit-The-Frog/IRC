#include "Client.hpp"

map<string, int> Client::nick_map;

Client::Client() {}
Client::Client(int client_fd, const string& ip_addr) : client_fd(client_fd), ip_addr(ip_addr), is_passed(false) {}
Client::~Client() {
	nick_map.erase(this->nickname);
	map<string, Channel *>::iterator it;
	it = channel_map.begin();
	while (it != channel_map.end()) {
		map<string, Channel *>::iterator tmp_it = it;
		it++;
		Channel &curr_channel = *(tmp_it->second);
		curr_channel.deleteInvite(this->nickname, *this);
		curr_channel.deleteOperator(this->nickname);
		curr_channel.deleteMember(this->nickname, *this);
	}
	it=invite_map.begin();
	while (it != invite_map.end()) {
		map<string, Channel *>::iterator tmp_it = it;
		tmp_it->second->deleteInvite(this->nickname, *this);
		it++;
	}
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

string Client::getNickname() const {
	return nickname;
}

void Client::setUsername(const string& username) {
	this->username = username;
}

string Client::getUsername() const {
	return username;
}

void Client::setRealname(const string& realname) {
	this->realname = realname;
}

string Client::getRealname() const {
	return realname;
}

string Client::getIpAddr() const {
	return ip_addr;
}

void Client::setSendBuff(const string& data) {
	this->send_buff.append(data);
}

bool Client::getIsRegistered() const {
	return (is_passed && this->username.length() != 0 && this->nickname.length() != 0);
}

void Client::setIsPassedTrue() {
	this->is_passed = true;
}

bool Client::getIsPassed() const {
	return this->is_passed;
}

void Client::addChannel(const string &channel_name, Channel &channel) {
	if (channel_map.find(channel_name) == channel_map.end())
		channel_map[channel_name] = &channel;
}

void Client::deleteChannel(const string &channel_name) {
	channel_map.erase(channel_name);
}

void Client::addInvite(const string& channel_name, Channel& channel) {
	if (invite_map.find(channel_name) == invite_map.end())
		invite_map[channel_name] = &channel;
}

void Client::deleteInvite(const string& channel_name) {
	invite_map.erase(channel_name);
}

int	Client::getSockFdByNick(const string& nick) {
	map<string, int>::iterator it = nick_map.find(nick);
	if (it == nick_map.end())
		throw Client::NoSuchNickException();
	return (it->second);
}
