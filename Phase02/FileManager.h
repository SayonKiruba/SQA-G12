#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include "User.h"
class FileManager {
    std::string dataDirectory; std::vector<std::string> transactions;
public:
    explicit FileManager(const std::string& d) : dataDirectory(d) {}
    std::vector<User> readUsers() {
        std::vector<User> v; std::ifstream f(dataDirectory + "/current-users.txt"); std::string l;
        while (std::getline(f,l) && l != "END") {
            std::string n = l.substr(0,15); n.erase(n.find_last_not_of(' ')+1);
            v.emplace_back(n, l.substr(16,2), std::stod(l.substr(19)));
        }
        return v;
    }
    void saveTransaction(const std::string& r) { transactions.push_back(r); }
    bool writeDailyTransactions(const std::string& fn) {
        std::ofstream o(fn, std::ios::app); for (auto& t : transactions) o << t << "\n"; return bool(o);
    }
    void clearTransactions() { transactions.clear(); }
};
#endif
