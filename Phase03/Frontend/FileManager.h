#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include <vector>
#include "User.h"
class FileManager {
    std::string dataDirectory; std::vector<std::string> transactions;

    static std::string trimRight(std::string value);
    static bool takeLastField(std::string& line, std::string& field);

public:
    explicit FileManager(const std::string& d) : dataDirectory(d) {}
    std::vector<User> readUsers();
    std::vector<Game> readGames();
    std::vector<GameOwnership> readCollections();
    void saveTransaction(const std::string& record);
    bool writeDailyTransactions(const std::string& filename);
    void clearTransactions();
};
#endif
