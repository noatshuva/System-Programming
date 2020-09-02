//
// Created by noate@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include <StompMessagingProtocol.h>
#include "KeyboardReader.h"

using namespace std;

KeyboardReader::KeyboardReader(ConnectionHandler *pHandler, DataBase *pBase) : connectionHandler(pHandler) , db(pBase){}

void KeyboardReader::run(){
	StompMessagingProtocol protocol(connectionHandler,db);
	while (!connectionHandler->shouldTerminate()){
		if(!db->queIsEmpty()) {
			string nextMessage = db->getMsg();
			vector<string> vec;
			string word;
			for (auto x : nextMessage) { //input the string to vector
				if (x == ' ') {
					vec.push_back(word);
					word = "";
				} else
					word += x;
			}
			vec.push_back(word);
			protocol.process(vec);
		}
	}

}



