#include "lottery_number.h"
#include <random>

LotteryNumberGenerator::LotteryNumberGenerator() {}

void LotteryNumberGenerator::loadExisting(const std::unordered_set<long long>& used) {
    usedNumbers = used;
}

long long LotteryNumberGenerator::generate() {
    // static: the random engine is seeded once and reused across calls,
    // instead of re-seeding (and wasting entropy) every single time.
    static std::random_device rd;
    static std::mt19937_64 engine(rd());
    static std::uniform_int_distribution<long long> dist(10000000LL, 99999999LL);

    long long candidate;
    do {
        candidate = dist(engine);
    } while (usedNumbers.count(candidate) > 0); // retry on collision

    usedNumbers.insert(candidate);
    return candidate;
}
