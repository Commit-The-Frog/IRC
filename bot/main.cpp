#include <exception>
#include <iostream>
#include "Bot.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cout << "usage : ./bot <server ip> <server port> <server password>\n" << endl;
		return 1;
	}
	try {
		Bot bot = Bot(argv[1], argv[2], argv[3]);
		bot.run();
	} catch (exception &e) {
		cout << e.what() << endl;
	}
}
