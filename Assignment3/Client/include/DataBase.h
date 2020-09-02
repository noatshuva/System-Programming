//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_DATABASE_H
#define BOOST_ECHO_CLIENT_DATABASE_H

#include <string>
#include <queue>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>
#include "book.h"

using namespace std;

class DataBase {
public:
	DataBase(string username);
	int increaseAndGetReceiptId();
	int increaseAndFetTopicId();
	void addToTopicMap(int id, string topic);
	void addToReceiptMap(int id, string topic);
	void removeFromTopicMap(string topic);
	void removeFromReceiptMap(int id);
	void addToInventory(string topic, book book);
	std::string getUserName();
	void removeFromInventory(string topic,  string bookName);
	book getBookByName(string bookName, string topic);
	string getFrameByRecId(int id);
	string getTopicById(int id);
	void addToWishList(book booki);
	bool isThisBookIsWished(string bookName);
	void removeFromWishList(book booki);
	book getWishedBook(string bookName);
	bool isExistInInventory(string topic, string bookName);
	void addToQueue(string msg);
	void setUserName(string name);
	bool queIsEmpty();
	string getMsg();
	int getIdByTopic(string topic);
	bool checkSub(string topic);
	vector<book> getBooks(string topic);
	void removeUserFromAllTopics();
	void addToBorrowMap(string bookName, string owner);
	string getFromBorrowedMap(string bookName);
private:
	std::string userName;
	queue<string> msgQue;
	map<int ,string> receiptMap;
	map<string, vector<book>> inventory;
	map<int ,string> topicMap;
	map<string, string> bookBorrowed;
	vector<book> wishList;
	std::atomic_int receiptId;
	std::atomic_int topicId;
	mutex inventoryLock;
	mutex receiptLock;
	mutex topicLock;
	mutex wishListLock;
	mutex msgQueLock;
	mutex borrowedLock;
};


#endif //BOOST_ECHO_CLIENT_DATABASE_H
