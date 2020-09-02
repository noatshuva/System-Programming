//
// Created by noate@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include "DataBase.h"
#include <vector>
#include <map>
#include "book.h"

using namespace std;

DataBase::DataBase(string username) : userName(username), msgQue(queue<string>()), receiptMap(map<int ,string>()), inventory(map<string, vector<book>>()), topicMap(map<int ,string>()), bookBorrowed(map<string, string>()), wishList(vector<book>()), receiptId(0),
									  topicId(0), inventoryLock(), receiptLock(), topicLock(), wishListLock(), msgQueLock(), borrowedLock() {}


int DataBase::increaseAndGetReceiptId() {
	return receiptId++;
}

int DataBase::increaseAndFetTopicId() {
	return topicId++;
}

void DataBase::addToTopicMap(int id, string topic) {
	topicLock.lock();
	topicMap[id] = topic;
	topicLock.unlock();
}

void DataBase::addToReceiptMap(int id, string topic) {
	receiptLock.lock();
	receiptMap[id] = topic;
	receiptLock.unlock();
}

void DataBase::removeFromReceiptMap(int id) {
	receiptLock.lock();
	if(receiptMap.begin() != receiptMap.end()) {
		for (auto &x : receiptMap) {
			if (x.first == id){
				topicMap.erase(x.first);
			}
		}
	}
	receiptLock.unlock();
}

void DataBase::removeFromTopicMap(string topic) {
	topicLock.lock();
	if(topicMap.begin() != topicMap.end()) {
		for (auto &x : topicMap) {
			if (x.second == topic){
				topicMap.erase(x.first);
			}
		}
	}
	topicLock.unlock();
}

void DataBase::addToInventory(string topic, book booki) {
	inventoryLock.lock();
	if (inventory.find(topic) == inventory.end()) {
		vector<book> books;
		inventory.emplace(topic, books);
	}
	inventory.at(topic).push_back(booki);
	inventoryLock.unlock();
}

void DataBase::removeFromInventory(string topic, string bookName) {
	inventoryLock.lock();
	if(inventory.begin() != inventory.end()) {
		for (size_t i = 0; i < inventory.at(topic).size() ; i++) {
			if (bookName == inventory.at(topic).at(i).getName()) {
				inventory.at(topic).erase(inventory.at(topic).begin()+i);
			}
		}
	}
	inventoryLock.unlock();
}

string DataBase::getUserName() {
	return userName;
}

book DataBase::getBookByName(string bookName, string topic) {
	book output("","","");
	inventoryLock.lock();
	if(inventory.begin() != inventory.end()) {
		for (size_t i = 0; i < inventory.at(topic).size(); i++) {
			if (bookName == inventory.at(topic).at(i).getName()) {
				inventoryLock.unlock();
				output = inventory.at(topic).at(i);
			}
		}
	}
	inventoryLock.unlock();
	return output;
}

string DataBase::getFrameByRecId(int id) {
	return receiptMap[id];
}

string DataBase::getTopicById(int id) {
	return topicMap[id];
}

void DataBase::addToWishList(book booki) {
	wishListLock.lock();
	wishList.push_back(booki);
	wishListLock.unlock();
}

bool DataBase::isThisBookIsWished(string bookName) {
	bool output = false;
	for (auto& booki : wishList) {
		if (booki.getName() == bookName)
			output = true;
	}
	return output;
}

book DataBase::getWishedBook(string bookName) {
	wishListLock.lock();
	for (auto booki : wishList) {
		if (booki.getName() == bookName) {
			wishListLock.unlock();
			return booki;
		}
	}
	wishListLock.unlock();

}

void DataBase::removeFromWishList(book booki) {
	for (size_t i = 0; i < wishList.size(); i++) {
		wishListLock.lock();
		if (wishList[i].getName() == booki.getName()) {
			wishList.erase(wishList.begin() + i);
		}
		wishListLock.unlock();
	}
}

bool DataBase::isExistInInventory(string topic, string bookName) {
	bool output = false;
	inventoryLock.lock();
	if (inventory.find(topic) != inventory.end()) {
		for (size_t i = 0; i < inventory.at(topic).size() && !output; i++) {
			if (bookName == inventory.at(topic).at(i).getName()) {
				output = true;
			}
		}
	}
	inventoryLock.unlock();
	return output;
}

void DataBase::addToQueue(string msg) {
	msgQueLock.lock();
	msgQue.push(msg);
	msgQueLock.unlock();
}

void DataBase::setUserName(string name) {
	userName = name;
}

string DataBase::getMsg() {
	string output = "";
	msgQueLock.lock();
	if (!msgQue.size() == 0) {
		output = msgQue.front();
		msgQue.pop();
	}
	msgQueLock.unlock();
	return output;
}

bool DataBase::queIsEmpty() {
	return msgQue.size() == 0;
}

int DataBase::getIdByTopic(string topic) {
	if (topicMap.begin() != topicMap.end()) {
		for (auto &x : topicMap) {
			if (x.second == topic) {
				return x.first;
			}
		}
	}
	return -1;
}


bool DataBase::checkSub(string topic) {
	bool output = false;
	int id = getIdByTopic(topic);
	if(id != -1){
		output = true;
	}
	return output;
}

vector<book> DataBase::getBooks(string topic) {
	vector<book> books;
	inventoryLock.lock();
	if (inventory.find(topic) != inventory.end())
		books = inventory.at(topic);
	inventoryLock.unlock();
	return books;
}

void DataBase::removeUserFromAllTopics() {
	inventoryLock.lock();
	inventory.clear();
	inventoryLock.unlock();
	topicLock.lock();
	topicMap.clear();
	topicLock.unlock();
}

void DataBase::addToBorrowMap(string bookName, string owner) {
	borrowedLock.lock();
	bookBorrowed[bookName] = owner;
	borrowedLock.unlock();
}

string DataBase::getFromBorrowedMap(string bookName) {
	if(bookBorrowed.find(bookName) != bookBorrowed.end())
		return bookBorrowed[bookName];
	return "notFound";
}





