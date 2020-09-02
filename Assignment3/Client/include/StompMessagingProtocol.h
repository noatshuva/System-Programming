//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H

#include "ConnectionHandler.h"
#include <vector>
#include "DataBase.h"
#include "book.h"

using namespace std;

class StompMessagingProtocol {
public:
	StompMessagingProtocol(ConnectionHandler* connectionHandler1, DataBase* db1);
	string process(vector<string> vector);
private:
	ConnectionHandler* connectionHandler;
	DataBase *db;
};


#endif //BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
