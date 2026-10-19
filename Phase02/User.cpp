//
// Created by lodhi on 2026-10-18.
//
#include "User.h"
#include <cmath>

static long long toCents(double d) { return std::llround(d * 100.0); }

bool User::adjustCredit(double amount) {
    long long next = toCents(credit) + toCents(amount);
    if (next < 0 || next > MAX_BALANCE_CENTS) return false;
    credit = next / 100.0;
    return true;
}

