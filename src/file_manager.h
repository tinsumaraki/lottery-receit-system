#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <unordered_set>
#include "receipt.h"

struct WinnerRecord {
    std::string drawDate;
    long long lotteryNumber = 0;
    double prizeAmount = 0.0;

    std::string toCsvRow() const;
};

WinnerRecord winnerFromCsvRow(const std::string& row);

// Handles all reading/writing to receipts.csv and winners.csv.
// Creates each file with a header row the first time it's used.
class FileManager {
public:
    FileManager(const std::string& receiptsPath, const std::string& winnersPath);

    std::vector<Receipt> loadReceipts() const;
    std::unordered_set<long long> loadUsedLotteryNumbers() const;
    int nextReceiptId() const;
    void appendReceipt(const Receipt& r) const;

    std::vector<WinnerRecord> loadWinners() const;
    void appendWinner(const WinnerRecord& w) const;

private:
    std::string receiptsPath;
    std::string winnersPath;
    void ensureFileWithHeader(const std::string& path, const std::string& header) const;
};

#endif
