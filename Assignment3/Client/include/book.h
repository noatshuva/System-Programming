//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//


#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include<string>

using namespace std;

class book {
public:
	book(string name, string genre, string owner);
	string getName();
	void setOwner(string o);

private:
	string name;
	string genre;
	string owner;
};


#endif //BOOST_ECHO_CLIENT_BOOK_H
