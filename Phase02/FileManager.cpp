#include "FileManager.h"

#include <cctype>
#include <fstream>
#include <sstream>

std::string FileManager::trimRight(std::string value) {
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
        value.pop_back();
    }
    return value;
}

bool FileManager::takeLastField(std::string& line, std::string& field) {
    line = trimRight(line);
    const std::string::size_type end = line.find_last_not_of(" \t\r\n");
    if (end == std::string::npos) return false;

    const std::string::size_type separator = line.find_last_of(" \t", end);
    const std::string::size_type start =
        separator == std::string::npos ? 0 : separator + 1;
    field = line.substr(start, end - start + 1);
    line = separator == std::string::npos
        ? std::string()
        : trimRight(line.substr(0, separator));
    return true;
}

std::vector<User> FileManager::readUsers() {
    std::vector<User> users;
    std::ifstream input(dataDirectory + "/current-users.txt");
    std::string line;

    while (std::getline(input, line) && line != "END") {
        std::string username = line.substr(0, 15);
        const std::string::size_type lastCharacter = username.find_last_not_of(' ');
        username.erase(lastCharacter == std::string::npos ? 0 : lastCharacter + 1);
        users.emplace_back(username, line.substr(16, 2), std::stod(line.substr(19)));
    }
    return users;
}

std::vector<Game> FileManager::readGames() {
    std::vector<Game> games;
    std::ifstream input(dataDirectory + "/available-games.txt");
    std::string line;

    while (std::getline(input, line) && line != "END") {
        std::string priceText;
        std::string seller;
        if (!takeLastField(line, priceText) ||
            !takeLastField(line, seller) ||
            line.empty()) {
            continue;
        }

        std::istringstream priceStream(priceText);
        double price = 0;
        char extra = '\0';
        if (!(priceStream >> price) || (priceStream >> extra)) continue;

        games.push_back(Game{line, seller, price, 0});
    }
    return games;
}

std::vector<GameOwnership> FileManager::readCollections() {
    std::vector<GameOwnership> collections;
    std::ifstream input(dataDirectory + "/game-collection.txt");
    std::string line;

    while (std::getline(input, line) && line != "END") {
        std::string username;
        if (!takeLastField(line, username) || line.empty()) continue;
        collections.push_back(GameOwnership{line, username});
    }
    return collections;
}

void FileManager::saveTransaction(const std::string& record) {
    transactions.push_back(record);
}

bool FileManager::writeDailyTransactions(const std::string& filename) {
    std::ofstream output(filename, std::ios::app);
    for (const std::string& transaction : transactions) {
        output << transaction << '\n';
    }
    return static_cast<bool>(output);
}

void FileManager::clearTransactions() {
    transactions.clear();
}