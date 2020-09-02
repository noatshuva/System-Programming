//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include "book.h"
#include <iostream>

using namespace std;

book::book(string name, string genre, string owner) : name(name), genre(genre), owner(owner){}

string book::getName(){
	return name;
}

void book::setOwner(string o) {
	owner = o;
}

