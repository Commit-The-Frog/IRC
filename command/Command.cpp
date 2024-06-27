#include "Command.hpp"

Command::Command(map<int, Client>& client_map, map<int, Channel>& channel_map)
:client_map(client_map), channel_map(channel_map)
{}