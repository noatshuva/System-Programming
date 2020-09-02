#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "Watchable.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
	Session(const Session &other);
	Session &operator=(const Session &other);
	Session &operator=(Session&& other);
	Session(Session	&&other);
	void start();
	std::vector<Watchable*> getContent();
	User* getActiveUser();
	std::vector<BaseAction*> getActionLog();
	void setExit();
	std::unordered_map<std::string,User*>& getUserMap();
	void setUserMap(std::string, User*);
	std::vector<std::string> split(std::string str);
	void setActiveUser(User* name);
	void delUser(std::string name);
	std::vector<std::string> getCommand();
	void setCommand(std::vector<std::string> cmd);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    std::vector<std::string> command;
    User* activeUser;
    bool exit;
};
#endif
