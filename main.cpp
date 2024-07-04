#include "./server/Server.hpp"
#include <exception>

using namespace std;
/*
	[TEST CODE]
	- set every elems in cmd_map
	- parse input
	- run command
*/
int main(int argc, char *argv[]) {
	if (argc != 3)
		exit(1);
	int port = atoi(argv[1]);
	if (port < 1024 || port > 49151) {
		cout << "Wrong port!" <<endl;
		exit(1);
	}
	Server server = Server(atoi(argv[1]), argv[2]);
	try {
		server.run();
	} catch (exception& e) {
		cout << e.what() << endl;
	}
}
