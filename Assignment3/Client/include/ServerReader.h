//
// Created by noate@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SERVERREADER_H
#define BOOST_ECHO_CLIENT_SERVERREADER_H


#include "ConnectionHandler.h"

class ServerReader {
public:
	ServerReader(ConnectionHandler *pHandler, DataBase *pBase);
	void run();

private:
	ConnectionHandler* connectionHandler;
	DataBase* dataBase;
};


#endif //BOOST_ECHO_CLIENT_SERVERREADER_H
