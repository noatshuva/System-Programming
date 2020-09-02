#include <ConnectionHandler.h>
#include <KeyboardReader.h>
#include <thread>
#include <ServerReader.h>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/

// the last one //

vector<string> splitFromS(string str) {
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
	return vec;
}

vector<string> splitFromK(string str) {
	vector<string> vec;
	string word;
	for (auto x : str) { //input the string to vector
		if (x == ' ') {
			vec.push_back(word);
			word = "";
		} else
			word += x;
	}
	vec.push_back(word);
	return vec;
}


int main (int argc, char *argv[]) {
	string str;
	vector<string> vec;
	DataBase data("");
	while (str != "bye") {
		getline(cin, str);
		vec = splitFromK(str);
		if (vec.size() >= 4 && vec[0] == "login") {
			string hostPort = vec[1];
			data.setUserName(vec[2]);
			string host = hostPort.substr(0, hostPort.find(":"));
			string port = hostPort.substr(hostPort.find(":") + 1);
			ConnectionHandler con(host, stoi(port));
			if (con.connect()) {
				int recID = data.increaseAndGetReceiptId();
				string rec = std::to_string(recID);
				string str = "CONNECT\naccept-version:1.2\nhost:" + host + "\nlogin:" + vec[2] + "\npasscode:" + vec[3] +"\nreceipt:" + rec + "\n\n";
				con.sendFrameAscii(str, '\0');
				string input = "";
				con.getFrameAscii(input, '\0');
				vector<string> vec1 = splitFromS(input);
				if (vec1.size() >= 2 && vec1[0] == "CONNECTED") {
					std::cout << "Login successful" << endl;

					KeyboardReader task1(&con, &data);
					std::thread userThread(&KeyboardReader::run, &task1);
					ServerReader task2(&con, &data);
					std::thread serverThread(&ServerReader::run, &task2);

					while (str != "logout") {
						getline(cin, str);
						data.addToQueue(str);
					}

					userThread.join();
					serverThread.join();

					con.close();

				} else if (vec1[1].size() >= 3 && vec1[0] == "ERROR") {
					string error = vec1[2].substr(vec1[2].find(":") + 1);
					std::cout << error << endl;
				}
			} else {
				std::cout << "Could not connect to server" << endl;
			}
		}

	}
}














