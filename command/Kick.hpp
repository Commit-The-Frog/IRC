#ifndef KICK_HPP_
#define KICK_HPP_

#include "Command.hpp"

class Kick : public Command
{
	public:
		Kick(map<int, Client> &client_map, map<string, Channel> &channel_map)
		: Command(client_map, channel_map) {};
		~Kick() {};
		/*
			KICK 명령어 실행
			- 1번째 인자는 <channel>로 간주
			- 2번째 인자는 <user>로 간주
			- 마지막 인자는 <comment>로 간주
			- 예외처리
				- 파라미터가 2개 미만인 경우 461
				- 채널이 없는 경우 403
				- 채널 멤버가 1명 미만인 경우 403
				- 발신자가 채널의 멤버가 아닌 경우 442
				- 발신자가 채널의 오퍼레이터가 아닌 경우 482
				- 타겟 클라이언트의 닉네임이 없는 유저인 경우 401
				- 타겟 클라이언트가 채널에 없는 경우 441
		*/
		void execute(const Parser& parser, int client_fd) {
			vector<string> params = parser.getParams();
			set<string> targets;
			string comment;
			Client& sender = client_map[client_fd];

			if (params.size() < 2) {
				sender.setSendBuff(Reply::getCodeMsg("461", sender.getNickname(), ":Not enough parameters"));
				return ;
			}
			map<string, Channel>::iterator it;
			it = channel_map.find(params[0]);
			if (it == channel_map.end()) {
				sender.setSendBuff(Reply::getCodeMsg("403", sender.getNickname(), params[0] + " :No such channel"));
				return ;
			}
			Channel& targetChannel = it->second;
			if (targetChannel.getMemberMap().size() < 1) {
				sender.setSendBuff(Reply::getCodeMsg("403", sender.getNickname(), params[0] + " :No such channel"));
				return ;
			}
			if (!targetChannel.isMember(sender.getNickname())) {
				sender.setSendBuff(Reply::getCodeMsg("442", sender.getNickname(), params[1] + " :You're not on that channel"));
				return ;
			}
			if (!targetChannel.isOperator(sender.getNickname())) {
				sender.setSendBuff(Reply::getCodeMsg("482", sender.getNickname(), targetChannel.getChannelName() + " :You're not channel operator"));
				return ;
			}

			stringstream ss(params[1]);
			for (string tmp; getline(ss, tmp, ',');)
				targets.insert(tmp);
			comment = params[params.size()-1];

			set<string>::iterator sit;
			for (sit=targets.begin(); sit!=targets.end(); sit++) {
				kickClient(sender, targetChannel, sit, comment);
			}
			if (targetChannel.getMemberMap().size() < 1)
				channel_map.erase(params[0]);
		};

		void kickClient(Client& sender, Channel& targetChannel, set<string>::iterator& it, string& comment) {
			int target_fd;
			try {
				target_fd = Client::getSockFdByNick(*it);
			} catch (Client::NoSuchNickException& e) {
				sender.setSendBuff(Reply::getCodeMsg("401", sender.getNickname(), *it + " " + e.what()));
				return ;
			} catch (...) {
				cout << "unknown exception ocuured\n";
			}
			cout << target_fd << endl;

			Client &target = client_map[target_fd];

			if (!targetChannel.isMember(target.getNickname())) {
				sender.setSendBuff(Reply::getCodeMsg("441", sender.getNickname(), target.getNickname() + " " + targetChannel.getChannelName() + " :They aren't on that channel"));
				return ;
			}
			cout << "targetChannel : " << targetChannel.getChannelName() << endl;
			targetChannel.sendToAllMembers("", Reply::getCommonMsg(sender, "KICK", targetChannel.getChannelName() + " " + target.getNickname() + " :" + comment));

			targetChannel.deleteOperator(target.getNickname());
			targetChannel.deleteMember(target.getNickname(), target);
		};
};

#endif
