#include "User.hpp"

User::User(std::map<int, Client> &client_map): Command(client_map)
{
}

const char *User::ClientNotFoundException::what() const throw() {
    return ("Client is Not Found!");
}

std::vector<string> User::parseParams(const string &params) {
    std::vector<string> result;
    string current;
    bool stop = false;
    for (int i = 0; i < params.size(); i++) {
        if ( params[i] == ':' ) {
            stop = true;
        }
        if (!stop && params[i] == ' ') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            } 
        }else {
                current += params[i];
        }
    }
    if (!current.empty())
        result.push_back(current);

    for (int i = 0; i < result.size(); i++)
    {
        cout <<"result " << result[i] << endl;
    }
    return result;
}

void User::execute(const Parser &parser, int fd)
{
    std::map<int, Client>::iterator iter = client_map.find(fd);

    //if (iter != client_list.end()) {
        std::vector<string> params = parseParams(parser.getParams());
        Client &curr_client  = client_map[fd];
        cout << "name" << client_map[fd].getUsername() << endl;
        for (int i = 0; i < params.size(); i++)
        {
            cout << params[i] << endl;
        }

        if (params.size() < 4 || params[0] == "") {
            std::stringstream ss;
            ss << curr_client.getNickname() << " " << "461" << " :Not enough parameters";
            curr_client.setSendBuff(" :Not enough parameters");
            cout << " :Not enough parameters" <<endl;
            return ;
            // 461에러   "<client> <command> :Not enough parameters";
        }
        if (curr_client.getUsername() == ""){
            cout << "set client username" << endl;
            curr_client.setUsername(params[0]);
            curr_client.setRealname(params[3]);
            cout << "client username " << curr_client.getUsername();
        } else {
            cout << " :You may not register" << endl;
            std::stringstream ss;
            ss << curr_client.getNickname() << " " << "462" << " :You may not register";
            client_map[fd].setSendBuff(ss.str());
            //  "<client> :You may not reregister (462) 에러"
        }
        //1. 현재 client가 이름을 가지고 있는지 확인
        //2. 가지고 있다면 에러 응답.
        //3. 가지고 있지 않다면 가지고 setName해주기
        //4. 두번째 params와 세번째 params 무시.
        //5. 네번째 params는 realname
        //6. 다섯번째 params 무시
    //}else {
     //   throw User::ClientNotFoundException();
        // 이건 진짜 에러이므로 throw 처리해야함.
   // }
    // parser에서 params 4개를 받아와야함!
}