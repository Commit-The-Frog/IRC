#include "Client.hpp"

Client::Client(int client_fd, string server_pwd) : client_fd(client_fd), server_pwd(server_pwd) {}
Client::~Client() {}

/* 
	``` Client객체의 recv_buff에 data 저장 ```
	- data에 따른 명령어 파싱
	- 파싱된 명령어 실헹
*/
void Client::setRecvBuff(const string& data) {
	size_t idx;

	// PASS <password>
	idx = data.find("PASS");
	if (idx != 0)
		throw invalid_argument("ERROR");
	cout << idx << endl;
	cout << data.substr(idx) << endl;
	// while (idx < data.size()-1) {
		
	// 	idx++;
	// }
	
}
