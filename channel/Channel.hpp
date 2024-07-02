#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include "../client/Client.hpp"

using namespace std;

enum {
	MODE_I,
	MODE_T,
	MODE_K,
	MODE_O,
	MODE_L
};

class Client;

class Channel
{
	private:
		std::map<string, Client> operator_map;
		std::map<string, Client> member_map;
		std::map<string, Client> inivite_map;
		string key;
		string topic;
		bool options[5];
	public:
		Channel() {};
		~Channel() {};
		Channel(Channel const &channel);
		Channel &operator=(Channel const &channel);
		void addOperator(string nick, Client &client);
		void deleteOperator(string nick, Client &client);
		void addMember(string nick, Client &client);
		void deleteMember(string nick, Client &client);
		void addInvite(string nick, Client &client);
		void deleteInvite(string nick, Client &client);
		const std::map<string, Client> &getOperatorMap() const;
		const std::map<string, Client> &getMemberMap() const;
		const std::map<string, Client> &getInviteMap() const;
		void setKey(string str);
		const string &getKey() const;
		void setTopic(string str);
		const string &getTopic() const;
		bool getModeOptionI() const;
		bool getModeOptionT() const;
		bool getModeOptionK() const;
		bool getModeOptionO() const;
		bool getModeOptionL() const;
		void setModeOptionI(bool flag);
		void setModeOptionT(bool flag);
		void setModeOptionK(bool flag);
		void setModeOptionO(bool flag);
		void setModeOptionL(bool flag);
};

#endif
