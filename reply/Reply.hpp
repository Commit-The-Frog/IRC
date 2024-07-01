#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>
# include <sstream>

using namespace std;

class Reply {
	private:
		string	_code;
		string	_client_nick;
		string	_msg;
	public:
		Reply(const string& code, const string& client_nick, const string& msg):
			_code(code), _client_nick(client_nick), _msg(msg) {};
		string getString(void) const {
			stringstream ss;
			ss <<
		}
};

#endif
