#include "Mode.hpp"

/*	[getModeString]
	str_v는 "+i", "+k <mykey>", "+l 10"
	과 같은 형식으로 이루어져있다.
	이를, "+ikl <mykey> 10" 과 같은 형식으로 바꿔주는
	함수이다.
*/
string Mode::getModeString(vector<string>& str_v)
{
	vector<string>	mode_compound;
	char			flag = 0;
	string			res;

	for (vector<string>::iterator it = str_v.begin(); it != str_v.end(); it++) {
		string cur_str = *it;
		string sub_str = "";
		size_t pos = cur_str.find(' ');
		if (pos != string::npos) {
			sub_str = cur_str.substr(pos + 1);
			cur_str = cur_str.substr(0, pos);
		}
		if (!flag) {
			flag = cur_str[0];
			mode_compound.push_back(cur_str);
		} else {
			if (flag == cur_str[0]) {
				mode_compound[0] += cur_str[1];
			} else {
				mode_compound[0] += cur_str;
				flag = cur_str[0];
			}
		}
		if (sub_str.length() > 0)
		{
			if (cur_str.compare("-k") == 0)
				mode_compound.push_back("*");
			else
				mode_compound.push_back(sub_str);
		}
	}
	for (vector<string>::iterator it = mode_compound.begin(); it != mode_compound.end(); it++) {
		if (it != mode_compound.begin())
			res += " ";
		res += *it;
	}
	return (res);
}

string Mode::getCurModeString(const Channel& channel) {
	vector<string> mode_vec;
	stringstream	ss;

	ss << "+l ";
	if (channel.getModeOptionI()) {
		mode_vec.push_back("+i");
	}
	if (channel.getModeOptionT())
		mode_vec.push_back("+t");
	if (channel.getModeOptionK())
		mode_vec.push_back("+k " + channel.getKey());
	if (channel.getModeOptionL()) {
		ss << channel.getLimit();
		mode_vec.push_back(ss.str());
	}
	string mode_str = getModeString(mode_vec);
	return (mode_str);
}

/*	[Mode::execute]
1.	채널이 존재하는지 확인한다.
2.	user가 채널의 operator인지 확인한다.
3.	파싱 이후에 적용.
	- vector로 modestring을 관리한다.
	- 이후 params과 modestring을 순회하면서 modestring 뒤에 mode param을 붙인다.
	- 만약 params가 남는다면, 남는 param은 무시한다.

4.	파싱이 된 vector들을 순회하면서 적용시킨다. 적용이 안된 mode는 vector에서 삭제한다.
5.	이후 privmsg로 적용된 모드를 설명해주기 위해서, 파싱이 된 vector들을 역순으로 합친다.
	만약에 mode param이 있는 경우 뒤에다 해당 param을 붙여주는데, -k 이면 *을 붙인다.
*/
void Mode::execute(const Parser& parser, int client_fd)
{
	Client& client = client_map[client_fd];
	vector<string> mode_vec;
	vector<string> changed_mode_vec;
	vector<string> params = parser.getParams();

	if (!client.getIsRegistered()) {
		client.setSendBuff(Reply::getCodeMsg("451", client.getNickname(), ":You have not registered"));
		return;
	}
	if (params.size() < 1) { // 461 MODE :Not enough parameters
		client.setSendBuff(Reply::getCodeMsg("461", client.getNickname(), ":Not enough parameters"));
		return ;
	}
	map<string, Channel>::iterator it;
	if ((it = channel_map.find(params[0])) == channel_map.end()) {
		client.setSendBuff(Reply::getCodeMsg("403", client.getNickname(), params[0] + " :No such channel"));
		return ;
		// ERR_NOSUCHCHANNEL (403)
	}
	Channel& channel = it->second;
	if (params.size() == 1) { // RPL_CHANNELMODEIS (324)
		string cur_mode_str = getCurModeString(channel);
		client.setSendBuff(Reply::getCodeMsg("324", client.getNickname(), params[0] + " " + cur_mode_str));
		return ;
	} else { // Mode Setting
		char	flag = 0;
		if (!channel.isOperator(client.getNickname())) {
			client.setSendBuff(Reply::getCodeMsg("482", client.getNickname(), params[0] + " :You're not channel operator"));
			return ;
		}
		for (int i = 0; i < params[1].length(); i++) {
			if (params[1][i] == '+' || params[1][i] == '-') {
				flag = params[1][i];
				continue;
			}
			if (params[1][i] == 'i' || params[1][i] == 't' || params[1][i] == 'k' || params[1][i] == 'o' || params[1][i] == 'l') {
				string	mode_vec_element = "";
				mode_vec_element += flag;
				mode_vec_element += params[1][i];
				mode_vec.push_back(mode_vec_element);
			} else { // ERR_UNKNOWNMODE (472) || ERR_UMODEUNKNOWNFLAG (501)
				stringstream ss;
				ss << params[1][i];
				ss << " :is unknown mode char to me";
				client.setSendBuff(Reply::getCodeMsg("472", client.getNickname(), ss.str()));
				return ;
			}
		}
		vector<string>::iterator mode_it = mode_vec.begin();
		vector<string>::iterator param_it = params.begin();
		param_it += 2;
		while (mode_it != mode_vec.end()) {
			if ((*mode_it).compare("+k") == 0 || (*mode_it).compare("-k") == 0 || (*mode_it).compare("+o") == 0 || (*mode_it).compare("-o") == 0 || (*mode_it).compare("+l") == 0) {
				if (param_it == params.end())
					return ;
				(*mode_it) += (" " + *param_it);
				param_it++;
			}
			mode_it++;
		}
	}
	for (vector<string>::iterator it = mode_vec.begin(); it != mode_vec.end(); it++) {
		string cur_str = *it;
		string sub_str = "";
		size_t pos = cur_str.find(' ');
		if (pos != string::npos) {
			sub_str = cur_str.substr(pos + 1);
			cur_str = cur_str.substr(0, pos);
		}
		switch (cur_str[1]) {
			case 'i':
				if (cur_str[0] == '+') {
					if (!channel.getModeOptionI()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(true);
					}
				} else {
					if (channel.getModeOptionI()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(false);
					}
				}
				break;
			case 't':
				if (cur_str[0] == '+') {
					if (!channel.getModeOptionT()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionT(true);
					}
				} else {
					if (channel.getModeOptionT()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionT(false);
					}
				}
				break;
			case 'k':
				if (cur_str[0] == '+') {
					if (!channel.getModeOptionK()) {
						channel.setModeOptionK(true);
					}
					changed_mode_vec.push_back(*it);
					channel.setKey(sub_str);
				} else {
					if (channel.getModeOptionK()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionK(false);
						channel.setKey("");
					}
				}
				break;
			case 'o':
				int client_fd;
				try {
					client_fd = Client::getSockFdByNick(sub_str);
				} catch (Client::NoSuchNickException &e) {
					client.setSendBuff(Reply::getCodeMsg("401", client.getNickname(), sub_str + " :No such nick/channel"));
					continue;
				}
				if (cur_str[0] == '+') {
					Client& target_client = client_map[client_fd];
					if (!channel.isMember(sub_str)) {
						client.setSendBuff(Reply::getCodeMsg("441", client.getNickname(), \
							sub_str + " " + params[0] + " :They aren't on that channel"));
					} else if (!channel.isOperator(sub_str)) {
						changed_mode_vec.push_back(*it);
						channel.addOperator(sub_str, target_client);
					}
				} else {
					if (!channel.isMember(sub_str)) {
						client.setSendBuff(Reply::getCodeMsg("441", client.getNickname(), \
							sub_str + " " + params[0] + " :They aren't on that channel"));
					} else if (channel.isOperator(sub_str)) {
						changed_mode_vec.push_back(*it);
						channel.deleteOperator(sub_str);
					}
				}
				break;
			case 'l':
				if (cur_str[0] == '+') {
					channel.setModeOptionL(true);
					int	limit;
					stringstream ss(sub_str);
					ss >> limit;
					if (ss.fail())
						continue;
					channel.setLimit(limit);
					changed_mode_vec.push_back(*it);
				} else {
					if (channel.getModeOptionL())
					{
						channel.setModeOptionL(false);
						changed_mode_vec.push_back(*it);
						channel.setLimit(-1);
					}
				}
				break;
			default:
				break;
		}
	}
	if (changed_mode_vec.size() > 0)
		client.setSendBuff(Reply::getCommonMsg(client, "MODE", params[0] + " " + getModeString(changed_mode_vec)));
}

