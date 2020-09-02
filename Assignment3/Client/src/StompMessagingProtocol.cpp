//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//
#include "StompMessagingProtocol.h"
#include "DataBase.h"
#include "book.h"
#include <vector>
#include <iostream>

using namespace std;

StompMessagingProtocol::StompMessagingProtocol(ConnectionHandler *connectionHandler1, DataBase *db1)
		: connectionHandler(connectionHandler1), db(db1) {}

string StompMessagingProtocol::process(vector<string> vec) {

	if (vec.size() >= 2 && vec[0] == "join") {
		int recID = db->increaseAndGetReceiptId();
		int topID = db->increaseAndFetTopicId();
		string rec = std::to_string(recID);
		string id = std::to_string(topID);
		db->addToTopicMap(topID, vec[1]);
		db->addToReceiptMap(recID, "SUBSCRIBE:" + vec[1]);
		string str = "SUBSCRIBE\ndestination:" + vec[1] + "\nid:" + id + "\nreceipt:" + rec + "\n\n";
		connectionHandler->sendFrameAscii(str, '\0');
	}

	if (vec.size() >= 2 && vec[0] == "exit") {
		int recID = db->increaseAndGetReceiptId();
		string rec = std::to_string(recID);
		string id = std::to_string(db->getIdByTopic(vec[1]));
		db->removeFromTopicMap(vec[1]);
		db->addToReceiptMap(recID, "UNSUBSCRIBE:" + vec[1]);
		string str = "UNSUBSCRIBE\nid:" + id + "\nreceipt:" + rec + "\n\n";
		connectionHandler->sendFrameAscii(str, '\0');
	}

	if (vec.size() >= 1 && vec[0] == "logout") {
		int recID = db->increaseAndGetReceiptId();
		string rec = std::to_string(recID);
		db->addToReceiptMap(recID, "DISCONNECT:");
		string str = "DISCONNECT\nreceipt:" + rec + "\n\n";
		connectionHandler->sendFrameAscii(str, '\0');
		db->removeUserFromAllTopics();
	}

	if (vec.size() >= 2 && vec[0] == "RECEIPT") {
		string recId = vec[1].substr(vec[1].find(":") + 1);
		string frame = db->getFrameByRecId( stoi(recId));
		string frameType = frame.substr(0, frame.find(":"));
		string frameVal = frame.substr(frame.find(":") + 1);
		if (frameType == "SUBSCRIBE")
			cout << "Joined club " + frameVal << endl;
		if (frameType == "UNSUBSCRIBE")
			cout << "Exited club " + frameVal << endl;
		if(frameType == "DISCONNECT")
			connectionHandler->terminate();
	}

	if(vec.size() >= 3 && vec[0] == "add"){
		string str = "SEND\ndestination:" + vec[1] + "\n\n" + db->getUserName() + " has added the book ";
		string bookName = vec[2];
		for (size_t i = 3; i < vec.size(); i++) {
				bookName = bookName + " " + vec[i];
		}
		book newBook (bookName, vec[1], db->getUserName());
		db->addToInventory(vec[1], newBook);
		str = str + bookName + "\n";
		connectionHandler->sendFrameAscii(str,'\0');
	}

	if(vec.size() >= 3 && vec[0] == "borrow"){
		string bookName = vec[2];
		for (size_t i = 3; i < vec.size(); i++) {
			bookName = bookName + " " + vec[i];
		}
		book wantBook(bookName, vec[1], "");
		db->addToWishList(wantBook);
		string str = "SEND\ndestination:"+vec[1]+"\n\n"+db->getUserName()+ " wish to borrow "+ bookName +"\n";
		connectionHandler->sendFrameAscii(str,'\0');
	}

	if(vec.size() >= 3 && vec[0] == "return"){
		string bookName = vec[2];
		for (size_t i = 3; i < vec.size(); i++) {
			bookName = bookName + " " + vec[i];
		}
		db->removeFromInventory(vec[1], bookName);
		string name = db->getFromBorrowedMap(bookName);
		if(name == "notFound")
			name = "";
		string str = "SEND\ndestination:"+vec[1]+"\n\nReturning "+ bookName + " to " + name +"\n";
		connectionHandler->sendFrameAscii(str,'\0');
	}

	if(vec.size() >= 2 && vec[0] == "status"){
		string str = "SEND\ndestination:"+vec[1]+"\n\nbook status\n";
		connectionHandler->sendFrameAscii(str,'\0');
	}


	if(vec.size() >= 6 && vec[0] == "MESSAGE") {
		istringstream iss(vec[4]);
		std::vector<string> results(std::istream_iterator<std::string>{iss},
									std::istream_iterator<std::string>());

		if (results.size() >= 5 && results[3] == "borrow") {
			string bookName = results[4];
			for (size_t i = 5; i < results.size() ; i++) {
				bookName = bookName + " " + results[i];
			}
			string topic = vec[3].substr(vec[3].find(":") + 1);
			book toBor = db->getBookByName(bookName, topic);
			if (db->isExistInInventory(topic, bookName)) {
				string str = "SEND\ndestination:" + topic + "\n\n" + db->getUserName() + " has " + bookName + "\n";
				connectionHandler->sendFrameAscii(str, '\0');
			}
		}
		else if (results.size() >= 3 && results[1] == "has") {
			string bookName = results[2];
			for (size_t i = 3; i < results.size() ; i++) {
				bookName = bookName + " " + results[i];
			}
			if (db->isThisBookIsWished(bookName)) {
				book theWish = db->getWishedBook(bookName);
				theWish.setOwner(results[0]);
				string desti = vec[3].substr(vec[3].find(":") + 1);
				db->addToBorrowMap(bookName, results[0]);
				db->addToInventory(desti ,theWish);
				db->removeFromWishList(theWish);
				string str = "SEND\ndestination:" + desti +"\n\nTaking " + bookName + " from " +results[0] + "\n";
				connectionHandler->sendFrameAscii(str, '\0');
			}
		}
		else if (results.size() >= 4 && results[0] == "Taking"){
			int n = results.size()-1;
			string bookName = results[1];
			for (size_t i = 2; i < results.size() - 2 ; i++) {
				bookName = bookName + " " + results[i];
			}
			string desti = vec[3].substr(vec[3].find(":") + 1);
			if(results[n] == db->getUserName()) {
				db->removeFromInventory(desti, bookName);
			}
		}
		else if(results.size() >= 4 && results[0] == "Returning"){
			int n = results.size()-1;
			string bookName = results[1];
			for (size_t i = 2; i < results.size() - 2 ; i++) {
				bookName = bookName + " " + results[i];
			}
			if(results[n] == db->getUserName()){
				string topic = vec[3].substr(vec[3].find(":") + 1);
				string name = db->getFromBorrowedMap(bookName);
				if(name == "notFound"){
					book booki(bookName, topic,"");
					db->addToInventory(topic, booki);
				} else{
					book booki(bookName, topic,name);
					db->addToInventory(topic, booki);
				}
			}
		}
		else if (results.size()>=2 && results[1] == "status"){
			string desti = vec[3].substr(vec[3].find(":") + 1);
			if(db->checkSub(desti)){
				vector<book> books = db->getBooks(desti);
				string booksString = "";
				for(size_t i = 0; i<books.size(); i++){
						if (i == books.size() - 1)
							booksString = booksString + books[i].getName();
						else
							booksString = booksString + books[i].getName() + ",";
				}
				string str = "SEND\ndestination:" + desti +"\n\n" + db->getUserName() + ":" +booksString + "\n";
				connectionHandler->sendFrameAscii(str, '\0');
			}
		}
		cout<< vec[4] <<endl;
	}
	return "";
}
