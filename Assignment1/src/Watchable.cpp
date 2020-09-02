#include <string>
#include <vector>
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"

using namespace std;

Watchable::Watchable(long id, int length, const vector<string> &tags) : id(id), length(length), tags(tags) {}

Watchable::~Watchable() {}

long Watchable::getId() const {
	return id;
}

int Watchable::getLength() const {
	return length;
}

vector<string> Watchable::getTags() const {
	return tags;
}

Movie::Movie(long id, const string &name, int length, const vector<string> &tags) : Watchable(id, length, tags),
																					name(name) {}

string Movie::toString() const {
	int length = getLength();
	vector<string> tags;
	string str;
	if (!(tags = getTags()).empty()) {
		for (size_t i = 0; i < tags.size(); i++)
			str += ", " + tags[i];
		str = str.substr(2);
	}
	string lengthS = to_string(length);
	string output = name + " " + lengthS + " minutes [" + str + "]";
	return output;
}

string Movie::getName() {
	return name;
}

string Movie::getType() {
	return "movie";
}

Watchable *Movie::getNextWatchable(Session &s) const {
	User *user = s.getActiveUser();
	Watchable *output = user->getRecommendation(s);
	return output;
}

Watchable *Movie::clone() {
	Watchable *movie = new Movie(this->getId(), this->getName(), this->getLength(), this->getTags());
	return movie;
}

Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
				 const vector<string> &tags) : Watchable(id, length, tags), seriesName(seriesName), season(season),
											   episode(episode), nextEpisodeId(id + 1) {}

string Episode::toString() const {
	int length = getLength();
	vector<string> tags = getTags();
	string string;
	for (size_t i = 0; i < tags.size(); i++) {
		if (i == tags.size() - 1)
			string = string + tags[i];
		else
			string = string + tags[i] + ", ";
	}
	std::string lengthS = to_string(length);
	std::string seasonS = to_string(season);
	std::string episodeS = to_string(episode);
	std::string output = seriesName + " S" + seasonS + "E" + episodeS + ". " + lengthS + " minutes [" + string + "]";
	return output;
}

string Episode::getName() {
	return seriesName;
}

string Episode::getType() {
	return "episode";
}

Watchable *Episode::getNextWatchable(Session &s) const {
	User *user = s.getActiveUser();
	Watchable *nextCon=nullptr;
	int contSize = s.getContent().size();
	if (nextEpisodeId < contSize) {
		Watchable *nextEp = s.getContent().at(nextEpisodeId - 1);
		if (nextEp->getType() == "episode") {
			string nameNext = nextEp->getName();
			if (nameNext == seriesName) {
				nextCon = nextEp;
			}
		}
	}
	if(!nextCon)
		nextCon = user->getRecommendation(s);
	return nextCon;
}

Watchable *Episode::clone() {
	Watchable *episode = new Episode(this->getId(), this->getName(), this->getLength(), this->season, this->episode,
									 this->getTags());
	return episode;
}


