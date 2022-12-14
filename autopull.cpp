#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define RED		"\033[31m"
#define YELLOW "\033[33m"
#define GREEN	"\033[32m"
#define WHITE	"\033[37m"
#define RESET	"\033[0m"

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
	if (argc > 1) {
		inp.open(argv[1]);
		cout << "Updating repositories from file " << argv[1] << "\n\n";
	}
	else inp.open("robilkot_autopull_repos.txt");
	if (!inp.is_open()) {
		cerr << RED << "Couldn't open file with repos list!\n" << RESET;
		exit(EXIT_FAILURE);
		system("pause");
	}

	int updatedrepos=0, totalrepos=0;
	for (string currentline; getline(inp, currentline); totalrepos++) {
		vector<string> arguments(SplitString(currentline));
		while (arguments.size() < 3) arguments.push_back("");
		cout << WHITE << "Updating repository at " << arguments[0] << " (remote: '" << arguments[1] << "', branch: '" << arguments[2] << "')\n" << RESET;

		string command = "git --git-dir=" + arguments[0] + ".git --work-tree=" + arguments[0] + " pull " + arguments[1] + " " + arguments[2] + " > autopull_log.txt 2> autopull_error.txt";

		bool connectionerror = 0;
		for (int i = 0; i < 3; i++) {
			system(command.c_str()); // exec pull, out to err.txt

			ifstream checkerror("autopull_error.txt");
			string error;
			getline(checkerror, error);
			checkerror.close();

			system("del autopull_log.txt > nul 2> nul");
			system("del autopull_error.txt > nul 2> nul");

			if (error.empty() || error.find("From") != string::npos) {
				cout << GREEN << "Updated!\n" << RESET;
				connectionerror = 0;
				updatedrepos++;
				break;
			}

			//cout << error << "\n";

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
				errortype = "Not a git repository, incorrect remote or access denied";
				fatal = 1;
			}

			if (fatal) {
				cout << RED << "Update failed! (" << errortype << ")\n" << RESET;
				break;
			}
			else cout << YELLOW << "Update failed! (" << errortype << ", attempt " << i + 1 << "/3)\n" << RESET;

			if (i < 2) {
				string timeout = "timeout " + to_string(5 * (i + 1));
				system(timeout.c_str());
			}
		}
		if (connectionerror) {
			//cerr << RED << "Connection error! Aborting. \n" << RESET;
			break;
		}
		cout << "\n";
	}
	inp.close();
	
	if (updatedrepos == 0) {
		cerr << RED << "Update failed! No repositories updated!\n" << RESET;
		system("pause");
	}
	else if (updatedrepos != totalrepos) {
		cout << YELLOW << "Updated " << updatedrepos << "/" << totalrepos << " repositories! See log for details.\n" << RESET;
		system("pause");
	}
	else cout << GREEN << "Succesfully updated!\n" << RESET;
	return 0;
}
