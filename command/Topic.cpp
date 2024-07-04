#include "Topic.hpp"

/*	[Topic::execute]
	- 만약에 parameter의 size가 1이면,
	해당 채널의 topic을 받아온다.
	topic이 설정되어있지 않으면, 331응답.
	topic이 설정되어있으면 332응답 이후 332응답.

	- 만약에 parameter의 size가 2이상이면,
	해당 채널의 topic을 설정한다.
	설정하는 토픽의 길이가 0이면 topic을 현재 설정된 토픽을 지운다.
*/
void	Topic::execute(const Parser& parser, int client_fd) {
	Client& client = client_map[client_fd];
	vector<string> params = parser.getParams();

	if (params.size() < 1) {
		client.setSendBuff(Reply::getCodeMsg("461", client.getNickname(), ":Not enough parameters"));
		return ;
	}
	map<string, Channel>::iterator it;
	if ((it = channel_map.find(params[0])) == channel_map.end()) {
		client.setSendBuff(Reply::getCodeMsg("403", client.getNickname(), params[0] + " :No such channel"));
		return ;
	}
	Channel& channel = it->second;
	if (!channel.isMember(client.getNickname())) {
		client.setSendBuff(Reply::getCodeMsg("442", client.getNickname(), params[0] + " :You're not on that channel"));
		return ;
	}
	if (params.size() == 1) { // topic 받기
		string topic = channel.getTopic();
		if (topic.length() < 1) {
			client.setSendBuff(Reply::getCodeMsg("331", client.getNickname(), params[0] + " :No topic is set"));
		} else {
			client.setSendBuff(Reply::getCodeMsg("332", client.getNickname(), params[0] + " :" + topic));
			client.setSendBuff(Reply::getCodeMsg("333", client.getNickname(), params[0] + " " + channel.getTopicWhoTime()));
		}
		// check topic
	} else { // topic 새로 설정
		if (channel.getModeOptionT() && !channel.isOperator(client.getNickname())) {
			client.setSendBuff(Reply::getCodeMsg("482", client.getNickname(), params[0] + " :You're not channel operator"));
			return ;
		}
		channel.setTopic(params[1], client);
		string msg = Reply::getCommonMsg(client, "TOPIC", params[0] + " :" + params[1]);
		channel.sendToAllMembers(msg, "");
		// topic setting
	}
}
