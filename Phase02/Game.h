#ifndef GAME_H
#define GAME_H
#include <string>

// Plain data holder (UML: name, seller, price, listedSession; no methods).
struct Game {
    std::string name;      // max 25 chars
    std::string seller;    // max 15 chars
    double      price = 0; // 0.00 - 999.99
    int         listedSession = 0;
};
#endif