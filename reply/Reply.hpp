#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>
# include <sstream>
# include "../client/Client.hpp"

using namespace std;

class Reply {
	public:
		static string getCodeMsg(const string& code, const string& client_nick, const string& msg) {
			stringstream ss;
			string tempnick = client_nick.length() == 0 ? "*" : client_nick;
			ss << ":ircserv" << " " << code << " " << tempnick << " " << msg << "\r\n";
			return ss.str();
		}
		static string getCommonMsg(const Client& client, const string& command, const string& msg) {
			string prefix = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getIpAddr();
			stringstream ss;
			ss << prefix << " " << command << " " << msg << "\r\n";
			return ss.str();
		}
};

#endif
