#include "FrontEnd.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include "User.h"

using std::cin; using std::cout; using std::string;

static string trim(string s) {                      // strips '\r' and surrounding spaces
    while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    size_t i = 0; while (i < s.size() && isspace((unsigned char)s[i])) ++i;
    return s.substr(i);
}
static string normalize(string s) {                 // commands only: trim + lowercase
    s = trim(s);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}

FrontEnd::FrontEnd(const std::string& dataDir, const std::string& out)
    : fileManager(dataDir), transactionOut(out) {
    transactionManager.loadData(fileManager);
}

void FrontEnd::run() {
    string line;
    while (std::getline(cin, line)) {
        string cmd = normalize(line);
        if (cmd.empty()) continue;                  // tests contain stray blank lines
        processCommand(cmd);
    }
}

void FrontEnd::processCommand(const std::string& command) {
    if (command == "login")  { login();  return; }
    if (command == "logout") { logout(); return; }
    if (!loggedIn)           { cout << "Invalid: Not logged in\n"; return; }

    static const char* known[] = {"create","delete","sell","buy","refund","add credit"};
    if (std::find(std::begin(known), std::end(known), command) == std::end(known)) {
        cout << "Error: Invalid Command\n";
        return;
    }
    transactionManager.processTransaction(command, fileManager);
}

void FrontEnd::login() {
    if (loggedIn) { cout << "Invalid: Currently logged into an account\n"; return; }
    cout << "Enter Username:\n";
    string name;
    if (!std::getline(cin, name)) return;
    name = trim(name);                               // usernames stay case-sensitive
    User* u = transactionManager.findUser(name);
    if (!u) { cout << "Invalid: User not found\n"; return; }
    loggedIn = true;
    currentUsername = u->username;
    ++sessionNumber;
    transactionManager.beginSession(*u, sessionNumber);
    cout << "Logged in as " << u->username << "\n";
}

void FrontEnd::logout() {
    if (!loggedIn) { cout << "Invalid: Not logged in\n"; return; }
    transactionManager.endSession(fileManager);      // queues the 00 record
    fileManager.writeDailyTransactions(transactionOut);
    fileManager.clearTransactions();
    loggedIn = false;
    currentUsername.clear();
    cout << "Logged out\n";
}
