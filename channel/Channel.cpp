#include "Channel.hpp"

Channel::Channel(Channel const &channel) {
	operator_map = channel.operator_map;
	member_map = channel.member_map;
	inivite_map = channel.inivite_map;
	key = channel.key;
	topic = channel.topic;
	for (int i = 0; i <5; i++)
		options[i]  = channel.options[i];
}

Channel &Channel::operator=(Channel const &channel) {
	operator_map = channel.operator_map;
	member_map = channel.member_map;
	inivite_map = channel.inivite_map;
	key = channel.key;
	topic = channel.topic;
	for (int i = 0; i <5; i++)
		options[i]  = channel.options[i];
}

void Channel::addOperatorMap(int fd, Client &client) {
	operator_map[fd] = client;
}

void Channel::deleteOperatorMap(int fd , Client &client) {
	operator_map.erase(fd);
}

void Channel::addMemberMap(int fd, Client &client) {
	member_map[fd] = client;
}

void Channel::deleteMemberMap(int fd , Client &client) {
	member_map.erase(fd);
}

void Channel::addInviteMap(int fd, Client &client) {
	inivite_map[fd] = client;
}

void Channel::deleteInviteMap(int fd , Client &client) {
	inivite_map.erase(fd);
}

std::map<int, Client> &Channel::getOperatorMap() {
	return operator_map;
}

std::map<int, Client> &Channel::getMemberMap() {
	return member_map;
}

std::map<int, Client> &Channel::getInviteMap() {
	return inivite_map;
}

void Channel::setKey(std::string str) {
	key = str;
}

std::string &Channel::getKey() {
	return key;
}

void Channel::setTopic(std::string str) {
	key = str;
}

void Channel::setTopic(std::string str) {
	topic = str;
}

std::string &Channel::getTopic() {
	return topic;
}

bool Channel::getModeOptionI() const {
	return options[MODE_I];
}

bool Channel::getModeOptionT() const {
	return options[MODE_T];
}

bool Channel::getModeOptionK() const {
	return options[MODE_K];
}

bool Channel::getModeOptionO() const {
	return options[MODE_O];
}


bool Channel::getModeOptionL() const {
	return options[MODE_L];
}

void Channel::setModeOptionI(bool flag) {
	options[MODE_I] = flag;
}

void Channel::setModeOptionT(bool flag) {
	options[MODE_T] = flag;
}

void Channel::setModeOptionK(bool flag) {
	options[MODE_K] = flag;
}

void Channel::setModeOptionO(bool flag) {
	options[MODE_O] = flag;
}

void Channel::setModeOptionL(bool flag) {
	options[MODE_L] = flag;
}