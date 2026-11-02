#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H
#include <string>
#include <vector>
#include "FileManager.h"
#include "User.h"

class TransactionManager {
    std::vector<User> users;
    std::vector<Game> games;
    std::vector<GameOwnership> collections;
    std::string currentUsername;
    int session = 0;

    User* currentUser();
    const User* currentUser() const;

public:
    void loadData(FileManager& fm);
    User* findUser(const std::string& username);
    Game* findGame(const std::string& name);
    bool ownsGame(const std::string& username, const std::string& gameName) const;
    void beginSession(User& user, int sessionNumber);
    void endSession(FileManager& fm);
    void processTransaction(const std::string& command, FileManager& fm);
    void createUser(FileManager& fm);
    void deleteUser(FileManager& fm);
    void sellGame(FileManager& fm);
    void buyGame(FileManager& fm);
    void refund(FileManager& fm);
    void addCredit(FileManager& fm);
    void listGames() const;
    void listAccounts() const;
};
#endif