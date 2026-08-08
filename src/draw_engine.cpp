#include "draw_engine.h"
#include <algorithm>
#include <random>

DrawEngine::DrawEngine(int numWinners, double prizePerWinner)
    : numWinners(numWinners), prizePerWinner(prizePerWinner) {}

DrawResult DrawEngine::runDraw(const std::vector<Receipt>& receipts, const std::string& drawDate) const {
    DrawResult result;
    result.drawDate = drawDate;

    // Only receipts that paid the 1-birr fee are eligible for the draw.
    std::vector<long long> eligibleNumbers;
    for (const Receipt& r : receipts) {
        if (r.feeApplied) {
            eligibleNumbers.push_back(r.lotteryNumber);
            result.totalCollected += LOTTERY_FEE;
        }
    }

    // Shuffle eligible numbers, then take the first N as winners.
    // This guarantees no duplicate winners without needing a retry loop.
    static std::random_device rd;
    static std::mt19937 engine(rd());
    std::shuffle(eligibleNumbers.begin(), eligibleNumbers.end(), engine);

    int winnersToPick = std::min(numWinners, static_cast<int>(eligibleNumbers.size()));
    for (int i = 0; i < winnersToPick; ++i) {
        WinnerRecord w;
        w.drawDate = drawDate;
        w.lotteryNumber = eligibleNumbers[i];
        w.prizeAmount = prizePerWinner;
        result.winners.push_back(w);
        result.totalPaidOut += prizePerWinner;
    }

    result.operatorSurplus = result.totalCollected - result.totalPaidOut;
    return result;
}
