#include "action/Client.hpp"
#include "command/Command.hpp"
#include "command/Pass.hpp"
#include "parser/Parser.hpp"
#include "server/Server.hpp"
#include <vector>
#include <sstream>

// enum cmd_type {
// 	PASS, NICK, USER
// };

void init(map<int, Command*>& cmd_map, map<int, Client>& client_map, string& server_pwd) {
	cmd_map[PASS] = new Pass(client_map, server_pwd);
}

/*
	[TEST CODE]
	- set every elems in cmd_map
	- parse input
	- run command
*/
int main(int argc, char *argv[]) {
	if (argc != 3)
		exit(1);
	Server server = Server(atoi(argv[1]), argv[2]);
	server.run();

	// string server_pwd = "1234";
	// map<int, Client> client_map;
	// map<int, Command*> cmd_map;
	// map<int, Client>::iterator it;

	// // make mock client list
	// Client test1(3);
	// test1.setNickname("junkim2");
	// test1.setUsername("kim junho");
	// Client test2(4);
	// test2.setNickname("minjacho");
	// test2.setUsername("choi minjae");
	// client_map.insert(make_pair(3, test1));
	// client_map.insert(make_pair(4, test2));

	// // set every elems in cmd_map
	// init(cmd_map, client_map, server_pwd);
	// // parse input
	// string tmp(argv[1]);
	// cmd_map[PASS]->execute(Parser(tmp), 3);
	// cout << client_map[3].getNickname() << " : " << client_map[3].getSendBuff() << endl;

	// free
	// delete cmd_map[PASS];
}