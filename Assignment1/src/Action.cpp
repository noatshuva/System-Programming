#include <string>
#include <iostream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"

using namespace std;

BaseAction::BaseAction() : errorMsg(""), status(PENDING) {}

BaseAction::~BaseAction() {}

ActionStatus BaseAction::getStatus() const {
	return status;
}

void BaseAction::complete() {
	status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
	status = ERROR;
	this->errorMsg = errorMsg;
	cout << getErrorMsg() << endl;
}

std::string BaseAction::getErrorMsg() const {
	string error = "Error - " + errorMsg;
	return error;
}


void CreateUser::act(Session &sess) {
	string name = sess.getCommand()[1];
	string rec = sess.getCommand()[2];
	unordered_map<string, User *> userMap = sess.getUserMap();
	if (userMap.find(name) != userMap.end())
		error("there is already a user with that name");
	else if (rec != "len" && rec != "rer" && rec != "gen")
		error("the 3-letter code is invalid");
	else {
		User *newUser;
		if (rec == "len")
			newUser = new LengthRecommenderUser(name);
		else if (rec == "rer")
			newUser = new RerunRecommenderUser(name);
		else //if (rec == "gen")
			newUser = new GenreRecommenderUser(name);
		sess.setUserMap(name, newUser);
		complete();
	}
}

CreateUser::~CreateUser() {}

std::string CreateUser::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "CreateUser PENDING";
	if (getStatus() == COMPLETED)
		log = "CreateUser COMPLETED";
	if (getStatus() == ERROR)
		log = "CreateUser ERROR: " + getErrorMsg();
	return log;
}

BaseAction *CreateUser::clone() {
	return new CreateUser(*this);
}

void ChangeActiveUser::act(Session &sess) {
	string name = sess.getCommand()[1];
	unordered_map<string, User *> userMap = sess.getUserMap();
	if (userMap.find(name) == userMap.end()) {
		error("the user doesn't exist");
		return;
	}
	//else
		sess.setActiveUser(userMap[name]);
	complete();
}

ChangeActiveUser::~ChangeActiveUser() {}

std::string ChangeActiveUser::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "ChangeActiveUser PENDING";
	if (getStatus() == COMPLETED)
		log = "ChangeActiveUser COMPLETED";
	if (getStatus() == ERROR)
		log = "ChangeActiveUser ERROR: " + getErrorMsg();
	return log;
}

BaseAction *ChangeActiveUser::clone() {
	return new ChangeActiveUser(*this);
}

void DeleteUser::act(Session &sess) {
	string name = sess.getCommand()[1];
	unordered_map<string, User *> Map = sess.getUserMap();
	if (Map.find(name) == Map.end()) {
		error("the user doesn't exist");
		return;
	}
	//else
		sess.delUser(name);
	complete();
}

DeleteUser::~DeleteUser() {}

std::string DeleteUser::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "DeleteUser PENDING";
	if (getStatus() == COMPLETED)
		log = "DeleteUser COMPLETED";
	if (getStatus() == ERROR)
		log = "DeleteUser ERROR: " + getErrorMsg();
	return log;
}

BaseAction *DeleteUser::clone() {
	return new DeleteUser(*this);
}

void DuplicateUser::act(Session &sess) {
	string oUser = sess.getCommand()[1];
	string nUser = sess.getCommand()[2];
	//get wanted user's name, if not exist in umap- > error (ouser)
	unordered_map<string, User *> &userMap = sess.getUserMap();
	if (userMap.find(oUser) == userMap.end()) {
		error("the original user doesn't exist");
		return;
	}
	if (userMap.find(nUser) != userMap.end()) {    //check for nUser in umap, if exist -> error
		error("new user name is already taken");
		return;
	}
	//get wanted user's  history, and recommendation alg
	User *orgUser = userMap[oUser];
	vector<Watchable *> history = orgUser->get_history();
	string rec = orgUser->getRecType();

	//create 3 letter alg, and create user with nUser as name and 2 letter as rec
	User *newUser = orgUser->clone();
	newUser->setName(nUser);
	userMap.insert({nUser, newUser});

	complete();
}

DuplicateUser::~DuplicateUser() {}

std::string DuplicateUser::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "DuplicateUser PENDING";
	if (getStatus() == COMPLETED)
		log = "DuplicateUser COMPLETED";
	if (getStatus() == ERROR)
		log = "DuplicateUser ERROR: " + getErrorMsg();
	return log;
}

BaseAction *DuplicateUser::clone() {
	return new DuplicateUser(*this);
}

void PrintContentList::act(Session &sess) {
	std::vector<Watchable *> content = sess.getContent();
	for (size_t i = 0; i < content.size(); i++) {
		string str = content[i]->toString();
		cout << i + 1 << ". " << str << endl;
	}
	complete();
}

PrintContentList::~PrintContentList() {}

std::string PrintContentList::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "PrintContentList PENDING";
	if (getStatus() == COMPLETED)
		log = "PrintContentList COMPLETED";
	if (getStatus() == ERROR)
		log = "PrintContentList ERROR: " + getErrorMsg();
	return log;
}

BaseAction *PrintContentList::clone() {
	return new PrintContentList(*this);
}

void PrintWatchHistory::act(Session &sess) {
	std::string name = sess.getActiveUser()->getName();
	cout << "Watch history for " << name << endl;
	std::vector<Watchable *> history = sess.getActiveUser()->get_history();
	for (size_t i = 0; i < history.size(); i++) {
		string str = history[i]->toString();
		cout << i + 1 << ". " << str << endl;
	}
	complete();
}

PrintWatchHistory::~PrintWatchHistory() {}

std::string PrintWatchHistory::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "PrintWatchHistory PENDING";
	if (getStatus() == COMPLETED)
		log = "PrintWatchHistory COMPLETED";
	if (getStatus() == ERROR)
		log = "PrintWatchHistory ERROR: " + getErrorMsg();
	return log;
}

BaseAction *PrintWatchHistory::clone() {
	return new PrintWatchHistory(*this);
}

void Watch::act(Session &sess) {
	long id = stol(sess.getCommand()[1]);
	vector<Watchable *> content = sess.getContent();
	Watchable *watching = content.at(id - 1);
	string nameOfCont = "Watching " + watching->getName(),
			str = watching->toString(),
			name = watching->getName(),
			noname,
			sande,
			nextWatch;

	int namelen = name.length();
	noname = str.substr(namelen + 1, str.length() - 1);

	if (watching->getType() == "episode") {
	sande = noname.substr(0, noname.find_first_of(' '));
	nameOfCont += " " + sande;
	}
	cout << nameOfCont << endl;
	User *user = sess.getActiveUser();
	user->setHistory(watching); //enter the current watch t the history
	Watchable *nameOfNextRec = watching->getNextWatchable(sess); //get the next watchable content
	if (!nameOfNextRec) {
		complete();
		return;
	}
	string str2 = nameOfNextRec->toString();
	name = nameOfNextRec->getName();
	if (nameOfNextRec->getType() == "movie") {
		nextWatch = "We recommend watching " + name;
	}
	if (nameOfNextRec->getType() == "episode") {
		namelen = name.length();
		noname = str2.substr(namelen + 1, str2.length() - 1);
		sande = noname.substr(0, noname.find_first_of(' '));
		nextWatch = "We recommend watching " + name + " " + sande;
	}
	nextWatch += ", continue watching? [y/n]";
	cout << nextWatch << endl;
	string choice;
	getline(cin, choice);
	if (choice == "y") {
		long ans = nameOfNextRec->getId();
		sess.setCommand(vector<string>{"watch", to_string(ans)});
		BaseAction *next = new Watch();
		next->act(sess);
		delete (next);
	}
	complete();
}

Watch::~Watch() {}

std::string Watch::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "Watch PENDING";
	if (getStatus() == COMPLETED)
		log = "Watch COMPLETED";
	if (getStatus() == ERROR)
		log = "Watch ERROR: " + getErrorMsg();
	return log;
}

BaseAction *Watch::clone() {
	return new Watch(*this);
}

void PrintActionsLog::act(Session &sess) {
	std::vector<BaseAction *> actionLog = sess.getActionLog();
	for (size_t i = 0; i < actionLog.size(); i++) {
		size_t pos = actionLog.size() - 1 - i;
		cout << actionLog.at(pos)->toString() << endl;
	}
	complete();
}

PrintActionsLog::~PrintActionsLog() {}

std::string PrintActionsLog::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "PrintActionsLog PENDING";
	if (getStatus() == COMPLETED)
		log = "PrintActionsLog COMPLETED";
	if (getStatus() == ERROR)
		log = "PrintActionsLog ERROR: " + getErrorMsg();
	return log;
}

BaseAction *PrintActionsLog::clone() {
	return new PrintActionsLog(*this);
}

void Exit::act(Session &sess) {
	sess.setExit();
	complete();
}

Exit::~Exit() {}

std::string Exit::toString() const {
	string log;
	if (getStatus() == PENDING)
		log = "Exit PENDING";
	if (getStatus() == COMPLETED)
		log = "Exit COMPLETED";
	if (getStatus() == ERROR)
		log = "Exit ERROR: " + getErrorMsg();
	return log;
}

BaseAction *Exit::clone() {
	return new Exit(*this);
}
