#ifndef PRIVMSG_HPP_
#define PRIVMSG_HPP_

#include "Command.hpp"
#include <set>

class Privmsg : public Command
{
	public:
		Privmsg(map<int, Client>& client_map, map<string, Channel>& channel_map)
		: Command(client_map, channel_map) {};
		~Privmsg() {};
		/*
			PRIVMSG 명령 실행
			- register 안된 경우 451
			- 파라미터 없는 경우 411
			- 파라미터 1개인 경우 412: 보낼 텍스트 없음
			- 1번째 파라미터는 ','로 구분되어 여러 target에게 전송 가능
			- 2번째 파라미터는 보낼 텍스트로 인식(3번째부터는 무시)
			- 모든 target에게 텍스트 전송
				- client인 경우 : send_buff에 텍스트 저장
				- channel인 경우 : sendToAllMembers() 호출
				- client/channel 못찾은 경우 401
				- target 중복된 경우 한번만 전송
		*/
		void execute(const Parser& parser, int client_fd) {
			vector<string> params = parser.getParams();
			set<string> targets;
			string ttbs;
			Client& sender = client_map[client_fd];

			if (!sender.getIsRegistered()) {
				sender.setSendBuff(Reply::getCodeMsg("451", sender.getNickname(), ":You have not registered"));
				return;
			}
			if (params.size() == 0) {
				sender.setSendBuff(Reply::getCodeMsg("411", sender.getNickname(), ":No recipient given (PRIVMSG)"));
				return;
			}
			if (params.size() == 1) {
				sender.setSendBuff(Reply::getCodeMsg("412", sender.getNickname(), ":No text to send"));
				return;
			}

			stringstream ss(params[0]);
			for (string tmp; getline(ss, tmp, ',');)
				targets.insert(tmp);
			ttbs = " :" + params[1];

			set<string>::iterator it;
			for (it=targets.begin(); it!=targets.end(); it++) {
				if ((*it)[0] == '#')
					sendToChannel(sender, it, ttbs);
				else
					sendToClient(sender, it, ttbs);
			}
		}
		void sendToClient(Client& sender, set<string>::iterator &it, string& ttbs) {
			try {
				Client& targetClient = client_map.find(Client::getSockFdByNick(*it))->second;
				targetClient.setSendBuff(Reply::getCommonMsg(sender, "PRIVMSG", sender.getNickname() + ttbs));
			} catch (Client::NoSuchNickException e) {
				sender.setSendBuff(Reply::getCodeMsg("401", sender.getNickname(), *it + " " + e.what()));
			} catch (...) {
				cout << "unknown exception ocuured\n";
			}
		}
		void sendToChannel(Client& sender, set<string>::iterator &it, string ttbs) {
			try {
				map<string, Channel>::iterator cit;
				cit = channel_map.find(*it);
				if (cit == channel_map.end())
					throw Client::NoSuchNickException();
				cit->second.sendToAllMembers(sender.getNickname(), Reply::getCommonMsg(sender, "PRIVMSG", cit->second.getChannelName() + ttbs));
			} catch (Client::NoSuchNickException& e) {
				sender.setSendBuff(Reply::getCodeMsg("401", sender.getNickname(), *it + " " + e.what()));
			} catch (...) {
				cout << "unknown exception ocuured\n";
			}
		}
};

#endif