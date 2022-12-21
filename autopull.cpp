#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "color-console\include\color.hpp"

using namespace std;

vector<string> SplitString(string str)
{
	string s;
	stringstream ss(str);
	vector<string> v;
	while (getline(ss, s, ' ')) v.push_back(s);
	return v;
}

int main(int argc, char* argv[])
{
	ifstream inp;
	if (argc == 2) { // Open given file
		inp.open(argv[1]);
		cout << "Updating repositories from file " << argv[1] << "\n\n";
	}
	else inp.open("robilkot_autopull_repos.txt"); // .. Or the default one
	if (!inp.is_open()) {
		cerr << dye::light_red("Couldn't open file with repos list!\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	int updatedrepos=0, totalrepos=0;
	bool connectionerror = 0;

	for (string currentline; getline(inp, currentline); totalrepos++)
	{
		if (currentline.empty()) continue;
		vector<string> arguments(SplitString(currentline));
		string repo, remote, branch;
		switch (arguments.size()) {
		case 1: {
			repo = arguments[0];
			break;
		}
		case 2: {
			repo = arguments[0];
			remote = arguments[1];
			break;
		}
		default: {
			repo = arguments[0];
			remote = arguments[1];
			branch = arguments[2];
			break;
		}
		}

		cout << "Updating repository at " << repo << "; ";
		if (!remote.empty()) cout << "remote: '" << remote << "; ";
		if (!branch.empty()) cout << "branch: '" << branch << ";";
		cout << "\n";

		string filename_error = "autopull_error" + to_string(totalrepos) + ".txt"; // Forming filename for error logging
		string command_output = " > nul 2> " + filename_error; // set output file > for log, 2> for errors
		string command = "git --git-dir=" + repo + ".git " + "--work-tree=" + repo + " pull " + remote + " " + branch + command_output;

		for (int i = 0; i < 3; i++) {
			system(command.c_str()); // Execute command

			ifstream checkerror(filename_error);
			if (!checkerror.is_open()) cout << "Something went wrong! Couldn't check for erros.\n";
			string error;
			getline(checkerror, error);
			checkerror.close();

			if (error.empty() || error.find("From") != string::npos) {
				connectionerror = 0;
				cout << dye::light_green("\rUpdated!");
				cout.width(70);
				cout.fill(' ');
				cout << "\n";
				updatedrepos++;
				string del_command = "del " + filename_error + " > nul 2> nul";
				system(del_command.c_str());
				break;
			}

			string errortype;
			bool fatal = 0;
			if (error.find("Could not resolve") != string::npos) {
				errortype = "Connection error";
				connectionerror = 1;
			}
			else if (error.find("invalid refspec") != string::npos) {
				errortype = "Wrong branch or remote name";
				fatal = 1;
			}
			else if (error.find("does not appear") != string::npos) {
				errortype = "Incorrect remote or access denied";
				fatal = 1;
			}
			else if (error.find("not a git") != string::npos || error.find("no such file") != string::npos) {
				errortype = "Incorrect path or folder is not a repository";
				fatal = 1;
			}
			else if (error.find("local changes") != string::npos) {
				errortype = "Conflict while merging";
				fatal = 1;
			}
			else {
				errortype = "Unknown error";
			}

			if (errortype != "Unknown error") {
				string del_command = "del " + filename_error + " > nul 2> nul";
				system(del_command.c_str());
			}

			if (fatal) {
				cout << dye::light_red("Update failed! (") << dye::light_red(errortype) << dye::light_red(")\n");
				break;
			}
			else {
				if (i > 0) cout << "\r";
				cout << dye::light_yellow("Update failed! (") << dye::light_yellow(errortype) << dye::light_yellow(", attempt ") << dye::light_yellow(i + 1) << dye::light_yellow("/3)");
			}

			if (i < 2) {
				string timeout = "timeout " + to_string(5 * (i + 1)) + " > nul 2> nul";
				system(timeout.c_str());
			}
		}
		cout << "\n";
		if (connectionerror) {
			cout << "\n";
			break;
		}
	}
	inp.close();
	
	if (updatedrepos == 0) {
		cerr << dye::white_on_red("No repositories were updated! ");
		if(connectionerror) cout << dye::white_on_red("Check your internet connection.");
		cout << "\n";
		system("pause");
	}
	else if (updatedrepos != totalrepos) {
		cout << dye::on_light_yellow(updatedrepos) << dye::on_light_yellow("/") << dye::on_light_yellow(totalrepos) << dye::on_light_yellow(" repositories updated! See above for details.\n");
		system("pause");
	}
	else cout << dye::light_green("Succesfully updated all repositories!\n");
	return 0;
}
