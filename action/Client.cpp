#include "Client.hpp"
#include "Command.hpp"

Client::Client(int client_fd, string server_pwd) : client_fd(client_fd), server_pwd(server_pwd) {}
Client::~Client() {}

/* 
	``` Client객체의 recv_buff에 data 저장 ```
	- recv_buff에 data저장
	- crlf까지 추출
	- 추출된 명령어 파싱
	- 파싱된 명령어 실헹
	- 전송이 필요한 경우 send_buff에 저장
*/
void Client::setRecvBuff(const string& data) {
	size_t idx;
	size_t clrf_idx = 0;
	string substr_data;

	// CRLF 추출
	recv_buff.append(data);
	while (1) {
		clrf_idx = recv_buff.find("\r\n");
		if (clrf_idx == string::npos)
			return;
		substr_data = recv_buff.substr(0, 7);
		recv_buff = recv_buff.substr(7);
		// cout << substr_data << endl;
		// 파싱 후 Command객체에 저장 (미구현)

		// 명령어 실행 (미구현)

		// send_buff에 저장
		send_buff.append(substr_data);
	}
}

string Client::getSendBuff() const {
	return send_buff;
}

void Client::clearSendBuff() {
	send_buff.clear();
}
