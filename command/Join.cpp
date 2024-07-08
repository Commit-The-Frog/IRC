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
			cout << "channel key : " << channel.getKey() << endl;
			cout << "input key : " << str << endl;
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
		if (static_cast<size_t>(channel.getLimit()) <= channel.getMemberMap().size())
			throw Join::JoinVerificateLimitException();
	}
}

map<string,string> Join::joinParse(const string &channels, const string &keys) {
	std::stringstream channels_(channels);
	std::stringstream keys_(keys);
    std::string channel;
	std::string key;
	map<string, string> join_map;

    while (std::getline(channels_, channel, ',')) {
		if (join_map.find(channel) == join_map.end()) {
			if (std::getline(keys_, key, ',')) {
				join_map[channel] = key;
			} else {
				join_map[channel] = "";
			}
		}
    }
	return join_map;
}

void Join::channelJoinResponse(Client &client, string channel_name)
{
	// 채널에 존재하는 모든 멤버에 대한 닉네임을 name_list 에 만든후 join 한 클라이언트에 대해 응답
	string name_list;
	Channel &channel = channel_map[channel_name];
	map<string, Client *> member_map = channel.getMemberMap();

	for (map<string, Client *>::iterator it = member_map.begin(); it != member_map.end(); it++) {
		string curr_client_name = it->first;
		if (channel.isOperator(curr_client_name)) {
			name_list += "@";
		}
		name_list += curr_client_name;
		name_list += " ";
	}
	client.addSendBuff(Reply::getCommonMsg(client, "Join ", channel_name));
	if (channel.getTopic() != "") {
		// topic 이 있을경우 토픽에 대한 응답
		client.addSendBuff(Reply::getCodeMsg("332", client.getNickname(), " " + channel_name + " :" + channel.getTopic()));
		client.addSendBuff(Reply::getCodeMsg("333", client.getNickname(), " " + channel_name + " :" + channel.getTopicWhoTime()));
	}
	client.addSendBuff(Reply::getCodeMsg("353", client.getNickname(), "= " + channel_name + " :" + name_list));
	client.addSendBuff(Reply::getCodeMsg("366", client.getNickname(), channel_name + " :End of /NAMES list"));
}

void Join::execute(const Parser &parser, int fd)
{
	Client &client = client_map[fd];
	string client_name = client.getNickname();

	if (!client.getIsRegistered()) {// registered 안되어있을 경우
		client.addSendBuff(Reply::getCodeMsg("451", client.getNickname(), ":You have not registered"));
		return;
	}
	if (parser.getParams().size() == 0) // params 없을 경우
	{
		client.addSendBuff(Reply::getCodeMsg("461", client_name, " :Not enough parameters"));
		return ;
	}
	map<string, string> join_map = joinParse(parser.getParams()[0], parser.getParams().size() < 2 ? "" : parser.getParams()[1]); // params 파싱해서 채널네임과 키 짝지어줌
	for (map<string, string>::iterator it_j = join_map.begin(); it_j != join_map.end(); it_j++) {
		string channel_name = it_j->first;
		string channel_key = it_j->second;
		if (channel_name.at(0) != '#') { // 유효한 채널 네임인지 확인 (# 붙어있어야함)
			client.addSendBuff(Reply::getCodeMsg("403",client_name, channel_name + " :No such channel"));
			continue;
		}
		map<string, Channel>::iterator it = channel_map.find(channel_name);
		if (it != channel_map.end()) { // 이미 존재하는 채널일 경우
			Channel &curr_channel = it->second;
			if (curr_channel.isMember(client_name)) { // 이미 해당 채널의 멤버이면 아무 동작도 안함
				continue;
			}
			try {
				if (curr_channel.getMemberMap().size() == 0) {
					curr_channel.initial();
					curr_channel.addOperator(client_name);
				} else {
					verificateKey(curr_channel, channel_key); // key 검증
					verificateLimit(curr_channel); // limit 검증
					verificateInvite(curr_channel,fd);  // invite 검증
				}
				curr_channel.addMember(client_name, client); // 채널에 추가
				channelJoinResponse(client, channel_name); // 조인 응답
				curr_channel.sendToAllMembers(client_name, Reply::getCommonMsg(client, "JOIN", channel_name)); // 채널에 속한 모든 멤버에게 해당 클라이언트가 입장햇음을 알림
				cout << client_name << "join in" << channel_name << endl;
			} catch(Join::JoinVerificateKeyException &e) {
				client.addSendBuff(Reply::getCodeMsg("475", client_name, channel_name + " :Cannot join channel (+k)"));
			} catch (Join::JoinVerificateLimitException &e) {
				client.addSendBuff(Reply::getCodeMsg("471", client_name, channel_name + " :Cannot join channel (+l)"));
			} catch (Join::JoinVerificateInviteException &e) {
				client.addSendBuff(Reply::getCodeMsg("473", client_name, channel_name + " :Cannot join channel (+i)"));
			}
		} else { // 새로 만든 채널일 경우
			channel_map[channel_name] = Channel(channel_name); // 채널 생성후 채널리스트에 추가
			Channel &curr_channel = channel_map[channel_name];
			curr_channel.addMember(client_name, client); // 클라이언트 채널에 추가
			curr_channel.addOperator(client_name); // 클라이언트 오퍼레이터로 추가
			channelJoinResponse(client, channel_name); // 조인 응답
			cout << client_name << "join in" << channel_name << endl;
		}
	}
}
