#ifndef LOTTERY_NUMBER_H
#define LOTTERY_NUMBER_H

#include <unordered_set>

// Generates unique random 8-digit lottery numbers (10,000,000 - 99,999,999).
// Keeps track of numbers already issued so no two receipts ever collide.
class LotteryNumberGenerator {
public:
    LotteryNumberGenerator();

    // Call this once at startup with numbers already stored in receipts.csv,
    // so numbers generated in past runs are never reused.
    void loadExisting(const std::unordered_set<long long>& usedNumbers);

    // Returns a new, guaranteed-unique 8-digit number.
    long long generate();

private:
    std::unordered_set<long long> usedNumbers;
};

#endif
