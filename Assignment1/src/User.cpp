#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <algorithm>


using namespace std;

User::User(const std::string &name) : history(), name(name){}
std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const {
    return history;
}
void User::setHistory(Watchable* watch) {
	history.push_back(watch->clone());
}

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
	//return null when there is no history or all movies and episodes are watched
	std::vector<Watchable*> content = s.getContent();
	if (history.empty() || content.size()==history.size())
		return nullptr;
	int sumOfLength = 0;
    for(auto & j : history)
        sumOfLength += j->getLength();
    int av = sumOfLength/history.size();
    int closest = std::numeric_limits<int>::max();
    int place = 0;
    for( size_t i = 0; i < content.size(); i++){
    	bool found  = false;
    	for(size_t j = 0; j < history.size() && !found; j++){
    		if(history.at(j)->getId() == content[i]->getId())
    			found = true;
    	}
        if (!found) {
            int len = content[i] -> getLength();
            int dist = 0;
            dist = abs(len - av);
            if (dist < closest) {
				closest = dist;
				place = i;
			}
        }
    }
    return content.at(place);//
}
std::string LengthRecommenderUser::getRecType() {
	return  "len";
}
User* LengthRecommenderUser::clone() {
	return new LengthRecommenderUser(*this);
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), index(0){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
	if (history.empty())
		return nullptr;
    size_t n = history.size();
	index = (index+1)%n;
	return history[index];
}
std::string RerunRecommenderUser::getRecType() {
	return  "rer";
}
User* RerunRecommenderUser::clone() {
	return new RerunRecommenderUser(*this);
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name){}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
	//return null when there is no history or all movies and episodes are watched
	std::vector<Watchable*> content = s.getContent();
	if (history.empty() || content.size()==history.size())
		return nullptr;
	typedef std::pair<std::string,int> pair;
	std::unordered_map<std::string, int> umap;
	for(auto & i : history){
		std::vector<std::string> tags = i ->getTags();
		for(const auto & tag : tags) {
			if (umap.find(tag) == umap.end())
				umap[tag] = 1;
			else
				umap[tag]++;
		}
	}

	  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //this part of was copied from https://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<pair> vec;
	std::copy(umap.begin(),
			  umap.end(),
			  std::back_inserter<std::vector<pair>>(vec));
	std::sort(vec.begin(), vec.end(),
			  []( pair& l, pair& r) {
				  if (l.second != r.second)
					  return l.second > r.second;
				  return l.first < r.first;
			  });

	Watchable* output = nullptr;
	for(auto & i : content){
		bool found = false;
		for(size_t j = 0; j < history.size() && !found; j++){
			if(history.at(j)->getId() == i->getId())
				found = true;
		}
		if (!found) {
			std::vector<std::string> tags = i ->getTags();
			for(const auto & tag : tags) {
				for (auto &k : vec) {
					if (tag == k.first)
						output = i;
				}
			}
		}
	}
	return output;
}

std::string GenreRecommenderUser::getRecType() {
	return "gen";
}

User* GenreRecommenderUser::clone() {
	return new GenreRecommenderUser(*this);
}

//destructor
User::~User() {
	for(auto content:history) delete content;
	history.clear();
}

//copy constructor
User::User(const User &other) : history(), name(other.name) {
	for(Watchable* cont : other.history)
		history.push_back(cont->clone());
}

//move constructor
User::User(User &&other) :history(move(other.history)), name(other.name) {}

//copy assignment operator
User& User::operator=(const User &other) {
	if(this != &other) {
		history.clear();
		for (auto & i : history) delete(i);

		for (auto cont:other.history)
			this->history.push_back(cont->clone());
		this->name = other.name;
	}
	return *this;
}

//move assignment operator
User& User::operator=(User &&other) {
	if (this != &other) {
		for (size_t i = 0; i < history.size(); ++i)
			delete(history[i]);

		this->name = other.name;
		history=move(other.history);
	}
	return *this;
}

void User::setName(std::string name1) {
	name=name1;
}


