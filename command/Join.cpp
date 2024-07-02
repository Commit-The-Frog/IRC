#include "Join.hpp"

void Join::verificateKey(Channel const &channel) {
	if (channel.getModeOptionK())  {

	}
}

void Join::verificateInvite(Channel const &channel) {
	if (channel.getModeOptionI()) {

	}
}

void Join::verificateLimit(Channel const &channel) {
	if (channel.getModeOptionL()) {

	}
}

void Join::execute(const Parser &parser, int fd)
{
	map<string, Channel>::iterator it = channel_map.find(key);
	if (it != channel_map.end()) {
		Channel &curr_channel = it->second;
		if (curr_channel.getModeOptionK()) {
			// key 가 맞는지 확인
		}
		if (curr_channel.getModeOptionI()) {
			// invited 리스트에 있는지 확인
		}
		if (curr_channel.getModeOptionL()) {
			// limit을 넘었는지 확인
		}
	}else {
		channel_map[key] = Channel();
	}
}