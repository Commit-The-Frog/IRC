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
	channel_name = channel.channel_name;
	operator_set = channel.operator_set;
	member_map = channel.member_map;
	invite_map = channel.invite_map;
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
	for (std::map<string, Client *>::iterator it = invite_map.begin(); it != invite_map.end(); it++) {
		Client *client = it->second;
		client->deleteInvite(this->channel_name);
	}
}

void Channel::setChannelName(const string &channel_name) {
	this->channel_name = channel_name;
}

const string &Channel::getChannelName() const {
	return this->channel_name;
}

void Channel::addOperator(const string &nick) {
	operator_set.insert(nick);
}

void Channel::deleteOperator(const string &nick) {
	operator_set.erase(nick);
}

void Channel::addMember(const string &nick, Client &client) {
	member_map[nick] = &client;
	client.addChannel(this->channel_name, *this);
}

void Channel::deleteMember(const string &nick, Client &client) {
	member_map.erase(nick);
	client.deleteChannel(channel_name);
}

void Channel::changeMemberName(const string &prevnick, const string &changenick) {
	if (isMember(prevnick)) {
		Client *temp = member_map.find(prevnick)->second;
		member_map.erase(prevnick);
		member_map[changenick] = temp;
	}
	if (isOperator(prevnick)) {
		operator_set.erase(prevnick);
		operator_set.insert(changenick);
	}
	if (isInvited(prevnick)) {
		invite_map[changenick] = invite_map[prevnick];
		invite_map.erase(prevnick);
	}
}

void Channel::initial() {
	options[MODE_I] = false;
	options[MODE_T] = true;
	options[MODE_K] = false;
	options[MODE_L] = false;
	key = "";
	topic = "";
	limit = 0;
}

void Channel::addInvite(const string &nick, Client& client) {
	invite_map[nick] = &client;
	client.addInvite(channel_name, *this);
}

void Channel::deleteInvite(const string &nick, Client& client) {
	invite_map.erase(nick);
	client.deleteInvite(channel_name);
}

const std::map<string, Client *> &Channel::getMemberMap() const {
	return member_map;
}

bool Channel::isOperator(const string &nick) const {
	return operator_set.find(nick) != operator_set.end();
}

bool Channel::isMember(const string &nick) const {
	return member_map.find(nick) != member_map.end();
}

bool Channel::isInvited(const string &nick) const {
	return invite_map.find(nick) != invite_map.end();
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

string Channel::getTopicWhoTime() const {
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
			it->second->addSendBuff(msg);
	}
}
