#ifndef PASS_HPP_
#define PASS_HPP_

#include "Command.hpp"
#include "../parser/Parser.hpp"
#include "../reply/Reply.hpp"
#include "../client/Client.hpp"

class Pass : public Command {
	private:
		const string server_pwd;
	public:
		/* client_map, channel_map, server_pwd로 초기화 */
		Pass(map<int, Client>& client_map, map<string, Channel>& channel_map, const string& server_pwd)
		:Command(client_map, channel_map), server_pwd(server_pwd) {};
		~Pass() {};
		/*
			PASS 명령어 실행 :
			- pwd 검증
			- client_fd에 해당하는 사용자의 send_buff에 응답 저장
		*/
		void execute(const Parser& parser, int client_fd) {
			Client &curr_client = client_map[client_fd];
			if (curr_client.getIsRegistered())
				curr_client.setSendBuff(Reply("462", curr_client.getNickname(), ":You may not reregister"));
			else if (parser.getParams().size() == 0) {
				curr_client.setSendBuff(Reply("461", curr_client.getNickname(), ":Not enough parameters"));
			}
			else if (parser.getParams()[0] != this->server_pwd)
				curr_client.setSendBuff(Reply("464", curr_client.getNickname(), ":Password incorrect"));
			else if (parser.getParams()[0] == this->server_pwd) {
				client_map[client_fd].setIsPassedTrue();
			}
		};
};

#endif

