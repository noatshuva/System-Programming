//
// Created by noate@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include <StompMessagingProtocol.h>
#include "ServerReader.h"

using namespace std;

ServerReader::ServerReader(ConnectionHandler *pHandler, DataBase *pBase) : connectionHandler(pHandler), dataBase(pBase){}

void ServerReader::run() {
	StompMessagingProtocol protocol = StompMessagingProtocol(connectionHandler, dataBase);
	while (!connectionHandler->shouldTerminate()){
		string str;
		if(connectionHandler->getFrameAscii(str, '\0')){
			vector<string> vec;
			string word;
			for (auto x : str) { //input the string to vector
				if (x == '\n' || x == '\r') {
					vec.push_back(word);
					word = "";
				} else
					word += x;
			}
			vec.push_back(word);
			protocol.process(vec);
		} else{
			cout << "Exiting..." << endl;
			break;
		}
	}
}