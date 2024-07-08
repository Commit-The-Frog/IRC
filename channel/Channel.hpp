#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include <set>
#include <sstream>
#include <ctime>
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
		std::map<string, Client *> member_map;
		set<string> operator_set;
		std::map<string, Client *> invite_map;
		string key;
		string topic;
		string latest_topic_set_user;
		string latest_topic_set_time;
		int limit;
		bool options[4];
	public:
		Channel() {};
		Channel(const string &str);
		Channel(const Channel &channel);
		~Channel();
		Channel &operator=(const Channel &channel);
		void setChannelName(const string &channel_name);
		const string &getChannelName() const;
		void addOperator(const string &nick);
		void deleteOperator(const string &nick);
		void addMember(const string &nick, Client &clientt);
		void deleteMember(const string &nick, Client &client);
		void addInvite(const string &nick, Client& client);
		void deleteInvite(const string &nick, Client& client);
		void initial();
		void changeMemberName(const string &prevnick, const string &changenick);
		const std::map<string, Client*> &getMemberMap() const;
		bool isOperator(const string &nick) const;
		bool isMember(const string &nick) const;
		bool isInvited(const string &nick) const;
		void setKey(const string &str);
		const string &getKey() const;
		void setTopic(const string &str, const Client& client);
		const string &getTopic() const;
		string getTopicWhoTime() const;
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
		void sendToAllMembers(const string &sender, const string &msg);
};

#endif
