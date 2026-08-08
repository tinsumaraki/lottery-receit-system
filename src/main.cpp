#include <iostream>
#include <iomanip>
#include <limits>
#include "receipt.h"
#include "lottery_number.h"
#include "file_manager.h"
#include "draw_engine.h"

const std::string RECEIPTS_FILE = "data/receipts.csv";
const std::string WINNERS_FILE = "data/winners.csv";
const int WINNERS_PER_DRAW = 10;
const double PRIZE_PER_WINNER = 1000000.0; // 1,000,000 birr

void printReceipt(const Receipt& r) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Receipt #" << r.receiptId << " ---\n";
    std::cout << "Item amount:     " << r.itemAmount << " birr\n";
    std::cout << "VAT (15%):       " << r.vatAmount << " birr\n";
    std::cout << "Lottery fee:     " << (r.feeApplied ? "1.00 birr" : "not applied") << "\n";
    std::cout << "Total charged:   " << r.total() << " birr\n";
    if (r.feeApplied) {
        std::cout << "Lottery number:  " << r.lotteryNumber << "\n";
    }
    std::cout << "----------------------------\n";
}

void addReceiptFlow(FileManager& fm, LotteryNumberGenerator& gen) {
    double amount;
    std::cout << "\nEnter item price (birr): ";
    while (!(std::cin >> amount) || amount < 0) {
        std::cout << "Please enter a valid non-negative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int id = fm.nextReceiptId();
    Receipt r = createReceipt(id, amount, "2026-08-08", gen);
    fm.appendReceipt(r);
    printReceipt(r);
}

void runDrawFlow(FileManager& fm) {
    auto receipts = fm.loadReceipts();
    int eligibleCount = 0;
    for (const auto& r : receipts) if (r.feeApplied) eligibleCount++;

    if (eligibleCount == 0) {
        std::cout << "\nNo qualifying receipts yet. Add some receipts first.\n";
        return;
    }

    DrawEngine engine(WINNERS_PER_DRAW, PRIZE_PER_WINNER);
    DrawResult result = engine.runDraw(receipts, "2026-08-08");

    for (const auto& w : result.winners) {
        fm.appendWinner(w);
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Draw Results (" << result.drawDate << ") ===\n";
    std::cout << "Eligible receipts:      " << eligibleCount << "\n";
    std::cout << "Total fees collected:    " << result.totalCollected << " birr\n";
    std::cout << "Winners this draw:       " << result.winners.size() << "\n";
    std::cout << "Total prize paid out:    " << result.totalPaidOut << " birr\n";
    std::cout << "Operator surplus:        " << result.operatorSurplus << " birr\n";
    std::cout << "\nWinning numbers:\n";
    for (const auto& w : result.winners) {
        std::cout << "  " << w.lotteryNumber << "  ->  " << w.prizeAmount << " birr\n";
    }
    std::cout << "===================================\n";
}

void showReport(FileManager& fm) {
    auto receipts = fm.loadReceipts();
    auto winners = fm.loadWinners();

    double totalFees = 0;
    int qualifying = 0;
    for (const auto& r : receipts) {
        if (r.feeApplied) { totalFees += LOTTERY_FEE; qualifying++; }
    }

    double totalPrizesPaid = 0;
    for (const auto& w : winners) totalPrizesPaid += w.prizeAmount;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== System Report ===\n";
    std::cout << "Total receipts issued:      " << receipts.size() << "\n";
    std::cout << "Qualifying (>=100 birr):    " << qualifying << "\n";
    std::cout << "Total lottery fees:         " << totalFees << " birr\n";
    std::cout << "Total winners so far:       " << winners.size() << "\n";
    std::cout << "Total prizes paid so far:   " << totalPrizesPaid << " birr\n";
    std::cout << "======================\n";
}

int main() {
    FileManager fm(RECEIPTS_FILE, WINNERS_FILE);
    LotteryNumberGenerator gen;
    gen.loadExisting(fm.loadUsedLotteryNumbers());

    int choice = 0;
    while (choice != 4) {
        std::cout << "\n===== Receipt-Linked Lottery System =====\n";
        std::cout << "1. Add a new receipt\n";
        std::cout << "2. Run quarterly draw\n";
        std::cout << "3. View report\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addReceiptFlow(fm, gen); break;
            case 2: runDrawFlow(fm); break;
            case 3: showReport(fm); break;
            case 4: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid option, try again.\n";
        }
    }
    return 0;
}
