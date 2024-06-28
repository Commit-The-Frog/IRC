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

void Channel::addOperator(string nick, Client &client) {
	operator_map[nick] = client;
}

void Channel::deleteOperator(string nick, Client &client) {
	operator_map.erase(nick);
}

void Channel::addMember(string nick, Client &client) {
	member_map[nick] = client;
}

void Channel::deleteMember(string nick, Client &client) {
	member_map.erase(nick);
}

void Channel::addInvite(string nick, Client &client) {
	inivite_map[nick] = client;
}

void Channel::deleteInvite(string nick, Client &client) {
	inivite_map.erase(nick);
}

std::map<string, Client> &Channel::getOperatorMap() const {
	return operator_map;
}

std::map<string, Client> &Channel::getMemberMap() const {
	return member_map;
}

std::map<string, Client> &Channel::getInviteMap() const {
	return inivite_map;
}

void Channel::setKey(string str) {
	key = str;
}

string &Channel::getKey() const {
	return key;
}

void Channel::setTopic(string str) {
	key = str;
}

void Channel::setTopic(string str) {
	topic = str;
}

string &Channel::getTopic() const {
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