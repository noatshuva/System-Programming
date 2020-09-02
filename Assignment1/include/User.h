#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const ;
    void setHistory(Watchable* watch);
	virtual std::string getRecType() = 0;
	virtual ~User();
	User(const User &other);
	User &operator=(const User &other);
	User &operator=(User&& other);
	User(User &&other);
	virtual User* clone() = 0;
	void setName(std::string nname);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
	std::string getRecType();
	User* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
	std::string getRecType();
	User* clone();
private:
	size_t index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
	std::string getRecType();
	User* clone();
private:
};

#endif
