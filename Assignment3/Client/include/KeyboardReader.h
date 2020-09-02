//
// Created by noate@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARDREADER_H
#define BOOST_ECHO_CLIENT_KEYBOARDREADER_H


#include "ConnectionHandler.h"

class KeyboardReader {
public:
	KeyboardReader(ConnectionHandler *pHandler, DataBase *pBase);
	void run();

private:
	ConnectionHandler* connectionHandler;
	DataBase* db;
};


#endif //BOOST_ECHO_CLIENT_KEYBOARDREADER_H
