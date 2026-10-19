#include "TransactionManager.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace {
std::string trim(const std::string& value) {
    std::string::size_type first = 0;
    while (first < value.size() && std::isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }
    std::string::size_type last = value.size();
    while (last > first && std::isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }
    return value.substr(first, last - first);
}

std::string lowercase(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool readLine(std::string& value) {
    if (!std::getline(std::cin, value)) return false;
    value = trim(value);
    return true;
}

bool parseCents(const std::string& value, long long& cents) {
    const std::string text = trim(value);
    if (text.empty()) return false;

    char* end = nullptr;
    const long double amount = std::strtold(text.c_str(), &end);
    if (end == text.c_str() || *end != '\0' || !std::isfinite(amount)) return false;

    const long double rounded = std::round(amount * 100.0L);
    if (std::fabs(amount * 100.0L - rounded) > 0.000001L ||
        rounded < static_cast<long double>(std::numeric_limits<long long>::min()) ||
        rounded > static_cast<long double>(std::numeric_limits<long long>::max())) {
        return false;
    }
    cents = static_cast<long long>(rounded);
    return true;
}

long long toCents(double amount) {
    return std::llround(amount * 100.0);
}

double fromCents(long long cents) {
    return static_cast<double>(cents) / 100.0;
}

std::string accountRecord(const char* code, const User& user, double amount) {
    std::ostringstream record;
    record << code << ' ' << std::left << std::setw(15) << user.username
           << ' ' << user.userType << ' ' << std::right << std::setfill('0')
           << std::setw(9) << std::fixed << std::setprecision(2) << amount;
    return record.str();
}

std::string gameRecord(const char* code, const Game& game) {
    std::ostringstream record;
    record << code << ' ' << std::left << std::setw(20) << game.name
           << ' ' << std::setw(15) << game.seller
           << ' ' << std::right << std::setfill('0') << std::setw(6)
           << std::fixed << std::setprecision(2) << game.price;
    return record.str();
}

std::string purchaseRecord(const Game& game, const std::string& buyer) {
    std::ostringstream record;
    record << "04 " << std::left << std::setw(20) << game.name
           << ' ' << std::setw(15) << game.seller
           << ' ' << std::setw(15) << buyer
           << ' ' << std::right << std::setfill('0') << std::setw(6)
           << std::fixed << std::setprecision(2) << game.price;
    return record.str();
}

std::string refundRecord(const User& buyer, const User& seller, double amount) {
    std::ostringstream record;
    record << "05 " << std::left << std::setw(15) << buyer.username
           << ' ' << std::setw(15) << seller.username
           << ' ' << std::right << std::setfill('0') << std::setw(9)
           << std::fixed << std::setprecision(2) << amount;
    return record.str();
}

std::size_t characterCount(const std::string& value) {
    std::size_t count = 0;
    for (unsigned char byte : value) {
        if ((byte & 0xC0) != 0x80) ++count;
    }
    return count;
}
}

User* TransactionManager::currentUser() {
    return findUser(currentUsername);
}

const User* TransactionManager::currentUser() const {
    for (const User& user : users) {
        if (user.username == currentUsername) return &user;
    }
    return nullptr;
}

void TransactionManager::loadData(FileManager& fm) {
    users = fm.readUsers();
    games = fm.readGames();
    collections = fm.readCollections();

    for (const GameOwnership& ownership : collections) {
        User* owner = findUser(ownership.username);
        if (owner && !ownsGame(owner->username, ownership.gameName)) {
            Game ownedGame;
            ownedGame.name = ownership.gameName;
            ownedGame.seller = owner->username;
            owner->ownedGames.push_back(ownedGame);
        }
    }
}

User* TransactionManager::findUser(const std::string& username) {
    for (User& user : users) {
        if (user.username == username) return &user;
    }
    return nullptr;
}

Game* TransactionManager::findGame(const std::string& name) {
    for (Game& game : games) {
        if (game.name == name) return &game;
    }
    return nullptr;
}

bool TransactionManager::ownsGame(const std::string& username, const std::string& gameName) const {
    for (const User& user : users) {
        if (user.username != username) continue;
        for (const Game& game : user.ownedGames) {
            if (game.name == gameName) return true;
        }
        return false;
    }
    return false;
}

void TransactionManager::beginSession(User& user, int sessionNumber) {
    currentUsername = user.username;
    session = sessionNumber;
}

void TransactionManager::endSession(FileManager& fm) {
    User* user = currentUser();
    if (!user) return;
    fm.saveTransaction(accountRecord("00", *user, 0.0));
    user->resetSessionCredit();
    currentUsername.clear();
    session = 0;
}

void TransactionManager::processTransaction(const std::string& command, FileManager& fm) {
    const std::string normalized = lowercase(trim(command));
    if (normalized == "create") {
        createUser(fm);
    } else if (normalized == "delete") {
        deleteUser(fm);
    } else if (normalized == "sell") {
        sellGame(fm);
    } else if (normalized == "buy") {
        buyGame(fm);
    } else if (normalized == "refund") {
        refund(fm);
    } else if (normalized == "add credit") {
        addCredit(fm);
    } else if (normalized == "list games") {
        listGames();
    } else if (normalized == "list accounts") {
        listAccounts();
    } else {
        std::cout << "Error: Invalid Command\n";
    }
}

void TransactionManager::createUser(FileManager& fm) {
    User* creator = currentUser();
    if (!creator || !creator->isAdmin()) {
        std::cout << "Invalid: Not an Admin\n";
        return;
    }

    std::cout << "Enter Username:\n";
    std::string username;
    if (!readLine(username)) return;
    if (characterCount(username) > 15) {
        std::cout << "Invalid: Username too long\n";
        return;
    }
    if (username.empty()) {
        std::cout << "Invalid: Username cannot be empty\n";
        return;
    }
    if (findUser(username)) {
        std::cout << "Invalid: User already exists\n";
        return;
    }

    std::cout << "Enter Type:\n";
    std::string type;
    if (!readLine(type)) return;
    const std::string normalizedType = lowercase(type);
    if (normalizedType == "aa" || normalizedType == "admin" ||
        normalizedType == "full-admin" || normalizedType == "full admin") {
        type = "AA";
    } else if (normalizedType == "fs" || normalizedType == "full-standard" ||
               normalizedType == "full standard") {
        type = "FS";
    } else if (normalizedType == "bs" || normalizedType == "buy-standard" ||
               normalizedType == "buy standard") {
        type = "BS";
    } else if (normalizedType == "ss" || normalizedType == "sell-standard" ||
               normalizedType == "sell standard") {
        type = "SS";
    } else {
        std::cout << "Invalid: Invalid user type\n";
        return;
    }

    users.emplace_back(username, type, 0.0);
    fm.saveTransaction(accountRecord("01", users.back(), 0.0));
    std::cout << "created " << username << '\n';
}

void TransactionManager::deleteUser(FileManager& fm) {
    User* deleter = currentUser();
    if (!deleter || !deleter->isAdmin()) {
        std::cout << "Invalid: Not an Admin\n";
        return;
    }

    std::cout << "Enter Username:\n";
    std::string username;
    if (!readLine(username)) return;
    User* target = findUser(username);
    if (!target) {
        std::cout << "Invalid: User does not exist\n";
        return;
    }
    if (username == currentUsername) {
        std::cout << "Invalid: Cannot delete the user because they are currently logged on\n";
        return;
    }

    fm.saveTransaction(accountRecord("02", *target, 0.0));
    games.erase(std::remove_if(games.begin(), games.end(), [&username](const Game& game) {
        return game.seller == username;
    }), games.end());
    collections.erase(std::remove_if(collections.begin(), collections.end(),
        [&username](const GameOwnership& ownership) {
            return ownership.username == username;
        }), collections.end());
    users.erase(std::remove_if(users.begin(), users.end(), [&username](const User& user) {
        return user.username == username;
    }), users.end());
    std::cout << "Deleted " << username << '\n';
}

void TransactionManager::sellGame(FileManager& fm) {
    User* seller = currentUser();
    if (!seller) return;
    if (!seller->canSell()) {
        std::cout << "Invalid: Cannot sell from a buyer account\n";
        return;
    }
    for (const Game& game : games) {
        if (game.seller == seller->username && game.listedSession == session) {
            std::cout << "Invalid: No further transactions allowed in this session\n";
            return;
        }
    }

    std::cout << "Enter Game Name:\n";
    std::string name;
    if (!readLine(name)) return;
    if (characterCount(name) > 25) {
        std::cout << "Invalid: Game name exceeds maximum of 25 characters\n";
        return;
    }
    if (name.empty()) {
        std::cout << "Invalid: Game name cannot be empty\n";
        return;
    }
    if (findGame(name)) {
        std::cout << "Invalid: Game already exists in seller list\n";
        return;
    }

    std::cout << "Enter Price:\n";
    std::string priceInput;
    if (!readLine(priceInput)) return;
    long long priceCents = 0;
    if (!parseCents(priceInput, priceCents) || priceCents < 1 || priceCents > 99999) {
        std::cout << "Invalid: Price must be between 0.01 and 999.99\n";
        return;
    }

    Game listed{name, seller->username, fromCents(priceCents), session};
    games.push_back(listed);
    fm.saveTransaction(gameRecord("03", listed));
    std::cout << "Game listed in seller list\n";
}

void TransactionManager::buyGame(FileManager& fm) {
    User* buyer = currentUser();
    if (!buyer) return;
    if (!buyer->canBuy()) {
        std::cout << "Invalid: Cannot buy from a sell account\n";
        return;
    }

    std::cout << "Enter Game Name:\n";
    std::string name;
    if (!readLine(name)) return;
    std::vector<Game>::iterator listing = std::find_if(games.begin(), games.end(),
        [&name](const Game& game) { return game.name == name; });
    if (listing == games.end()) {
        std::cout << "Invalid: Game does not exist\n";
        return;
    }
    if (ownsGame(buyer->username, name)) {
        std::cout << "Invalid: Game already owned\n";
        return;
    }

    const Game purchased = *listing;
    User* seller = findUser(purchased.seller);
    if (!seller) {
        std::cout << "Invalid: Seller does not exist\n";
        return;
    }
    if (seller == buyer) {
        std::cout << "Invalid: Cannot buy your own game\n";
        return;
    }

    const long long price = toCents(purchased.price);
    if (toCents(buyer->credit) < price) {
        std::cout << "Invalid: Insufficient credit\n";
        return;
    }
    if (toCents(seller->credit) + price > User::MAX_BALANCE_CENTS) {
        std::cout << "Invalid: Seller credit limit exceeded\n";
        return;
    }

    buyer->adjustCredit(-fromCents(price));
    seller->adjustCredit(fromCents(price));
    buyer->ownedGames.push_back(purchased);
    collections.push_back(GameOwnership{purchased.name, buyer->username});
    games.erase(listing);
    fm.saveTransaction(purchaseRecord(purchased, buyer->username));
    std::cout << "Purchase successful\n";
}

void TransactionManager::refund(FileManager& fm) {
    User* administrator = currentUser();
    if (!administrator || !administrator->isAdmin()) {
        std::cout << "Error: Permission denied. Privileged transaction restricted to admin.\n";
        return;
    }

    std::cout << "Enter Buyer Name:\n";
    std::string buyerName;
    if (!readLine(buyerName)) return;
    User* buyer = findUser(buyerName);

    std::cout << "Enter Seller Name:\n";
    std::string sellerName;
    if (!readLine(sellerName)) return;
    User* seller = findUser(sellerName);

    std::cout << "Enter Credit to Transfer:\n";
    std::string amountInput;
    if (!readLine(amountInput)) return;
    long long amount = 0;
    if (!parseCents(amountInput, amount) || amount <= 0) {
        std::cout << "Invalid: Invalid credit amount\n";
        return;
    }
    if (!buyer) {
        std::cout << "Invalid: Buyer does not exist\n";
        return;
    }
    if (!seller) {
        std::cout << "Invalid: Seller does not exist\n";
        return;
    }
    if (toCents(seller->credit) < amount) {
        std::cout << "Invalid: Seller has insufficient credit\n";
        return;
    }
    if (toCents(buyer->credit) + amount > User::MAX_BALANCE_CENTS) {
        std::cout << "Invalid: Buyer credit limit exceeded\n";
        return;
    }

    buyer->adjustCredit(fromCents(amount));
    seller->adjustCredit(-fromCents(amount));
    fm.saveTransaction(refundRecord(*buyer, *seller, fromCents(amount)));
    std::cout << "Transfer Successful\n";
}

void TransactionManager::addCredit(FileManager& fm) {
    User* actor = currentUser();
    if (!actor) return;

    std::cout << "Enter Amount to Add:\n";
    std::string amountInput;
    if (!readLine(amountInput)) return;
    long long amount = 0;
    if (!parseCents(amountInput, amount) || amount <= 0) {
        std::cout << "Invalid: Invalid credit amount\n";
        return;
    }

    User* recipient = actor;
    if (actor->isAdmin()) {
        std::cout << "Enter Username to add credit to:\n";
        std::string username;
        if (!readLine(username)) return;
        recipient = findUser(username);
        if (!recipient) {
            std::cout << "Invalid: User does not exist\n";
            return;
        }
    }

    if (amount > User::MAX_SESSION_ADD_CENTS ||
        (recipient == actor &&
         toCents(actor->addedCredit) + amount > User::MAX_SESSION_ADD_CENTS)) {
        std::cout << "Invalid: Cannot add more than 1000.00 per session\n";
        return;
    }
    if (toCents(recipient->credit) + amount > User::MAX_BALANCE_CENTS) {
        std::cout << "Invalid: Credit balance limit exceeded\n";
        return;
    }

    recipient->adjustCredit(fromCents(amount));
    if (recipient == actor) {
        actor->addedCredit = fromCents(toCents(actor->addedCredit) + amount);
    }
    fm.saveTransaction(accountRecord("06", *recipient, fromCents(amount)));
    if (actor->isAdmin()) {
        std::cout << "Credit Added to '" << recipient->username << "' Successfully\n";
    } else {
        std::cout << "Credit Added Successfully\n";
    }
}

void TransactionManager::listGames() const {
    for (const Game& game : games) {
        std::cout << game.name << " | " << game.seller << " | "
                  << std::fixed << std::setprecision(2) << game.price << '\n';
    }
}

void TransactionManager::listAccounts() const {
    for (const User& user : users) {
        std::cout << user.username << " | " << user.userType << " | "
                  << std::fixed << std::setprecision(2) << user.credit << '\n';
    }
}