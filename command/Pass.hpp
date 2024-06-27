#ifndef PASS_HPP_
#define PASS_HPP_

#include "Command.hpp"

class Pass : public Command
{
	private:
		const string server_pwd;
	public:
		/* client_map, channel_map, server_pwd로 초기화 */
		Pass(map<int, Client>& client_map, map<int, Channel>& channel_map, const string server_pwd)
		:Command(client_map, channel_map), server_pwd(server_pwd) {};
		/* 
			PASS 명령어 실행 :
			- pwd 검증
			- client_fd에 해당하는 사용자의 send_buff에 응답 저장 
		*/
		void execute(const Parser& parser, int client_fd) {};
};

#endif

