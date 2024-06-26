#include "client/Client.hpp"

int main(int argc, char* argv[]) {
	Client client(3, "1234");
	
	client.setRecvBuff(string(argv[1]));
}