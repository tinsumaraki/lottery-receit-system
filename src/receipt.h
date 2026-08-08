#ifndef RECEIPT_H
#define RECEIPT_H

#include <string>
#include "lottery_number.h"

// Fixed business rules for the system.
constexpr double VAT_RATE = 0.15;          // 15% VAT
constexpr double FEE_THRESHOLD = 100.0;    // minimum pre-VAT amount to qualify
constexpr double LOTTERY_FEE = 1.0;        // flat 1 birr lottery fee

struct Receipt {
    int receiptId = 0;
    double itemAmount = 0.0;
    double vatAmount = 0.0;
    bool feeApplied = false;
    long long lotteryNumber = 0;   // 0 means "did not qualify"
    std::string date;

    double total() const;              // item + VAT + fee (if applied)
    std::string toCsvRow() const;      // serialize for saving to receipts.csv
};

// Builds a Receipt from an item price, applying VAT and the lottery fee rule,
// and generating a unique lottery number if the receipt qualifies.
Receipt createReceipt(int receiptId, double itemAmount, const std::string& date,
                       LotteryNumberGenerator& generator);

// Rebuilds a Receipt from a single CSV line (used when loading receipts.csv).
Receipt receiptFromCsvRow(const std::string& row);

#endif
