#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/User.h"
#include "../include/Watchable.h"
#include <vector>
#include <string>
#include "../include/Session.h"
#include "../include/Action.h"
#include <sstream>


using json = nlohmann::json;
using namespace std;


Session::Session(const std::string &configFilePath)
		: content(), actionsLog(), userMap(), command(), activeUser(), exit(false) {
	User *def = new LengthRecommenderUser("default");
	userMap["default"] = def;
	this->activeUser = def;
	// read a JSON file
	std::ifstream i(configFilePath);
	json j;
	i >> j;

	int id = 1;

	for (auto &element : j["movies"].items()) {

		string name = element.value()["name"];
		int length = element.value()["length"];
		vector<string> tags;

		for (auto tag: element.value()["tags"].items()) {
			tags.push_back(tag.value());
		}
		content.push_back(new Movie(id, name, length, tags));
		id++;
	}

	int id2 = j["movies"].size() + 1;

	for (auto &element : j["tv_series"].items()) {

		string name = element.value()["name"];
		int episode_length = element.value()["episode_length"];
		vector<string> tags;

		for (auto tag: element.value()["tags"].items()) {
			tags.push_back(tag.value());
		}

		int seasonc = 1;
		for (auto season: element.value()["seasons"].items()) {
			int epis = season.value();
			for (int i = 1; i <= epis; i++) {
				Episode *episode = new Episode(id2, name, episode_length, seasonc, i, tags);
				content.push_back(episode);
				id2++;
			}
			seasonc++;
		}
	}
};

User *Session::getActiveUser() {
	return activeUser;
}

std::vector<Watchable *> Session::getContent() {
	return content;
}

void Session::setActiveUser(User *name) {
	activeUser = name;
}

void Session::delUser(std::string name) {
	delete(userMap[name]);
	userMap.erase(name);
}

void Session::setExit() {
	exit = true;
}

void Session::start() {
	cout << "SPLFLIX is now on!" << endl;
	string str;
	while (!exit) {
		getline(cin, str);
		command = split(str);
		BaseAction *x;
		if (command[0] == "createuser")
			x = new CreateUser();
		else if (command[0] == "changeuser")
			x = new ChangeActiveUser();
		else if (command[0] == "deleteuser")
			x = new DeleteUser();
		else if (command[0] == "dupuser")
			x = new DuplicateUser();
		else if (str == "content")
			x = new PrintContentList();
		else if (str == "watchhist")
			x = new PrintWatchHistory();
		else if (command[0] == "watch")
			x = new Watch();
		else if (str == "log")
			x = new PrintActionsLog();
		else if (str == "exit")
			x = new Exit();
		else
			continue;
		x->act(*this);
		actionsLog.push_back(x);
	}
	exit = false;
};

vector<string> Session::split(std::string str) {
	vector<string> vec;
	string word;
	for (auto x : str) {
		if (x == ' ') {
			vec.push_back(word);
			word = "";
		} else
			word += x;
	}
	vec.push_back(word);
	return vec;
}

std::unordered_map<std::string, User *> &Session::getUserMap() {
	return userMap;
}

void Session::setUserMap(string name, User *newUser) {
	userMap[name] = newUser;
}

std::vector<BaseAction *> Session::getActionLog() {
	return actionsLog;
}

//destructor
Session::~Session() {
	for (BaseAction *action:actionsLog)
		delete action;
	actionsLog.clear();
	for (Watchable *cont:content)
		delete cont;
	content.clear();
	for (auto pu: userMap)
		delete (pu.second);
	userMap.clear();
};

//copy constructor
Session::Session(const Session &other)
		: content(), actionsLog(), userMap(),command(other.command), activeUser(other.activeUser), exit(other.exit) {
	for (Watchable *cont : other.content)
		content.push_back(cont->clone());
	for (BaseAction *log : other.actionsLog)
		actionsLog.push_back(log->clone());
	for (auto user: other.userMap)
		this->userMap.insert({user.first, user.second->clone()});

	activeUser = userMap[other.activeUser->getName()];
}

//move constructor
Session::Session(Session &&other) : content(move(other.content)), actionsLog(move(other.actionsLog)),
									userMap(move(other.userMap)), command(other.command), activeUser(other.activeUser), exit(other.exit) {}

//copy assignment operator
Session &Session::operator=(const Session &other) {
	if (this != &other) {
		for (auto cont:content) {
			delete cont;
		}
		content.clear();
		for (BaseAction *log:actionsLog) {
			delete log;
		}
		actionsLog.clear();
		for (auto p:userMap) {
			delete p.second;
		}
		userMap.clear();

		this->exit = other.exit;
		for (Watchable *cont:other.content) {
			this->content.push_back(cont->clone());
		}
		for (BaseAction *log:other.actionsLog) {
			this->actionsLog.push_back(log->clone());
		}
		for (auto user: other.userMap) {
			this->userMap.insert(make_pair(user.first, user.second->clone()));
		}
		activeUser = userMap[other.activeUser->getName()];
	}
	return *this;
}

//move assignment operator
Session &Session::operator=(Session &&other) {
	if (this != &other) {
		for (Watchable *cont:content)
			delete cont;
		for (BaseAction *log:actionsLog)
			delete log;
		for (auto pu:userMap)
			delete pu.second;

		this->activeUser = other.activeUser;
		this->exit = other.exit;
		content = move(other.content);
		actionsLog = move(other.actionsLog);
		userMap = move(other.userMap);
	}
	return *this;
}

std::vector<std::string> Session::getCommand() {
	return command;
}

void Session::setCommand(std::vector<std::string> cmd) {
	command = cmd;
}
