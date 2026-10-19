#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H
#include <string>
#include <vector>
#include "FileManager.h"
#include "User.h"
class TransactionManager {
    std::vector<User> users; User* current = nullptr; int session = 0;
public:
    void loadData(FileManager& fm) { users = fm.readUsers(); }
    User* findUser(const std::string& n) { for (auto& u : users) if (u.username == n) return &u; return nullptr; }
    void beginSession(User& u, int s) { current = &u; session = s; }
    void endSession(FileManager& fm) {
        char buf[64]; std::snprintf(buf, sizeof buf, "00 %-15s %s %09.2f", current->username.c_str(), current->userType.c_str(), 0.0);
        fm.saveTransaction(buf); current->resetSessionCredit(); current = nullptr;
    }
    void processTransaction(const std::string&, FileManager&) {}
};
#endif