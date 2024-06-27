#include "Client.hpp"

int main(int argc, char* argv[]) {
	Client client(3, "1234");
	
	try {
		client.setRecvBuff(string(argv[1]));
		cout << client.getSendBuff();
	} catch (exception& e) {
		cout << e.what() << endl;
	}
}