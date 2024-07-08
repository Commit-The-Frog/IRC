#include "User.hpp"
const char *User::ClientNotFoundException::what() const throw() {
	return ("Client is Not Found!");
}

void User::execute(const Parser &parser, int client_fd)
{
		std::map<int, Client>::iterator it = client_map.find(client_fd);
		if (it == client_map.end()) {
			// client fd를 찾지 못한 경우
			throw User::ClientNotFoundException();
		}
		std::vector<string> params = parser.getParams();
		Client &curr_client  = client_map[client_fd];
		if (curr_client.getIsRegistered()){
			// irc 서버에 이미 등록된 경우 (username과 nickname이 있는 경우)
			curr_client.addSendBuff(Reply::getCodeMsg("462", curr_client.getNickname(), ":You may not reregister"));
		}
		else if (curr_client.getIsPassed() && curr_client.getUsername() == "" && (params.size() < 4 || params[0] == "")) {
			// username이 없고,  params의 숫자가 4보다 작은 경우.
			curr_client.addSendBuff(Reply::getCodeMsg("461", curr_client.getNickname(), ":Not enough parameters"));
		}
		else if (curr_client.getIsPassed() && curr_client.getUsername() == "") {
			// 정상적으로 등록할 수 있는 경우 (응답 없음)
			curr_client.setUsername(params[0]);
			curr_client.setRealname(params[3]);
			cout << "set client username " << curr_client.getUsername() << endl;
			if (curr_client.getNickname().length() > 0 && curr_client.getIsPassed()) {
				// 초기 설정이 완료된 경우
				curr_client.addSendBuff(Reply::getCodeMsg("001", curr_client.getNickname(), \
					":Welcome to the FT_IRC Network, " + curr_client.getNickname()));
				cout << "connection established" << endl;
			}
		}
		// 이 외에는 아무 동작도 하지 않음
}
