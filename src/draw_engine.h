#ifndef DRAW_ENGINE_H
#define DRAW_ENGINE_H

#include <vector>
#include "receipt.h"
#include "file_manager.h"

struct DrawResult {
    std::string drawDate;
    std::vector<WinnerRecord> winners;
    double totalCollected = 0.0;   // total lottery fees collected from all qualifying receipts
    double totalPaidOut = 0.0;     // sum of all prizes actually paid this draw
    double operatorSurplus = 0.0; // totalCollected - totalPaidOut
};

class DrawEngine {
public:
    DrawEngine(int numWinners, double prizePerWinner);

    // Picks numWinners unique qualifying receipts at random and pays each
    // prizePerWinner. If fewer qualifying receipts exist than numWinners,
    // it pays as many winners as there are receipts (no duplicates, no crash).
    DrawResult runDraw(const std::vector<Receipt>& receipts, const std::string& drawDate) const;

private:
    int numWinners;
    double prizePerWinner;
};

#endif
