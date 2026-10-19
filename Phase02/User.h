#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include "Game.h"

class User {
public:
    static constexpr long long MAX_BALANCE_CENTS     = 99999999; // 999999.99
    static constexpr long long MAX_SESSION_ADD_CENTS = 100000;   // 1000.00

    std::string       username;     // max 15 chars
    std::string       userType;     // "AA", "FS", "BS", "SS"
    double            credit = 0;
    double            addedCredit = 0;   // credit added so far this session
    std::vector<Game> ownedGames;

    User() = default;
    User(std::string name, std::string type, double bal)
        : username(std::move(name)), userType(std::move(type)), credit(bal) {}

    // Applies +/- amount; refuses (returns false, no change) if the new
    // balance would fall below 0 or exceed 999999.99.
    bool adjustCredit(double amount);
    void resetSessionCredit() { addedCredit = 0; }

    // Small helpers the UML doesn't list yet - add them to the diagram.
    bool isAdmin()      const { return userType == "AA"; }
    bool canSell()      const { return userType == "AA" || userType == "FS" || userType == "SS"; }
    bool canBuy()       const { return userType == "AA" || userType == "FS" || userType == "BS"; }
};
#endif
