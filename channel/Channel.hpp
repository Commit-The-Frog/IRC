#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include "../action/Client.hpp"

enum {
	MODE_I,
	MODE_T,
	MODE_K,
	MODE_O,
	MODE_L
};

class Channel
{
	private:
		std::map<int, Client> operator_map;
		std::map<int, Client> member_map;
		std::map<int, Client> inivite_map;
		std::string key;
		std::string topic;
		bool options[5];
	public:
		Channel() {};
		~Channel() {};
		Channel(Channel const &channel);
		Channel &operator=(Channel const &channel);
		void addOperatorMap(int fd , Client &client);
		void deleteOperatorMap(int fd , Client &client);
		void addMemberMap(int fd , Client &client);
		void deleteMemberMap(int fd , Client &client);
		void addInviteMap(int fd , Client &client);
		void deleteInviteMap(int fd , Client &client);
		std::map<int, Client> &getOperatorMap();
		std::map<int, Client> &getMemberMap();
		std::map<int, Client> &getInviteMap();
		void setKey(std::string str);
		std::string &getKey();
		void setTopic(std::string str);
		std::string &getTopic();
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