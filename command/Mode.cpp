#include "Mode.hpp"

string Mode::getModeString(vector<string>& str_v)
{
	vector<string>	mode_compound;
	char			flag = 0;
	string			res;

	for (vector<string>::iterator it = str_v.begin(); it == str_v.end(); it++) {
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
	for (vector<string>::iterator it = mode_compound.begin(); it == mode_compound.end(); it++) {
		if (it != mode_compound.begin())
			res += " ";
		res += *it;
	}
	return (res);
}

void Mode::execute(const Parser& parser, int client_fd)
{
	Client& client = client_map[client_fd];

	// 1.	채널이 존재하는지 확인한다.
	// 2.	user가 채널의 operator인지 확인한다.
	// 3.	파싱 이후에 적용.
	//			1. vector로 modestring을 관리한다.
	//			2. 이후 params과 modestring을 순회하면서 modestring 뒤에 mode param을 붙인다.
	//			3. 만약 params가 남는다면, 남는 param은 무시한다.
	// 4.	파싱이 된 vector들을 순회하면서 적용시킨다. 적용이 안된 mode는 vector에서 삭제한다.
	// 5.	이후 privmsg로 적용된 모드를 설명해주기 위해서, 파싱이 된 vector들을 역순으로 합친다.
	//		만약에 mode param이 있는 경우 뒤에다 해당 param을 붙여주는데, -k 이면 *을 붙인다.
	vector<string> mode_vec;
	vector<string> changed_mode_vec;
	vector<string> params = parser.getParams();
	if (params.size() < 2) {
		// 461 MODE :Not enough parameters
	}
	map<string, Channel>::iterator it;
	if ((it = channel_map.find(params[0])) == channel_map.end()) {
		// ERR_NOSUCHCHANNEL (403)
	}
	Channel& channel = it->second;
	if (params.size() == 2) {
		if (channel.getModeOptionI())
			mode_vec.push_back("+i");
		if (channel.getModeOptionT())
			mode_vec.push_back("+t");
		if (channel.getModeOptionK()) {
			mode_vec.push_back("+k " + channel.getKey());
		}
		if (channel.getModeOptionL()) {
			mode_vec.push_back("+l " + channel.getLimit());
		}
		client.setSendBuff(Reply::getCodeMsg("324", client.getNickname(), params[0] + " " + getModeString(mode_vec)));
		// mode 정보를 전달해줍니다. +인 애들만 합쳐서 전달.
	} else {
		char	flag = 0;
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
			} else {
				// 없는 설정값임을 알려줘야된다.
				break;
			}
		}
		vector<string>::iterator mode_it = mode_vec.begin();
		vector<string>::iterator param_it = params.begin();
		param_it += 2;
		while (mode_it != mode_vec.end()) {
			if ((*mode_it).compare("+k") == 0 || (*mode_it).compare("-k") == 0 || (*mode_it).compare("+o") == 0 || (*mode_it).compare("-o") == 0 || (*mode_it).compare("+l") == 0) {
				if (param_it == params.end()) {
					break;
					//파라미터 부족한 에러 발생.
				}
				(*mode_it) += (" " + *param_it);
				param_it++;
			}
			it++;
		}
	}
	for (vector<string>::iterator it = mode_vec.begin(); it == mode_vec.end(); it++) {
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
			case 't':
				if (cur_str[0] == '+') {
					if (!channel.getModeOptionT()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(true);
					}
				} else {
					if (channel.getModeOptionT()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(false);
					}
				}
			case 'k':
				if (cur_str[0] == '+') {
					if (!channel.getModeOptionK()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(true);
						channel.setKey(sub_str);
					}
				} else {
					if (channel.getModeOptionK()) {
						changed_mode_vec.push_back(*it);
						channel.setModeOptionI(false);
						channel.setKey("");
					}
				}
			case 'o':
				if (cur_str[0] == '+') {
						if (!channel.isMember(sub_str))
							// 채널의 맴버가 아니라는 에러
						else if (!channel.isOperator(sub_str)) {
							changed_mode_vec.push_back(*it);
							// 채널의 관리자로 추가.
						}
				} else {
						if (!channel.isMember(sub_str))
							// 채널의 맴버가 아니라는 에러
						else if (channel.isOperator(sub_str)) {
							changed_mode_vec.push_back(*it);
							// 채널의 관리자에서 제거.
						}
				}
			case 'l':
				if (cur_str[0] == '+') {
					channel.setModeOptionL(true);
					int	limit;
					stringstream ss(sub_str);
					ss >> limit;
					if (ss.fail())
						continue;// 이상한 값 들어왔음. 명령어 실행 x
					channel.setLimit(limit);
				} else {
					channel.setModeOptionL(false);
				}
			default:
		}
	}
}

