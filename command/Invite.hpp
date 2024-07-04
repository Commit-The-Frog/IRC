#ifndef INVITE_HPP_
#define INVITE_HPP_

#include "Command.hpp"

class Invite : public Command
{
	public:
		Invite(map<int, Client>& client_map, map<string, Channel>& channel_map)
		: Command(client_map, channel_map) {};
		~Invite() {};
		/*
			INVITE 명령어 실행
			- 파라미터가 2개가 아닌 경우 461
			- 채널이 없는 경우 403
			- 채널 멤버가 1명 미만인 경우 403
			- 발신자가 채널의 멤버가 아닌 경우 442
		*/
		void execute(const Parser& parser, int client_fd) {
			vector<string> params = parser.getParams();
			Client& sender = client_map[client_fd];

			if (params.size() != 2) {
				sender.setSendBuff(Reply::getCodeMsg("461", sender.getNickname(), ":Not enough parameters"));
				return ;
			}
			map<string, Channel>::iterator it;
			it = channel_map.find(params[1]);
			if (it == channel_map.end()) {
				sender.setSendBuff(Reply::getCodeMsg("403", sender.getNickname(), ":No such channel"));
				return ;
			}
			Channel& targetChannel = it->second;
			if (targetChannel.getMemberMap().size() < 1) {
				sender.setSendBuff(Reply::getCodeMsg("403", sender.getNickname(), ":No such channel"));
				return ;
			}
			if (!targetChannel.isMember(sender.getNickname())) {
				sender.setSendBuff(Reply::getCodeMsg("442", sender.getNickname(), ":You're not on that channel"));
				
			}

		};
};

#endif