CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/book.o bin/ConnectionHandler.o bin/DataBase.o bin/echoClient.o bin/KeyboardReader.o bin/ServerReader.o bin/StompMessagingProtocol.o $(LDFLAGS)

StompBookClubClient: bin/book.o bin/ConnectionHandler.o bin/DataBase.o bin/echoClient.o bin/KeyboardReader.o bin/ServerReader.o bin/StompMessagingProtocol.o

bin/book.o: src/book.cpp
	g++ -pthread $(CFLAGS) -o bin/book.o src/book.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/DataBase.o: src/DataBase.cpp
	g++ -pthread $(CFLAGS) -o bin/DataBase.o src/DataBase.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ -pthread $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/KeyboardReader.o: src/KeyboardReader.cpp
	g++ -pthread $(CFLAGS) -o bin/KeyboardReader.o src/KeyboardReader.cpp

bin/ServerReader.o: src/ServerReader.cpp
	g++ -pthread $(CFLAGS) -o bin/ServerReader.o src/ServerReader.cpp

bin/StompMessagingProtocol.o: src/StompMessagingProtocol.cpp
	g++ -pthread $(CFLAGS) -o bin/StompMessagingProtocol.o src/StompMessagingProtocol.cpp



.PHONY: clean
clean:
	rm -f bin/*
