#ifndef FRONTEND_H
#define FRONTEND_H
#include <string>
#include "FileManager.h"
#include "TransactionManager.h"

class FrontEnd {
public:
    FrontEnd(const std::string& dataDir, const std::string& transactionOut);
    void run();
    void processCommand(const std::string& command);
    void login();
    void logout();
private:
    bool               loggedIn = false;
    std::string        currentUsername;
    int                sessionNumber = 0;
    FileManager        fileManager;
    TransactionManager transactionManager;
    std::string        transactionOut;
};
#endif

