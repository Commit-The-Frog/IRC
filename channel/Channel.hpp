#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include "../client/Client.hpp"

using namespace std;

enum {
	MODE_I,
	MODE_T,
	MODE_K,
	MODE_L
};

class Client;

class Channel
{
	private:
		string channel_name;
		std::map<string, Client> operator_map;
		std::map<string, Client> member_map;
		std::map<string, Client> inivite_map;
		string key;
		string topic;
		int limit;
		bool options[4];
	public:
		Channel() {};
		Channel(const Channel &channel);
		~Channel();
		Channel &operator=(const Channel &channel);
		void setChannelName(const string &channel_name);
		const string &getChannelName() const;
		void addOperator(const string &nick, const Client &client);
		void deleteOperator(const string &nick);
		void addMember(const string &nick, const Client &clientt);
		void deleteMember(const string &nick);
		void addInvite(const string &nick, const Client &client);
		void deleteInvite(const string &nick);
		const std::map<string, Client> &getOperatorMap() const;
		const std::map<string, Client> &getMemberMap() const;
		const std::map<string, Client> &getInviteMap() const;
		bool isOperator(const string &nick);
		bool isMember(const string &nick);
		bool isInvited(const string &nick);
		void setKey(const string &str);
		const string &getKey() const;
		void setTopic(const string &str);
		const string &getTopic() const;
		void setLimit(int limit);
		int getLimit() const;
		bool getModeOptionI() const;
		bool getModeOptionT() const;
		bool getModeOptionK() const;
		bool getModeOptionL() const;
		void setModeOptionI(bool flag);
		void setModeOptionT(bool flag);
		void setModeOptionK(bool flag);
		void setModeOptionL(bool flag);
		void sendAllClients(const string &msg);
};

#endif
