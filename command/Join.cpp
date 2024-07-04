#include "Join.hpp"

const char* Join::JoinVerificateLimitException::what() const throw() {
	return ("Join verificate limit Exception!");
}

const char* Join::JoinVerificateKeyException::what() const throw() {
	return ("Join verificate key Exception!");
}

const char* Join::JoinVerificateInviteException::what() const throw() {
	return ("Join verificate invite Exception!");
}

void Join::verificateKey(const Channel &channel, const string &str) {
	if (channel.getModeOptionK())  {
		if (channel.getKey() != str) {
			throw Join::JoinVerificateKeyException();
		}
	}
}

void Join::verificateInvite(const Channel &channel, int client_fd) {
	if (channel.getModeOptionI()) {
		if (!channel.isInvited(client_map[client_fd].getNickname()))
		throw Join::JoinVerificateInviteException();
	}
}

void Join::verificateLimit(const Channel &channel) {
	if (channel.getModeOptionL()) {
		if (channel.getLimit() == channel.getMemberMap().size())
			throw Join::JoinVerificateLimitException();
	}
}

map<string,string> Join::joinParse(const string &channels, const string &keys) {
	std::stringstream channels_(channels);
	std::stringstream keys_(keys);
    std::string channel;
	std::string key;
	map<string, string> join_map;

	std::getline(keys_, key, ',');
    while (std::getline(channels_, channel, ',')) {
		if (join_map.find(channel) == join_map.end()) {
			if (std::getline(keys_, key, ',')) {
				join_map[channel] = key;
			} else {
				join_map[channel] = key;
			}
		}
    }
	return join_map;
}

void Join::channelJoinResponse(Client &client, string channel_name)
{
	string name_list;
	Channel &channel = channel_map[channel_name];
	map<string, Client *> member_map = channel.getMemberMap();
	map<string, Client *> operator_map = channel.getOperatorMap();

	
	for (map<string, Client *>::iterator it = member_map.begin(); it != member_map.end(); it++) {
		string curr_client_name = it->first;
		if (operator_map.find(curr_client_name) != operator_map.end()) {
			name_list += '@ ';
		}
		name_list += curr_client_name;
		name_list += ' ';
	}
	
	client.setSendBuff(Reply::getCommonMsg(client, "Join ", channel_name));
	client.setSendBuff(Reply::getCodeMsg("353", client.getNickname(), "= " + channel_name + " :" + name_list));
	client.setSendBuff(Reply::getCodeMsg("366", client.getNickname(), " :End of /LINKS list"));
	// member_map에 있는 애들과 operator_map  에 있는 애들 비교해서 중복돼서...
}

void Join::execute(const Parser &parser, int fd)
{
	map<string, string> join_map = joinParse(parser.getParams()[0], parser.getParams()[1]);
	Client &client = client_map[fd];
	string client_name = client.getNickname();

	if (parser.getParams().size() == 0)
	{
		client.setSendBuff(Reply::getCodeMsg("461", client_name, " :Not enough parameters"));
		return ;
	}
	for (map<string, string>::iterator it_j = join_map.begin(); it_j != join_map.end(); it_j++) {
		string channel_name = it_j->first;
		string channel_key = it_j->second;
		if (channel_name.at(0) != '#') {
			client.setSendBuff(Reply::getCodeMsg("403",client_name, channel_name + " :No such channel"));
			continue;
		}
		map<string, Channel>::iterator it = channel_map.find(channel_name);
		if (it != channel_map.end()) {
			Channel &curr_channel = it->second;
			try {
				verificateKey(curr_channel, channel_key);
				verificateLimit(curr_channel);
				verificateInvite(curr_channel,fd);
			} catch(Join::JoinVerificateKeyException &e) {
				client.setSendBuff(Reply::getCodeMsg("475", client_name, channel_name + " :Cannot join channel (+k)"));
			} catch (Join::JoinVerificateLimitException &e) {
				client.setSendBuff(Reply::getCodeMsg("471", client_name, channel_name + " :Cannot join channel (+l)"));
			} catch (Join::JoinVerificateInviteException &e) {
				client.setSendBuff(Reply::getCodeMsg("473", client_name, channel_name + " :Cannot join channel (+i)"));
			}
			curr_channel.addMember(client_name, client);
			channelJoinResponse(client, channel_name);
			cout << client_name << "join in" << channel_name << endl;
			// 잘  들어왔음 응답 보내야함
		} else {
			channel_map[channel_name] = Channel(channel_name);
			Channel &curr_channel = channel_map[channel_name];
			curr_channel.addMember(client_name, client);
			channelJoinResponse(client, channel_name);
			cout << client_name << "join in" << channel_name << endl;
		}
	}
}