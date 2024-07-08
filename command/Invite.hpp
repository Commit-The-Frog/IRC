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
			- 예외처리
				- 파라미터가 2개가 아닌 경우 461
				- 채널이 없는 경우 403
				- 채널 멤버가 1명 미만인 경우 403
				- 발신자가 채널의 멤버가 아닌 경우 442
				- 발신자가 채널의 오퍼레이터가 아닌 경우 482
				- 타겟 클라이언트의 닉네임이 존재하지 않는 경우 401
				- 타겟 클라이언트가 이미 타겟 채널에 속한 경우 443
			- 타겟 채널의 초대 목록에 타겟 클라이언트 추가
			- 타겟 클라이언트의 초대 목록에 타겟 채널 추가
			- 발신자에게 RPL_INVITING 응답
			- 타겟 유저에게 INVITE 메세지 응답
		*/
		void execute(const Parser& parser, int client_fd) {
			vector<string> params = parser.getParams();
			Client& sender = client_map[client_fd];

			if (params.size() != 2) {
				sender.addSendBuff(Reply::getCodeMsg("461", sender.getNickname(), ":Not enough parameters"));
				return ;
			}
			map<string, Channel>::iterator it;
			it = channel_map.find(params[1]);
			if (it == channel_map.end()) {
				sender.addSendBuff(Reply::getCodeMsg("403", sender.getNickname(), params[1] + " :No such channel"));
				return ;
			}
			Channel& targetChannel = it->second;
			if (targetChannel.getMemberMap().size() < 1) {
				sender.addSendBuff(Reply::getCodeMsg("403", sender.getNickname(), params[1] + " :No such channel"));
				return ;
			}
			if (!targetChannel.isMember(sender.getNickname())) {
				sender.addSendBuff(Reply::getCodeMsg("442", sender.getNickname(), params[1] + " :You're not on that channel"));
				return ;
			}
			if (!targetChannel.isOperator(sender.getNickname())) {
				sender.addSendBuff(Reply::getCodeMsg("482", sender.getNickname(), targetChannel.getChannelName() + " :You're not channel operator"));
				return ;
			}

			int receiver_fd;
			try {
				receiver_fd = Client::getSockFdByNick(params[0]);
			} catch (const Client::NoSuchNickException& e) {
				sender.addSendBuff(Reply::getCodeMsg("401", sender.getNickname(), params[0] + " :No such nick/channel"));
				return ;
			}
			Client& receiver = client_map[receiver_fd];
			if (targetChannel.isMember(receiver.getNickname())) {
				sender.addSendBuff(Reply::getCodeMsg("443", sender.getNickname(), receiver.getNickname() + " " + targetChannel.getChannelName() + " :is already on channel"));
				return ;
			}

			targetChannel.addInvite(receiver.getNickname(), receiver);
			sender.addSendBuff(Reply::getCodeMsg("341", sender.getNickname(), receiver.getNickname() + " " + targetChannel.getChannelName()));
			receiver.addSendBuff(Reply::getCommonMsg(sender, "INVITE", receiver.getNickname() + " " + targetChannel.getChannelName()));
		};
};

#endif
