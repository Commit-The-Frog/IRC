#include "Channel.hpp"

Channel::Channel(const string &str) {
	channel_name = str;
	options[MODE_I] = false;
	options[MODE_T] = true;
	options[MODE_K] = false;
	options[MODE_L] = false;
}

Channel::Channel(const Channel &channel) {
	*this = channel;
}

Channel &Channel::operator=(const Channel &channel) {
	operator_map = channel.operator_map;
	member_map = channel.member_map;
	inivite_map = channel.inivite_map;
	key = channel.key;
	topic = channel.topic;
	for (int i = 0; i < 4; i++)
		options[i]  = channel.options[i];
	return (*this);
}

Channel::~Channel() {
	for (std::map<string, Client *>::iterator it = member_map.begin(); it != member_map.end(); it++) {
		Client *client = it->second;
		client->deleteChannel(this->channel_name);
	}
}

void Channel::setChannelName(const string &channel_name) {
	this->channel_name = channel_name;
}

const string &Channel::getChannelName() const {
	return this->channel_name;
}

void Channel::addOperator(const string &nick, Client &client) {
	operator_map[nick] = &client;
}

void Channel::deleteOperator(const string &nick) {
	operator_map.erase(nick);
}

void Channel::addMember(const string &nick, Client &client) {
	member_map[nick] = &client;
	client.addChannel(this->channel_name, *this);
}

void Channel::deleteMember(const string &nick, Client &client) {
	member_map.erase(nick);
	client.deleteChannel(channel_name);
}

void Channel::addInvite(const string &nick, Client &client) {
	inivite_map[nick] = &client;
}

void Channel::deleteInvite(const string &nick) {
	inivite_map.erase(nick);
}

const std::map<string, Client *> &Channel::getOperatorMap() const {
	return operator_map;
}

const std::map<string, Client *> &Channel::getMemberMap() const {
	return member_map;
}

const std::map<string, Client *> &Channel::getInviteMap() const {
	return inivite_map;
}

bool Channel::isOperator(const string &nick) const {
	return operator_map.find(nick) != operator_map.end();
}

bool Channel::isMember(const string &nick) const {
	return member_map.find(nick) != member_map.end();
}

bool Channel::isInvited(const string &nick) const {
	return inivite_map.find(nick) != inivite_map.end();
}

void Channel::setKey(const string &str) {
	key = str;
}

const string &Channel::getKey() const {
	return key;
}

void Channel::setTopic(const string &str, const Client& client) {
	string user_str = client.getNickname() + "!~" + client.getUsername() + "@" + client.getIpAddr();
	stringstream ss;

	ss << time(NULL);
	topic = str;
	latest_topic_set_user = user_str;
	latest_topic_set_time = ss.str();
}

const string &Channel::getTopic() const {
	return topic;
}

const string &Channel::getTopicWhoTime() const {
	return (this->latest_topic_set_user + " " + this->latest_topic_set_time);
}

void Channel::setLimit(int limit) {
	this->limit = limit;
}

int Channel::getLimit() const {
	return this->limit;
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

void Channel::setModeOptionL(bool flag) {
	options[MODE_L] = flag;
}

void Channel::sendToAllMembers(const string &sender, const string &msg) {
	for (std::map<string, Client *>::iterator it = member_map.begin(); it != member_map.end(); ++it) {
		if (it->second->getNickname().compare(sender) != 0)
			it->second->setSendBuff(msg);
	}
}
