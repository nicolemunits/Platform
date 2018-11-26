/*
 * Global.h
 *
 *  Created on: May 24, 2016
 *      Author: user
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <vector>

#define DEBUG 1

using namespace std;
namespace npl {

class Global {
public:

	Global();

	virtual ~Global();

	static string convertTostring(int i) {
		string Result;
		ostringstream convert;
		convert << i;
		Result = convert.str();
		return Result;
	}

	static void printClientInstructions() {
		cout << "----------------------------------------------------------"
				<< endl;
		cout
				<< "c <server ip>                    - Connect Server (default: localhost)"
				<< endl;
		cout
				<< "register <user name> <password>  - Register User (and auto login)"
				<< endl;
		cout << "login <user name> <password>     - Login" << endl;
		cout << "lcu                              - List Connected Users"
				<< endl;
		cout
				<< "sg <user>                        - Start game with specific user"
				<< endl;
		cout << "rsg                              - Start game with random user"
				<< endl;
		cout << "d                                - Disconnect" << endl;
		cout << "help                             - This MSG" << endl;
		cout << "x                                - Quit" << endl;
		cout << "--------------------------------------------------------"
				<< endl;

	}

	static void debug(string msg) {
		if (DEBUG) {
			cout << "[DEBUG] " << msg << endl;
		}
	}

	static string* splitString(string data) {
		bool flag = false;
		string * myStr = new string[2];
		string str;
		istringstream iss(data);
		while (iss) {
			string temp;
			iss >> temp;
			if (flag == false) {
				myStr[0] = temp;
				flag = true;
			} else {
				str.append(temp);
			}
		}
		myStr[1] = str;
		return myStr;
	}

	static void printMap(map<string, string> data) {
		cout << "{" << endl;
		for (map<string, string>::iterator it = data.begin(); it != data.end();
				it++) {
			cout << "   " << it->first << " " << it->second << endl;
		}
		cout << "}" << endl;
	}

	static void printMapInt(map<int, string> data) {
		cout << "{" << endl;
		for (map<int, string>::iterator it = data.begin(); it != data.end();
				it++) {
			cout << "   " << it->first << " " << it->second << endl;
		}
		cout << "}" << endl;
	}
};

} /* namespace npl */

#endif /* GLOBAL_H_ */
